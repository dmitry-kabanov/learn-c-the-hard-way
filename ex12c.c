#include <stdio.h>

int main(int argc, char *argv[])
{
    int i;
    int j;
    char letter;
    for (j = 1; j < argc; j++) {
        for (i = 0, letter = argv[j][i];
             letter != '\0';
             i++, letter = argv[j][i]) {
            
            switch(letter) {
                case 'a':
                case 'A':
                    printf("%d: 'A'\n", i);
                    break;
    
                case 'e':
                case 'E':
                    printf("%d: 'E'\n", i);
                    break;
    
                case 'i':
                case 'I':
                    printf("%d: 'I'\n", i);
                    break;
    
                case 'o':
                case 'O':
                    printf("%d: 'O'\n", i);
                    break;
    
                case 'u':
                case 'U':
                    printf("%d: 'U'\n", i);
                    break;
    
                case 'y':
                case 'Y':
                    if (i > 2) {
                        // it's only sometimes Y
                        printf("%d: 'Y'\n", i);
                    }
                    break;
    
                default:
                    printf("%d: %c is not a vowel\n", i, letter);
                    break;
            }
    
        }
    }

    return 0;
}
