#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    line = nullptr;
}

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
    return EXIT_SUCCESS;
}
