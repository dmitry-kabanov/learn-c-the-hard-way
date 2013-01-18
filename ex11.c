#include <stdio.h>

int main(int argc, char *argv[])
{
    int i;
    if (argc == 1) {
        printf("You don't have any arguments.\n");
    } else if (argc == 2) {
        printf("You only have one argument.\n");
    } else if ((argc - 1) > 1 && (argc - 1) < 4) {
        printf("Here's your arguments:\n");

        for (i = 1; i < argc; i++) {
            printf("%s ", argv[i]);
        }
        printf("\n");
    } else {
        printf("You have too many arguments.\n");
    }

    return 0;
}
