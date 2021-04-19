/* References: None */
#include <stdio.h>
#include <stdlib.h>

#define ERR_EXIT(msg) \
    do {              \
        perror(msg);  \
        exit(1);      \
    } while (0)

int main(int argc, char *argv[]) {
    printf("Hello world\n");
    return 0;
}
