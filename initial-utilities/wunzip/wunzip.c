#include <stdio.h>
#include <stdlib.h>

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
    return EXIT_SUCCESS;
}
