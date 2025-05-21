#include <stdio.h>
#include <stdlib.h>

/**
* @brief Entry point for the wcat program, which prints file contents to standard output.
*
* @param argc The number of command-line arguments.
* @param argv An array of command-line arguments (input file paths).
* @return Exits with EXIT_SUCCESS on successful program execution.
*         Exits with EXIT_FAILURE if the input file cannot be opened.
*/
int main(const int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        FILE* fp = fopen(argv[i], "r");
        if (fp == nullptr)
        {
            printf("wcat: cannot open file\n");
            exit(EXIT_FAILURE);
        }

        char* line = nullptr;
        size_t len = 0;
        ssize_t num_read;
        while ((num_read = getline(&line, &len, fp)) != -1)
            fwrite(line, num_read, 1, stdout);

        free(line);
        fclose(fp);
    }
    exit(EXIT_SUCCESS);
}
