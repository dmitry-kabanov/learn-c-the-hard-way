#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("You must use only one argument.\n");
        return 1;
    }

    int i;
    for (i = 0; argv[1][i] != '\0'; i++) {
        char letter = argv[1][i];

        if (letter >= 'A' && letter <= 'Z') {
            letter += 32;
        }

        switch(letter) {
            case 'a':
                printf("%d: 'A'\n", i);
                break;

            case 'e':
                printf("%d: 'E'\n", i);
                break;

            case 'i':
                printf("%d: 'I'\n", i);
                break;

            case 'o':
                printf("%d: 'O'\n", i);
                break;

            case 'u':
                printf("%d: 'U'\n", i);
                break;

            case 'y':
                if (i > 2) {
                    // it's only sometimes a vowel.
                    printf("%d: 'Y'\n", i);
                }
                break;

            default:
                printf("%d: %c is not a vowel\n", i, letter);
                break;
        }

    }
    
    return 0;
}
