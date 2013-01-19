#include <stdio.h>

struct Person {
    char *name;
    int age;
    int height;
    int weight;
};

struct Person *Person_create(char *name, int age, int height, int weight)
{
    struct Person *who = malloc(sizeof(struct Person));
    assert(who != NULL);

    who->name = name;
    who->age = age;
    who->height = height;
    who->weight = weight;

    return who;
}

vooid Person_destroy(struct Person *who)
{
    assert(who != NULL);
    free(who);
}

void Person_print(struct Person *who)
{
    printf("Name :%s\n", who->name);
    printf("\tAge: %d\n", who->age);
    printf("\tHeight: %d\n", who->height);
    printf("\tWeight: %d\n", who->weight);
}

int main(int argc, char *argv[])
{
}

