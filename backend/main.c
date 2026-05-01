#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue.h"
#include "stack.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("{\"error\": \"No command given\"}\n");
        return 1;
    }

    if (strcmp(argv[1], "enqueue") == 0) {
        if (argc < 4) {
            printf("{\"error\": \"Usage: enqueue item price\"}\n");
            return 1;
        }
        enqueue(argv[2], atoi(argv[3]));
        return 0;
    }

    if (strcmp(argv[1], "dequeue") == 0) {
        dequeue();
        return 0;
    }

    if (strcmp(argv[1], "queue") == 0) {
        displayQueue();
        return 0;
    }

    if (strcmp(argv[1], "history") == 0) {
        displayStack();
        return 0;
    }

    printf("{\"error\": \"Unknown command\"}\n");
    return 1;
}