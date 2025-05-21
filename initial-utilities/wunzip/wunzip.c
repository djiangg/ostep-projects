#include <stdio.h>
#include <stdlib.h>

/**
* @brief Entry point for the wunzip program, which performs run-length decoding on the input files
 *       and prints the decoded data to standard output.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line arguments (input file paths).
 * @return Exits with EXIT_SUCCESS on successful program execution.
 *         Exits with EXIT_FAILURE if no arguments are provided, or if any of the input files
 *         cannot be opened.
 */
int main(const int argc, char* argv[])
{
    if (argc == 1)
    {
        printf("wunzip: file1 [file2 ...]\n");
        exit(EXIT_FAILURE);
    }

    char ch = EOF;
    int count = 0;
    for (int i = 1; i < argc; ++i)
    {
        FILE* fp = fopen(argv[i], "r");
        if (fp == nullptr)
        {
            printf("wunzip: cannot open file\n");
            exit(1);
        }
        while (fread(&count, sizeof(int), 1, fp) > 0)
        {
            fread(&ch, sizeof(char), 1, fp);
            for (int j = 0; j < count; ++j)
                fputc(ch, stdout);
        }
        fclose(fp);
    }
    exit(EXIT_SUCCESS);
}
