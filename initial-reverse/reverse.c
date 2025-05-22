#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "list_node.h"

/**
 * @brief Reads lines from an input file and writes them in reverse order to an output file.
 *
 * @param in A file pointer to the input file.
 * @param out A file pointer to the output file.
 */
void reverse(FILE* in, FILE* out)
{
    ListNode* head = nullptr;
    char* line = nullptr;
    size_t len = 0;
    while (getline(&line, &len, in) != -1)
        head = create_node(line, head); // Acts like a stack, head contains last read line

    const ListNode* curr = head;
    while (curr != nullptr)
    {
        fprintf(out, "%s", curr->data);
        curr = curr->next;
    }

    delete_list(&head);
    free(line);
}

/**
 * @brief Entry point of the reverse program, which reads an input file, reverses its contents,
 *        and writes the result to an output file.
 *
 * If no input or output file is provided, it defaults to standard input/output respectively.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line arguments.
 *             - argv[1] (optional): Path to input file.
 *             - argv[2] (optional): Path to output file.
 * @return Exits with EXIT_SUCCESS on successful program execution.
 *         Exits with EXIT_FAILURE if too many command-line arguments are provided,
 *         if the input or output file cannot be opened, if the input and output files
 *         refer to the same file, or if a memory allocation (malloc) fails.
 */
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
    exit(EXIT_SUCCESS);
}
