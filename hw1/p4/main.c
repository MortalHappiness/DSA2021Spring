/* References:
 * 1. https://www.geeksforgeeks.org/expression-evaluation/
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1000000

#define ERR_EXIT(msg) \
    do {              \
        perror(msg);  \
        exit(1);      \
    } while (0)

// ========================================
// Stack implementation

typedef union {
    char char_val;
    double double_val;
} Item;

typedef struct {
    Item array[MAX_LINE];
    size_t size;
} Stack;
Stack values, ops;

void stack_init(Stack *stack) { stack->size = 0; }

void stack_push(Stack *stack, Item item) {
    if (stack->size == MAX_LINE)
        ERR_EXIT("Push to full stack\n");
    stack->array[stack->size] = item;
    ++(stack->size);
}

Item stack_pop(Stack *stack) {
    if (stack->size == 0)
        ERR_EXIT("Pop from empty stack\n");
    --(stack->size);
    return stack->array[stack->size];
}

Item stack_gettop(Stack *stack) {
    if (stack->size == 0)
        ERR_EXIT("Get from empty stack\n");
    return stack->array[stack->size - 1];
}

size_t stack_size(Stack *stack) { return stack->size; }

// ========================================
// Global variables

char buf[MAX_LINE + 2]; // MAX_LINE + '\n' + '\0'
Stack values, ops;

// ========================================

int precedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}

double apply_op(double a, double b, char op) {
    switch (op) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
        default:
            ERR_EXIT("Invalid operator\n");
    }
}

bool is_digit(char ch) { return ch >= '0' && ch <= '9'; }

void evaluate(const char *expression, size_t len) {
    stack_init(&values);
    stack_init(&ops);
    Item item;
    double a, b;
    char op;
    int i = 0, val;

    while (i < len) {
        if (expression[i] == '(') {
            item.char_val = '(';
            stack_push(&ops, item);
        } else if (is_digit(expression[i])) {
            val = 0;
            while (i < len && is_digit(expression[i])) {
                val = 10 * val + (expression[i] - '0');
                ++i;
            }
            item.double_val = (double)val;
            stack_push(&values, item);
            --i;
        } else if (expression[i] == ')') {
            while (stack_size(&ops) != 0 &&
                   stack_gettop(&ops).char_val != '(') {
                b = stack_pop(&values).double_val;
                a = stack_pop(&values).double_val;
                op = stack_pop(&ops).char_val;
                item.double_val = apply_op(a, b, op);
                stack_push(&values, item);
            }
            stack_pop(&ops);
        } else {
            while (stack_size(&ops) != 0 &&
                   precedence(stack_gettop(&ops).char_val) >=
                       precedence(expression[i])) {
                b = stack_pop(&values).double_val;
                a = stack_pop(&values).double_val;
                op = stack_pop(&ops).char_val;
                item.double_val = apply_op(a, b, op);
                stack_push(&values, item);
            }
            item.char_val = expression[i];
            stack_push(&ops, item);
        }
        ++i;
    }

    while (stack_size(&ops) != 0) {
        b = stack_pop(&values).double_val;
        a = stack_pop(&values).double_val;
        op = stack_pop(&ops).char_val;
        item.double_val = apply_op(a, b, op);
        stack_push(&values, item);
    }

    printf("%.15lf\n", stack_gettop(&values).double_val);
}

int main(int argc, char *argv[]) {
    char buf[MAX_LINE + 2]; // MAX_LINE + '\n' + '\0'
    size_t len;
    while (fgets(buf, MAX_LINE + 2, stdin) != NULL) {
        len = strlen(buf);
        if (buf[len - 1] == '\n') {
            buf[len - 1] = '\0';
            len -= 1;
        }
        evaluate(buf, len);
    }
    return 0;
}
