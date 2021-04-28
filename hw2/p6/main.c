/* References: None */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define ERR_EXIT(msg) \
    do {              \
        perror(msg);  \
        exit(1);      \
    } while (0)

#define MAX_VAL 100000

// ========================================
// Vector implementation

#define MIN_CAPACITY 2

typedef struct {
    void **items;
    size_t size;
    size_t capacity;
} Vector;

Vector *NewVector() {
    Vector *vector = (Vector *)malloc(sizeof(Vector));
    vector->items = (void **)malloc(MIN_CAPACITY * sizeof(void *));
    vector->size = 0;
    vector->capacity = MIN_CAPACITY;
    return vector;
}

void VectorFree(Vector *vector) {
    free(vector->items);
    free(vector);
}

void VectorPushBack(Vector *vector, void *x) {
    vector->items[(vector->size)++] = x;

    if (vector->size == vector->capacity) {
        vector->capacity <<= 1;
        void **new_items = (void **)malloc(vector->capacity * sizeof(void *));
        for (int i = 0; i < vector->size; ++i)
            new_items[i] = vector->items[i];
        free(vector->items);
        vector->items = new_items;
    }
}

void *VectorPopBack(Vector *vector) {
    void *x = vector->items[vector->size - 1];
    --(vector->size);

    if (vector->capacity > MIN_CAPACITY &&
        (vector->size << 2) < vector->capacity) {
        vector->capacity >>= 1;
        void **new_items = (void **)malloc(vector->capacity * sizeof(void *));
        for (int i = 0; i < vector->size; ++i)
            new_items[i] = vector->items[i];
        free(vector->items);
        vector->items = new_items;
    }
    return x;
}

void *VectorGet(Vector *vector, int idx) {
    if (idx >= vector->size)
        ERR_EXIT("Vector out of bound");
    return vector->items[idx];
}

void VectorSet(Vector *vector, int idx, void *x) {
    if (idx >= vector->size)
        ERR_EXIT("Vector out of bound");
    vector->items[idx] = x;
}

size_t VectorSize(Vector *vector) { return vector->size; }

void VectorPrint(Vector *vector, void print_func(void *)) {
    if (vector->size == 0) {
        printf("{}\n");
        return;
    }
    printf("{");
    print_func(vector->items[0]);
    for (int i = 1; i < vector->size; ++i) {
        printf(", ");
        print_func(vector->items[i]);
    }
    printf("}\n");
}

// ========================================
// Doubly circular linked list implementation

typedef struct Node {
    struct Node *prev;
    struct Node *next;
    int val;
    int contained_idx;
} Node;

Node *NewNode(Node *prev, Node *next, int val) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->prev = prev;
    node->next = next;
    node->val = val;
    node->contained_idx = -1;
    return node;
}

Node *NewList() {
    // Return a sentinel node
    Node *node = NewNode(NULL, NULL, 0);
    node->prev = node;
    node->next = node;
    return node;
}

bool ListIsEmpty(Node *list) {
    return list->prev == list && list->next == list;
}

void ListPushFront(Node *list, Node *node) {
    node->prev = list;
    node->next = list->next;
    node->prev->next = node;
    node->next->prev = node;
}

void ListPushBack(Node *list, Node *node) {
    node->prev = list->prev;
    node->next = list;
    node->prev->next = node;
    node->next->prev = node;
}

Node *ListGetFront(Node *list) {
    if (ListIsEmpty(list))
        return NULL;
    return list->next;
}

Node *ListGetBack(Node *list) {
    if (ListIsEmpty(list))
        return NULL;
    return list->prev;
}

Node *ListPopFront(Node *list) {
    if (ListIsEmpty(list))
        return NULL;
    Node *node = list->next;
    node->prev->next = node->next;
    node->next->prev = node->prev;
    return node;
}

Node *ListPopBack(Node *list) {
    if (ListIsEmpty(list))
        return NULL;
    Node *node = list->prev;
    node->prev->next = node->next;
    node->next->prev = node->prev;
    return node;
}

void ListFree(Node *list) {
    Node *node = list;
    while (node->next != list) {
        node = node->next;
        free(node->prev);
    }
    free(node);
}

void ListPrint(Node *list) {
    Node *node = list->next;
    while (node != list) {
        printf("%d -> ", node->val);
        node = node->next;
    }
    printf("nil\n");
}

Node *ListMerge(Node *target, Node *source) {
    if (ListIsEmpty(source)) {
        free(source);
        return target;
    }
    if (ListIsEmpty(target)) {
        free(target);
        return source;
    }
    Node *source_head, *source_tail;
    source_head = source->next;
    source_tail = source->prev;
    source_head->prev = target->prev;
    source_tail->next = target;
    source_head->prev->next = source_head;
    source_tail->next->prev = source_tail;
    free(source);
    return target;
}

// ========================================
// Max heap implementation

// A heap is vector<Node *>
typedef Vector Heap;

Heap *NewHeap() { return NewVector(); }

void HeapFree(Heap *heap) { VectorFree(heap); }

size_t HeapSize(Heap *heap) { return VectorSize(heap); }

int _HeapSiftUp(Heap *heap, int idx) {
    int parent_idx;
    Node *node, *parent;
    while (idx > 0) {
        parent_idx = (idx - 1) >> 1;
        node = (Node *)VectorGet(heap, idx);
        parent = (Node *)VectorGet(heap, parent_idx);
        if (node->val <= parent->val)
            return idx;
        VectorSet(heap, idx, (void *)parent);
        VectorSet(heap, parent_idx, (void *)node);
        parent->contained_idx = idx;
        node->contained_idx = parent_idx;
        idx = parent_idx;
    }
    return 0;
}

void _HeapSiftDown(Heap *heap, int idx) {
    int left_idx, right_idx, largest_idx, heap_size;
    Node *node, *left, *right, *largest;
    heap_size = VectorSize(heap);
    while (idx < heap_size) {
        largest_idx = idx;
        largest = (Node *)VectorGet(heap, largest_idx);
        left_idx = (idx << 1) + 1;
        right_idx = left_idx + 1;
        node = (Node *)VectorGet(heap, idx);
        left = right = NULL;
        if (left_idx < heap_size) {
            left = (Node *)VectorGet(heap, left_idx);
            if (left->val > largest->val) {
                largest_idx = left_idx;
                largest = left;
            }
        }
        if (right_idx < heap_size) {
            right = (Node *)VectorGet(heap, right_idx);
            if (right->val > largest->val) {
                largest_idx = right_idx;
                largest = right;
            }
        }
        if (largest_idx == idx)
            return;
        VectorSet(heap, idx, largest);
        VectorSet(heap, largest_idx, (void *)node);
        largest->contained_idx = idx;
        node->contained_idx = largest_idx;
        idx = largest_idx;
    }
}

int HeapGetMax(Heap *heap) {
    if (VectorSize(heap) == 0)
        return -1;
    return ((Node *)VectorGet(heap, 0))->val;
}

Node *HeapExtractMax(Heap *heap) {
    if (VectorSize(heap) == 0)
        return NULL;
    if (VectorSize(heap) == 1)
        return (Node *)VectorPopBack(heap);
    Node *node = (Node *)VectorGet(heap, 0);
    Node *last = (Node *)VectorPopBack(heap);
    VectorSet(heap, 0, last);
    last->contained_idx = 0;
    _HeapSiftDown(heap, 0);
    return node;
}

int HeapInsert(Heap *heap, Node *node) {
    VectorPushBack(heap, (void *)node);
    node->contained_idx = VectorSize(heap) - 1;
    return _HeapSiftUp(heap, VectorSize(heap) - 1);
}

void HeapDelete(Heap *heap, int idx) {
    ((Node *)VectorGet(heap, idx))->val = MAX_VAL + 1;
    _HeapSiftUp(heap, idx);
    free(HeapExtractMax(heap));
}

Heap *HeapMerge(Heap *heap1, Heap *heap2) {
    Heap *temp;
    if (HeapSize(heap1) < HeapSize(heap2)) {
        temp = heap1;
        heap1 = heap2;
        heap2 = temp;
    }
    size_t heap2_size = HeapSize(heap2);
    for (int i = 0; i < heap2_size; ++i) {
        HeapInsert(heap1, (Node *)VectorGet(heap2, i));
    }

    HeapFree(heap2);
    return heap1;
}

void _print_node(void *ptr) { printf("%d", ((Node *)ptr)->val); }

void HeapPrint(Heap *heap) { VectorPrint(heap, _print_node); }

// ========================================
// Production line implementation

typedef struct {
    Node *list;
    Heap *heap;
} Line;

Line *NewLine() {
    Line *line = (Line *)malloc(sizeof(Line));
    line->list = NewList();
    line->heap = NewHeap();
    return line;
}

void LineFree(Line *line) {
    ListFree(line->list);
    HeapFree(line->heap);
    free(line);
}

size_t LineSize(Line *line) { return HeapSize(line->heap); }

void LinePushBack(Line *line, int x) {
    Node *node = NewNode(NULL, NULL, x);
    ListPushBack(line->list, node);
    HeapInsert(line->heap, node);
}

int LineGetFront(Line *line) { return ListGetFront(line->list)->val; }

int LineGetBack(Line *line) { return ListGetBack(line->list)->val; }

int LineGetMax(Line *line) { return HeapGetMax(line->heap); }

int LinePopFront(Line *line) {
    Node *node = ListPopFront(line->list);
    int x = node->val;
    HeapDelete(line->heap, node->contained_idx);
    return x;
}

int LinePopBack(Line *line) {
    Node *node = ListPopBack(line->list);
    int x = node->val;
    HeapDelete(line->heap, node->contained_idx);
    return x;
}

int LinePopMax(Line *line) {
    Node *node = HeapExtractMax(line->heap);
    int x = node->val;
    node->prev->next = node->next;
    node->next->prev = node->prev;
    free(node);
    return x;
}

void LineMerge(Line *target, Line *source) {
    target->list = ListMerge(target->list, source->list);
    target->heap = HeapMerge(target->heap, source->heap);
    free(source);
}

void LinePrint(Line *line) {
    printf("\nLine:\n");
    printf("  list: ");
    ListPrint(line->list);
    printf("  heap: ");
    HeapPrint(line->heap);
    printf("\n");
}

// ========================================

#define PUSH  0
#define MERGE 1

typedef struct {
    int op;
    int a;
    int b;
} Operation;

static int N, O, L;

Operation operations[MAX_VAL << 1];
int sequence[MAX_VAL];

// ========================================

bool sequence_possible() {
    Line **lines;
    int *pop_idxs, i, j, num, op, a, b, line_num, front, back, max;
    bool ans;

    lines = (Line **)malloc(L * sizeof(Line *));
    for (i = 0; i < L; ++i)
        lines[i] = NewLine();
    pop_idxs = (int *)malloc(N * sizeof(int));
    for (i = 0; i < N; ++i)
        pop_idxs[i] = -1;

    i = j = 0;
    while (1) {
        if (i == N) {
            ans = true;
            break;
        }
        num = sequence[i] - 1;
        if (pop_idxs[num] != -1) {
            // Do pop
            line_num = pop_idxs[num];
            front = LineGetFront(lines[line_num]);
            back = LineGetBack(lines[line_num]);
            max = LineGetMax(lines[line_num]);
            pop_idxs[front] = -1;
            pop_idxs[back] = -1;
            pop_idxs[max] = -1;
            if (front == num) {
                LinePopFront(lines[line_num]);
            } else if (back == num) {
                LinePopBack(lines[line_num]);
            } else { // max == num
                LinePopMax(lines[line_num]);
            }
            if (LineSize(lines[line_num])) {
                pop_idxs[LineGetFront(lines[line_num])] = line_num;
                pop_idxs[LineGetBack(lines[line_num])] = line_num;
                pop_idxs[LineGetMax(lines[line_num])] = line_num;
            }
            ++i;
        } else {
            if (j == O) {
                ans = false;
                break;
            }
            // Do operation
            op = operations[j].op;
            a = operations[j].a;
            b = operations[j].b;
            if (op == PUSH) {
                if (LineSize(lines[b])) {
                    pop_idxs[LineGetFront(lines[b])] = -1;
                    pop_idxs[LineGetBack(lines[b])] = -1;
                    pop_idxs[LineGetMax(lines[b])] = -1;
                }
                LinePushBack(lines[b], a - 1);
                pop_idxs[LineGetFront(lines[b])] = b;
                pop_idxs[LineGetBack(lines[b])] = b;
                pop_idxs[LineGetMax(lines[b])] = b;
            } else { // MERGE
                if (LineSize(lines[a])) {
                    pop_idxs[LineGetFront(lines[a])] = -1;
                    pop_idxs[LineGetBack(lines[a])] = -1;
                    pop_idxs[LineGetMax(lines[a])] = -1;
                }
                if (LineSize(lines[b])) {
                    pop_idxs[LineGetFront(lines[b])] = -1;
                    pop_idxs[LineGetBack(lines[b])] = -1;
                    pop_idxs[LineGetMax(lines[b])] = -1;
                }
                LineMerge(lines[b], lines[a]);
                lines[a] = NULL;
                if (LineSize(lines[b])) {
                    pop_idxs[LineGetFront(lines[b])] = b;
                    pop_idxs[LineGetBack(lines[b])] = b;
                    pop_idxs[LineGetMax(lines[b])] = b;
                }
            }
            ++j;
        }
    }

    for (i = 0; i < L; ++i) {
        if (lines[i] != NULL)
            LineFree(lines[i]);
    }
    free(lines);
    free(pop_idxs);
    return ans;
}

// ========================================

int main(int argc, char *argv[]) {
    int n, t, i;
    char buf[8];
    if ((n = scanf("%d", &t)) != 1)
        ERR_EXIT("scanf");
    while (t--) {
        if ((n = scanf("%d %d %d", &N, &O, &L)) != 3)
            ERR_EXIT("scanf");
        for (i = 0; i < O; ++i) {
            if ((n = scanf("%s %d %d", buf, &(operations[i].a),
                           &(operations[i].b))) != 3)
                ERR_EXIT("scanf");
            operations[i].op = (buf[0] == 'p') ? PUSH : MERGE;
        }
        for (i = 0; i < N; ++i) {
            if ((n = scanf("%d", sequence + i)) != 1)
                ERR_EXIT("scanf");
        }
        if (sequence_possible()) {
            printf("possible\n");
        } else {
            printf("impossible\n");
        }
    }
    return 0;
}
