#include <stdio.h>

int main(int argc, char *argv[])
{
    int i;
    
    // Go through each string in argv
    // Why am I skipping argv[0]?
    for (i = argc - 1; i > 0; i--) {
        printf("arg %d: %s\n", i, argv[i]);
    }

    char *states[] = {
        "California",
        "Oregon",
        "Washington",
        "Texas"
    };
    int num_states = 4;

    for (i = num_states - 1; i >= 0; i--) {
        printf("state %d: %s\n", i, states[i]);
    }

    return 0;
}
