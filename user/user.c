#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    FILE *f = fopen("/dev/fan_driver", "r+");
    if (f == NULL)
    {
        printf("Driver not exist!");
        return -1;
    }

    if (argc == 1)
    {
        printf("Command required!");
    }

    char *command = argv[1];
    uint8_t val;
    if (strcmp("read", command))
    {
        if (fread(&val, sizeof(uint8_t), 1, f) < 0)
        {
            printf("Error while read");
            return -1;
        }
        else
        {
            printf("Speed is: %s", &val);
            return 0;
        }
    }

    if (argc == 2)
    {
        printf("New speed val required!");
    }

    val = atoi(argv[2]);
    if (strcmp("write", command))
    {
        if (fwrite(&val, sizeof(uint8_t), 1, f) < 0)
        {
            printf("Error while write");
            return -1;
        }
        else
        {
            printf("Speed changed to: %s", &val);
            return 0;
        }
    }

    printf("Command unknown!");
    return -1;
}