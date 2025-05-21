#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Prints lines from a file that contain a specified search term.
 *
 * @param fp A file pointer to the input file.
 * @param search_term The search term to look for.
 */
void print_matching_lines(FILE* fp, const char* search_term)
{
    char* line = nullptr;
    size_t len = 0;
    ssize_t num_read = 0;
    while ((num_read = getline(&line, &len, fp)) != -1)
    {
        if (strstr(line, search_term) != nullptr)
            fwrite(line, num_read, 1, stdout);
    }
    free(line);
}

/**
 * @brief Entry point for the wgrep program, which searches files for lines containing a
 *        search term.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line arguments.
 *             - argv[1]: The search term to look for.
 *             - argv[2] ... argv[argc-1] (optional): Input file paths.
 * @return Exits with EXIT_SUCCESS on successful program execution.
 *         Exits with EXIT_FAILURE if no arguments are provided, or if any of the input files
 *         cannot be opened.
 */
int main(const int argc, char* argv[])
{
    if (argc == 1)
    {
        printf("wgrep: searchterm [file ...]\n");
        exit(EXIT_FAILURE);
    }
    if (argc == 2)
        print_matching_lines(stdin, argv[1]);

    for (int i = 2; i < argc; ++i)
    {
        FILE* fp = fopen(argv[i], "r");
        if (fp == nullptr)
        {
            printf("wgrep: cannot open file\n");
            exit(EXIT_FAILURE);
        }
        print_matching_lines(fp, argv[1]);
        fclose(fp);
    }
    exit(EXIT_SUCCESS);
}
