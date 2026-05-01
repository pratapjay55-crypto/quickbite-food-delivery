#ifndef STACK_H
#define STACK_H

#include "common.h"

struct DeliveredOrder {
    char item[50];
    int price;
    char deliveryBoy[50];
};

typedef struct DeliveredOrder DeliveredOrder;

void push(char item[], int price, char deliveryBoy[]);
void displayStack();

#endif