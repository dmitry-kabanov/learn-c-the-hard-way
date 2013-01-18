#include <stdio.h>
#include <ctype.h>

// forward declarations
void print_letters(char arg[]);

void print_arguments(int argc, char *argv[])
{
    int i = 0;
    for (i = 0; i < argc; i++) {
        print_letters(argv[i]);
    }
}

void print_letters(char arg[])
{
    int i;
    for (i = 0; arg[i] != '\0'; i++) {
        char c = arg[i];

        if (isalpha(c) || isblank(c)) {
            printf("'%c' == %d ", c, c);
        }
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    print_arguments(argc, argv);
    return 0;
}
