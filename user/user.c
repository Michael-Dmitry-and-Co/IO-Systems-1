#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static bool check_if_num(char* str)
{
    char* coursor = str;
    while (*coursor)
    {
        ++coursor;
        if (!isdigit(*coursor))
            return false;
    }
    return true;
}

int main(int argc, char **argv)
{
    FILE *f = fopen("/dev/fan_driver", "r+");
    if (f == NULL)
    {
        printf("Driver not exist!\n");
        return -1;
    }

    if (argc == 1)
    {
        printf("Command required!\n");
        return -1;
    }

    char *command = argv[1];
    uint8_t val;
    if (strcmp("read", command) == 0)
    {
        if (fread(&val, sizeof(uint8_t), 1, f) < 1)
        {
            printf("Error while read\n");
            return -1;
        }
        else
        {
            printf("Speed is: %s", &val);
        }
    }
    else if (strcmp("write", command) == 0)
    {
        if (argc == 2)
        {
            printf("New speed val required!\n");
            return -1;
        }
        if (!check_if_num(argv[2]))
        {
            printf("New speed val should be numeric!\n");
        }
        val = atoi(argv[2]);
        if (val < 0 || val > 255)
        {
            printf("New speed val should be in [0; 255]!\n");
        }
        if (fwrite(&val, 1, 1, f) < 1)
        {
            printf("Error while write");
            return -1;
        }
        else
        {
            printf("Speed changed to: %s", &val);
        }
    } else {
        printf("Command unknown!");
        return -1;
    }
    return 0;
}
