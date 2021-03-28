/* References:
 * 1.
 * https://www.geeksforgeeks.org/xor-linked-list-a-memory-efficient-doubly-linked-list-set-1/
 */

#include <stdio.h>
#include <stdlib.h>

#define ERR_EXIT(msg) \
    do {              \
        perror(msg);  \
        exit(1);      \
    } while (0)

// ========================================
// XOR Linked List implementation

typedef struct Node {
    int val;
    struct Node *ptr;
} Node;

typedef struct {
    Node *head;
    Node *tail;
} List;

List *ListCreate() {
    List *list = malloc(sizeof(List));
    list->head = list->tail = NULL;
    return list;
}

Node *XOR(Node *a, Node *b) {
    return (Node *)((unsigned long)a ^ (unsigned long)b);
}

void ListAppend(List *list, int val) {
    Node *node;
    node = (Node *)malloc(sizeof(Node));
    node->val = val;
    node->ptr = list->tail;
    if (list->tail == NULL) {
        list->head = list->tail = node;
    } else {
        list->tail->ptr = XOR(list->tail->ptr, node);
        list->tail = node;
    }
}

void ListPop(List *list) {
    if (list->tail == NULL)
        return;
    Node *node = list->tail;
    if (node->ptr == NULL) {
        list->head = list->tail = NULL;
    } else {
        node->ptr->ptr = XOR(node->ptr->ptr, node);
        list->tail = node->ptr;
    }
    free(node);
}

void ListReverse(List *list) {
    Node *temp = list->head;
    list->head = list->tail;
    list->tail = temp;
}

/* Move all nodes from "from" to "target" */
void ListMerge(List *target, List *from) {
    if (from->head == NULL)
        return;
    if (target->head == NULL) {
        target->head = from->head;
        target->tail = from->tail;
    } else {
        target->tail->ptr = XOR(target->tail->ptr, from->head);
        from->head->ptr = XOR(from->head->ptr, target->tail);
        target->tail = from->tail;
    }
    from->head = from->tail = NULL;
}

void ListPrint(List *list) {
    Node *node = list->head, *prev, *next;
    if (node != NULL) {
        printf("%d", node->val);
        prev = node;
        node = node->ptr;
    }
    while (node != NULL) {
        printf(" %d", node->val);
        next = XOR(prev, node->ptr);
        prev = node;
        node = next;
    }
    printf("\n");
}

// ========================================

int main(int argc, char *argv[]) {
    int i, k, n, a, b, num;
    char buf[8];
    List **lists;

    if ((num = scanf("%d %d", &k, &n)) != 2)
        ERR_EXIT("scanf error");
    lists = (List **)malloc(k * sizeof(List *));
    for (i = 0; i < k; ++i)
        lists[i] = ListCreate();

    for (i = 0; i < n; ++i) {
        if ((num = scanf("%8s", buf)) != 1)
            ERR_EXIT("scanf error");
        a = b = -1;
        switch (buf[0]) {
            case 'e': // enter
                if ((num = scanf("%d %d", &a, &b)) != 2)
                    ERR_EXIT("scanf error");
                ListAppend(lists[a], b);
                break;
            case 'l': // leave
                if ((num = scanf("%d", &a)) != 1)
                    ERR_EXIT("scanf error");
                ListPop(lists[a]);
                break;
            case 'm': // migrate
                if ((num = scanf("%d %d", &a, &b)) != 2)
                    ERR_EXIT("scanf error");
                ListReverse(lists[a]);
                ListMerge(lists[b], lists[a]);
                break;
            default:
                ERR_EXIT("Invalid input");
        }
    }

    for (i = 0; i < k; ++i)
        ListPrint(lists[i]);

    free(lists);
    return 0;
}
