#include <stdio.h>
#include <stdlib.h>

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
        line = nullptr;
        fclose(fp);
    }
    return EXIT_SUCCESS;
}
