#include <stdio.h>

int main(int argc, char *argv[])
{
    int ages[] = {23, 43, 12, 89, 2};
    char *names[] = {
        "Alan",
        "Frank",
        "Mary",
        "John",
        "Lisa"
    };

    int count = sizeof(ages) / sizeof(int);
    int i;
    for (i = 0; i < count; i++) {
        printf("%s has %d years alive.\n", names[i], ages[i]);
    }
    printf("---\n");

    // setup the pointers to the start of the arrays
    int *cur_age = ages;
    char **cur_name = names;

    // second way using pointers
    for (i = 0; i < count; i++) {
        printf("%s has %d years alive.\n", *(cur_name + i), *(cur_age + i));
    }
    printf("---\n");

    // third way, pointers are just arrays
    for (i = 0; i < count; i++) {
        printf("%s has %d years again.\n", cur_name[i], cur_age[i]);
    }
    printf("---\n");

    cur_age = ages + count - 1;
    cur_name = names + count - 1;
    for ( ; (cur_age - ages) >= 0; cur_age--, cur_name--) {
        printf("%s lived %d years so far.\n", *cur_name, *cur_age);
    }
    printf("---\n");

    for (cur_age = ages, cur_name = names;
         (cur_age - ages) < count;
         cur_age++, cur_name++)
    {
        printf("%s %p %p\n", *cur_name, cur_age, cur_name);
    }

    return 0;
}
