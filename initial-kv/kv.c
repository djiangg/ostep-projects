#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include "list_node.h"

/**
 * @brief Parses a command-line argument, validates it and splits it into command tokens.
 *
 * @param argv The raw input string from the command line.
 * @param tokens Pre-allocated array of 3 strings to hold the command, key, and value.
 *                - tokens[0] is the command character ('a', 'c', 'g', 'd', 'p')
 *                - tokens[1] is the key (optional, depending on command)
 *                - tokens[2] is the value (optional, for 'p' command)
 * @return True if the input is valid, false otherwise.
 */
bool process_arg(const char* argv, char** tokens)
{
    // Make a modifiable copy of the input string for strsep use
    char* argv_copy = strdup(argv);
    char* rest = argv_copy;
    const char* token = nullptr;
    // Tokenize the input by comma, expecting at most 3 components
    for (int i = 0; (token = strsep(&rest, ",")) != nullptr; ++i)
    {
        if (i > 2) // More than 3 components is invalid
        {
            free(argv_copy);
            return false;
        }
        if (token != nullptr)
            tokens[i] = strdup(token);
    }
    free(argv_copy);

    const char* cmd = tokens[0];
    if (strlen(cmd) > 1) // Commands must be a single character
        return false;

    if (cmd[0] != 'a' && cmd[0] != 'c' && cmd[0] != 'g' && cmd[0] != 'd' && cmd[0] != 'p')
        return false;
    if ((cmd[0] == 'a' || cmd[0] == 'c') && (tokens[1] != nullptr || tokens[2] != nullptr))
        return false;
    if ((cmd[0] == 'g' || cmd[0] == 'd') && (tokens[1] == nullptr || tokens[2] != nullptr))
        return false;
    if (cmd[0] == 'p' && (tokens[1] == nullptr || tokens[2] == nullptr || tokens[2][0] == '\0'))
        return false;
    // For 'p', 'g', and 'd' commands: validate the key string is a valid integer
    if (cmd[0] == 'p' || cmd[0] == 'g' || cmd[0] == 'd')
    {
        errno = 0;
        char* end_ptr = nullptr;
        char* n_ptr = tokens[1];
        const long key = strtol(n_ptr, &end_ptr, 10);
        // Use strtol's error checking to validate key
        if ((errno != 0) || (n_ptr == end_ptr) || (n_ptr && *end_ptr != 0))
            return false;
        if (key > INT_MAX || key < INT_MIN)  // Ensure key is within int bounds
            return false;
    }
    return true;
}

/**
 * @brief Executes a given command onto the key-value database.
 *
 * @param tokens An array of strings of size 3: [command, key, value].
 * @param head A double pointer to the head of the database list.
 */
void process_cmd(char** tokens, ListNode** head)
{
    const char cmd = *tokens[0];
    // Parse key if it exists ( for 'p', 'g', 'd' command)
    const int key = (tokens[1] != nullptr) ? (int)strtol(tokens[1], nullptr, 10) : 0;
    // Duplicate the value if it exists (for 'p' command)
    char* value = (tokens[2] != nullptr) ? strdup(tokens[2]) : nullptr;
    switch (cmd)
    {
        case 'p':
            if (!modify_node(*head, key, value))
                *head = create_node(*head, key, value);
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
        default: break; // Should never be reached due to earlier validation
    }
    if (value != nullptr)
        free(value);
}

/**
 * @brief Entry point of the key-value database program.
 *
 * This function parses and processes command-line arguments, performs operations on an in-memory
 * linked list representing a simple key-value database, and saves the list to a file
 * ("database.txt") at the end of execution. The database is loaded from the file on startup if
 * it exists.
 *
 * Commands supported:
 *   - p,<key>,<value>: Put — insert or update a key-value pair into the database.
 *   - g,<key>: Get — retrieve and print the value for a given key.
 *   - d,<key>: Delete — remove a key-value pair from the database.
 *   - c: Clear — delete all nodes from the database.
 *   - a: All — print all key-value pairs in the database.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line arguments.
 * @return Exits with EXIT_SUCCESS on successful program execution.
 *         Exits with EXIT_FAILURE if the database file cannot be opened, or if malloc fails.
 */
int main(const int argc, char* argv[])
{
    if (argc == 1)
        exit(EXIT_SUCCESS);

    FILE* db = nullptr;
    ListNode* head = nullptr;
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

    // Allocate space for up to 3 tokens: command, key, value
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

        // Clean up token strings for the next iteration
        for (int j = 0; j < 3; ++j)
        {
            if (tokens[j] != nullptr)
            {
                free(tokens[j]);
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
    exit(EXIT_SUCCESS);
}
