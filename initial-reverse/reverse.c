#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "list_node.h"

void reverse(FILE* in, FILE* out)
{
    ListNode* head = nullptr;
    char* line = nullptr;
    size_t len = 0;
    while (getline(&line, &len, in) != -1)
        head = create_node(line, head); // Acts like a stack, head will contain last read line

    const ListNode* curr = head;
    while (curr != nullptr)
    {
        fprintf(out, "%s", curr->data);
        curr = curr->next;
    }

    delete_list(&head);
    free(line);
    line = nullptr;
}

int main(const int argc, char* argv[])
{
    if (argc > 3)
    {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(EXIT_FAILURE);
    }

    struct stat in_info, out_info;
    FILE* in = (argc >= 2) ? fopen(argv[1], "r") : stdin;
    FILE* out = (argc == 3) ? fopen(argv[2], "w") : stdout;
    if (in == nullptr || out == nullptr)
    {
        fprintf(stderr, "reverse: cannot open file '%s'\n",
                (in == nullptr) ? argv[1] : argv[2]);
        exit(EXIT_FAILURE);
    }
    if (fstat(fileno(in), &in_info) == -1 || fstat(fileno(out), &out_info) == -1)
    {
        fprintf(stderr, "reverse: error getting file information\n");
        exit(EXIT_FAILURE);
    }
    if (in_info.st_ino == out_info.st_ino)
    {
        fprintf(stderr, "reverse: input and output file must differ\n");
        exit(EXIT_FAILURE);
    }

    reverse(in,out);
    fclose(in);
    fclose(out);
    return EXIT_SUCCESS;
}
