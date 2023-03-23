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
        fclose(f);
        return -1;
    }

    char *command = argv[1];
    uint8_t val;
    bool val_is_good = true;
    if (strcmp("read", command) == 0)
    {
        if (fread(&val, sizeof(uint8_t), 1, f) < 1)
        {
            printf("Error while read\n");
        }
        else
        {
            printf("Speed is: %s", &val);
        }
    }
    else if (strcmp("write", command) == 0)
    {
        if (argc <= 2)
        {
            printf("New speed val required!\n");
        }
        else
        {
            if (check_if_num(argv[2]))
            {
                val = atoi(argv[2]);
                if (val < 0 || val > 255)
                {
                    printf("New speed val should be in [0; 255]!\n");
                }
                else if (fwrite(&val, 1, 1, f) < 1)
                {
                    printf("Error while write");
                }
                else
                {
                    printf("Speed changed to: %s", &val);
                }
            }
            else
            {
                printf("New speed val should be numeric!\n");
            }
        }
    } else 
    {
        printf("Command unknown!");
    }
    fclose(f);
    return 0;
}
