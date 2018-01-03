#include "sjlj.h"
struct ExceptionState *stack;

jmp_buf *try(struct ExceptionState *entry) {
    entry->cleanupCount = 0;
    entry->next = stack;
    stack = entry;
    return &entry->jmpBuf;
}

void throw(void *data, const void *type) {
    stack->exception = data;
    stack->type = type;
    longjmp(stack->jmpBuf, 1);
}

void endTry() {
    stack = stack->next;
}

void catch() {
    while (stack->cleanupCount-- != 0) {
        struct CleanupHandler *h = &stack->cleanup[stack->cleanupCount];
        h->handler(h->data);
    }
    endTry();
}

void pushCleanup(void *data, void (*handler)(void *)) {
    struct CleanupHandler *h =  &stack->cleanup[stack->cleanupCount++];
    h->handler = handler;
    h->data = data;
}

void popCleanup() {
    stack->cleanupCount--;
}
