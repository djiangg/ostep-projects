#include <stdio.h>
#include <stdlib.h>

int main(const int argc, char* argv[])
{
    if (argc == 1)
    {
        printf("wzip: file1 [file2 ...]\n");
        exit(EXIT_FAILURE);
    }

    char curr_char = EOF;
    char last_char = EOF;
    int count = 1;
    for (int i = 1; i < argc; ++i)
    {
        FILE* fp = fopen(argv[i], "r");
        if (fp == nullptr) {
            printf("wzip: cannot open file\n");
            exit(EXIT_FAILURE);
        }
        while ((curr_char = (char)fgetc(fp)) != EOF)
        {
            if (curr_char == last_char)
                ++count;
            else
            {
                if (last_char != EOF) // Don't write until last_char has a valid value
                {
                    fwrite(&count, sizeof(int), 1, stdout);
                    fwrite(&last_char, sizeof(char), 1, stdout);
                }
                last_char = curr_char;
                count = 1;
            }
        }
        fclose(fp);
    }
    // Still need to print out the last count-character pair
    fwrite(&count, sizeof(int), 1, stdout);
    fwrite(&last_char, sizeof(char), 1, stdout);

    return EXIT_SUCCESS;
}
