#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include "list_node.h"

bool process_arg(const char* argv, char** tokens)
{
    // Need a static pointer to free strdup malloced memory after using strsep
    char* args_copy = strdup(argv);
    char* rest = args_copy;
    const char* token = nullptr;
    for (int i = 0; (token = strsep(&rest, ",")) != nullptr; ++i)
    {
        if (i > 2) // Valid inputs can only have maximum of 2 commas
        {
            free(args_copy);
            args_copy = nullptr;
            return false;
        }
        if (token != nullptr)
            tokens[i] = strdup(token); // tokens[0] = cmd, tokens[1] = key, tokens[2] = value
    }
    free(args_copy);
    args_copy = nullptr;

    const char* cmd = tokens[0];
    if (strlen(cmd) > 1) // Commands are all one character long
        return false;
    if (cmd[0] != 'a' && cmd[0] != 'c' && cmd[0] != 'g' && cmd[0] != 'd' && cmd[0] != 'p')
        return false;
    if ((cmd[0] == 'a' || cmd[0] == 'c') && (tokens[1] != nullptr || tokens[2] != nullptr))
        return false;
    if ((cmd[0] == 'g' || cmd[0] == 'd') && (tokens[1] == nullptr || tokens[2] != nullptr))
        return false;
    if (cmd[0] == 'p' && (tokens[1] == nullptr || tokens[2] == nullptr || tokens[2][0] == '\0'))
        return false;
    if (cmd[0] == 'p' || cmd[0] == 'g' || cmd[0] == 'd')
    {
        errno = 0;
        char* endptr = nullptr;
        char* nptr = tokens[1];
        const long key = strtol(nptr, &endptr, 10);
        // Use strtol's error checking to validate key
        if ((errno != 0) || (nptr == endptr) || (nptr && *endptr != 0))
            return false;
        if (key > INT_MAX || key < INT_MIN) // Since key values are integers
            return false;
    }
    return true;
}

void process_cmd(char** tokens, ListNode** head)
{
    const char cmd = *tokens[0];
    const int key = (tokens[1] != nullptr) ? (int)strtol(tokens[1], nullptr, 10) : 0;
    char* value = (tokens[2] != nullptr) ? strdup(tokens[2]) : nullptr;
    switch (cmd)
    {
        case 'p':
            if (!modify_node(*head, key, value))
                *head = create_node(*head, key, value); // Node not in list, so create new one
            break;
        case 'g':
            const ListNode* node = find_node(*head, key);
            if (node == nullptr)
                fprintf(stdout, "%d not found\n", key);
            else
                fprintf(stdout, "%d,%s\n", node->key, node->value);
            break;
        case 'd':
            if (!delete_node(head, key))
                fprintf(stdout, "%d not found\n", key);
            break;
        case 'c':
            delete_list(head);
            break;
        case 'a':
            print_list(stdout, *head);
            break;
        default: break;
    }
    if (value != nullptr)
    {
        free(value);
        value = nullptr;
    }
}

int main(const int argc, char* argv[])
{
    if (argc == 1)
        return EXIT_SUCCESS;

    FILE* db = nullptr;
    ListNode* head = nullptr;
    // Read-mode fopen won't work if db doesn't exist
    if (access("database.txt", F_OK) == 0)
    {
        db = fopen("database.txt", "r");
        if (db == nullptr)
        {
            fprintf(stderr, "error opening database file\n");
            exit(EXIT_FAILURE);
        }
        head = create_list(db);
        fclose(db);
    }

    char** tokens = calloc(3, sizeof(char*));
    if (tokens == nullptr)
    {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }
    for(int i = 1; i < argc; ++i)
    {
        if (!process_arg(argv[i], tokens))
            fprintf(stderr, "bad command '%s'\n", argv[i]);
        else
            process_cmd(tokens, &head);

        for (int j = 0; j < 3; ++j)
        {
            if (tokens[j] != nullptr)
            {
                free(tokens[j]); // Next command might malloc and replace tokens[j]
                tokens[j] = nullptr;
            }
        }
    }

    db = fopen("database.txt", "w");
    if (db == nullptr)
    {
        fprintf(stderr, "error opening database file\n");
        exit(EXIT_FAILURE);
    }
    print_list(db, head);
    fclose(db);

    delete_list(&head);
    free(tokens);
    tokens = nullptr;
    return EXIT_SUCCESS;
}
