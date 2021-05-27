/* References: None */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define ERR_EXIT(msg) \
    do {              \
        perror(msg);  \
        exit(1);      \
    } while (0)

#define MAX_N     100000
#define HASH_SIZE 400009

// ========================================

typedef struct {
    int i;
    int j;
} Pair;

void fill_pair(Pair *pair, int i, int j) {
    if (i <= j) {
        pair->i = i;
        pair->j = j;
    } else {
        pair->i = j;
        pair->j = i;
    }
}

// ========================================
// Hash table implementation

typedef struct HashItem {
    struct HashItem *next;
    Pair key;
    void *value;
} HashItem;

HashItem *adj_list[HASH_SIZE];
HashItem *vertices[HASH_SIZE];

long hash(Pair pair) {
    long h = (((long)pair.i << 32) | (long)pair.j);
    h *= 1231231557ull;
    h ^= (h >> 32);
    return h % HASH_SIZE;
}

void TableInit(HashItem **table) {
    for (int i = 0; i < HASH_SIZE; ++i)
        table[i] = NULL;
}

HashItem *_TableGetItem(HashItem **table, Pair key) {
    long h = hash(key);
    HashItem *node = table[h];
    while (node != NULL) {
        if (node->key.i == key.i && node->key.j == key.j)
            return node;
        node = node->next;
    }
    return NULL;
}

void *TableGet(HashItem **table, Pair key) {
    HashItem *item = _TableGetItem(table, key);
    if (item != NULL)
        return item->value;
    return NULL;
}

void TableSet(HashItem **table, Pair key, void *value) {
    HashItem *item = _TableGetItem(table, key);
    if (item != NULL) {
        item->value = value;
        return;
    }
    long h = hash(key);
    HashItem *node = (HashItem *)malloc(sizeof(HashItem));
    node->key = key;
    node->value = value;
    node->next = table[h];
    table[h] = node;
}

void FreeTable(HashItem **table, void free_func(void *)) {
    HashItem *node, *next;
    for (int i = 0; i < HASH_SIZE; ++i) {
        if (table[i] != NULL) {
            node = table[i];
            next = node->next;
            while (next != NULL) {
                free_func(node->value);
                free(node);
                node = next;
                next = node->next;
            }
            free_func(node->value);
            free(node);
        }
    }
}

// ========================================

typedef struct Node {
    struct Node *next;
    Pair pair;
} Node;

Node *Push(Node *node, Pair pair) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->next = node;
    new_node->pair = pair;
    return new_node;
}

void FreeList(void *data) {
    Node *node = (Node *)data;
    Node *next = node->next;
    while (next != NULL) {
        free(node);
        node = next;
        next = node->next;
    }
    free(node);
}

// ========================================

typedef enum {
    WHITE,
    GRAY,
    BLACK,
} Color;

typedef struct {
    Pair key;
    Color color;
} Vertex;

Node *topo_list = NULL;

// ========================================

bool DFS_visit(Node *vertices_list, Vertex *vertex) {
    Node *node;
    Vertex *v;

    vertex->color = GRAY;

    node = (Node *)TableGet(adj_list, vertex->key);
    while (node != NULL) {
        v = (Vertex *)TableGet(vertices, node->pair);
        if (v->color == GRAY) {
            return false;
        }
        if (v->color == WHITE) {
            if (!DFS_visit(vertices_list, v))
                return false;
        }
        node = node->next;
    }

    vertex->color = BLACK;
    topo_list = Push(topo_list, vertex->key);
    return true;
}

bool DFS(Node *vertices_list) {
    Node *node;
    Vertex *vertex;
    node = vertices_list;
    while (node != NULL) {
        vertex = (Vertex *)TableGet(vertices, node->pair);
        vertex->color = WHITE;
        node = node->next;
    }
    node = vertices_list;
    while (node != NULL) {
        vertex = (Vertex *)TableGet(vertices, node->pair);
        if (vertex->color == WHITE)
            if (!DFS_visit(vertices_list, vertex))
                return false;
        node = node->next;
    }
    return true;
}

// ========================================

int main(int argc, char *argv[]) {
    int n, N, i, j, l, x, prev;
    Pair pair1, pair2;
    Node *node, *vertices_list;
    Vertex *vertex;

    TableInit(adj_list);
    TableInit(vertices);

    vertices_list = NULL;

    if ((n = scanf("%d", &N)) != 1)
        ERR_EXIT("scanf");
    for (i = 0; i < N; ++i) {
        if ((n = scanf("%d", &l)) != 1)
            ERR_EXIT("scanf");
        prev = -1;
        for (j = 0; j < l; ++j) {
            if ((n = scanf("%d", &x)) != 1)
                ERR_EXIT("scanf");
            fill_pair(&pair1, i + 1, x);
            if (TableGet(vertices, pair1) == NULL) {
                vertex = (Vertex *)malloc(sizeof(Vertex));
                vertex->key = pair1;
                TableSet(vertices, pair1, vertex);
                vertices_list = Push(vertices_list, pair1);
            }
            if (prev == -1) {
                prev = x;
                continue;
            }
            fill_pair(&pair1, i + 1, prev);
            fill_pair(&pair2, i + 1, x);
            node = (Node *)TableGet(adj_list, pair1);
            node = Push(node, pair2);
            TableSet(adj_list, pair1, node);
            prev = x;
        }
    }

    if (DFS(vertices_list)) {
        printf("Yes\n");
        node = topo_list;
        while (node != NULL) {
            printf("%d %d\n", (node->pair).i, (node->pair).j);
            node = node->next;
        }
    } else {
        printf("No\n");
    }

    if (topo_list != NULL)
        FreeList(topo_list);
    FreeTable(adj_list, FreeList);
    FreeTable(vertices, free);

    return 0;
}
