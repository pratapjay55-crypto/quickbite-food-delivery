#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue.h"
#include "stack.h"

#define STATE_FILE "state.dat"

typedef struct {
    char item[50];
    int price;
    char deliveryBoy[50];
} Order;

typedef struct {
    char item[50];
    int price;
    char deliveryBoy[50];
} HistoryOrder;

typedef struct {
    Order queue[MAX];
    int front;
    int rear;
    int orderCount;
    HistoryOrder history[MAX];
    int historyTop;
} State;

static State st;
static char *boys[] = {"Ravi", "Kiran", "Arjun"};

void loadState() {
    FILE *f = fopen(STATE_FILE, "rb");
    if (f) {
        fread(&st, sizeof(State), 1, f);
        fclose(f);
    } else {
        st.front = -1;
        st.rear = -1;
        st.orderCount = 0;
        st.historyTop = -1;
    }
}

void saveState() {
    FILE *f = fopen(STATE_FILE, "wb");
    if (f) {
        fwrite(&st, sizeof(State), 1, f);
        fclose(f);
    }
}

int isFull() {
    return (st.rear + 1) % MAX == st.front;
}

int isEmpty() {
    return st.front == -1;
}

void enqueue(char item[], int price) {
    loadState();

    if (isFull()) {
        printf("{\"status\": \"error\", \"message\": \"Queue Full!\"}\n");
        return;
    }

    if (st.front == -1) {
        st.front = 0;
        st.rear = 0;
    } else {
        st.rear = (st.rear + 1) % MAX;
    }

    strcpy(st.queue[st.rear].item, item);
    st.queue[st.rear].price = price;
    strcpy(st.queue[st.rear].deliveryBoy, boys[st.orderCount % 3]);
    st.orderCount++;

    saveState();

    printf("{\"status\": \"ok\", \"message\": \"Order placed\", \"item\": \"%s\", \"price\": %d, \"deliveryBoy\": \"%s\"}\n",
           item, price, st.queue[st.rear].deliveryBoy);
}

void dequeue() {
    loadState();

    if (isEmpty()) {
        printf("{\"status\": \"error\", \"message\": \"No orders!\"}\n");
        return;
    }

    char item[50], boy[50];
    int price;
    strcpy(item, st.queue[st.front].item);
    strcpy(boy, st.queue[st.front].deliveryBoy);
    price = st.queue[st.front].price;

    if (st.historyTop < MAX - 1) {
        st.historyTop++;
        strcpy(st.history[st.historyTop].item, item);
        st.history[st.historyTop].price = price;
        strcpy(st.history[st.historyTop].deliveryBoy, boy);
    }

    if (st.front == st.rear) {
        st.front = -1;
        st.rear = -1;
    } else {
        st.front = (st.front + 1) % MAX;
    }

    saveState();

    printf("{\"status\": \"ok\", \"message\": \"Delivered\", \"item\": \"%s\", \"price\": %d, \"deliveryBoy\": \"%s\"}\n",
           item, price, boy);
}

void displayQueue() {
    int i, count;
    loadState();

    if (isEmpty()) {
        printf("{\"status\": \"ok\", \"orders\": []}\n");
        return;
    }

    printf("{\"status\": \"ok\", \"orders\": [");
    i = st.front;
    count = 0;
    while (1) {
        if (count > 0) printf(", ");
        printf("{\"item\": \"%s\", \"price\": %d, \"deliveryBoy\": \"%s\"}",
               st.queue[i].item,
               st.queue[i].price,
               st.queue[i].deliveryBoy);
        count++;
        if (i == st.rear) break;
        i = (i + 1) % MAX;
    }
    printf("]}\n");
}

void push(char item[], int price, char deliveryBoy[]) {
    /* handled inside dequeue directly */
}

void displayStack() {
    int i;
    loadState();

    if (st.historyTop == -1) {
        printf("{\"status\": \"ok\", \"history\": []}\n");
        return;
    }

    printf("{\"status\": \"ok\", \"history\": [");
    for (i = st.historyTop; i >= 0; i--) {
        if (i < st.historyTop) printf(", ");
        printf("{\"item\": \"%s\", \"price\": %d, \"deliveryBoy\": \"%s\"}",
               st.history[i].item,
               st.history[i].price,
               st.history[i].deliveryBoy);
    }
    printf("]}\n");
}