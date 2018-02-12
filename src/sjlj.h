#include <setjmp.h>
#define MAX_CLEANUPS 1024

struct CleanupHandler {
    void *data;
    void (*handler)(void *);
};

struct ExceptionState {
    struct ExceptionState *next;
    struct CleanupHandler cleanup[MAX_CLEANUPS];
    jmp_buf jmpBuf;
    int cleanupCount;
    void *exception;
    const char *type;
};

jmp_buf *try(struct ExceptionState *entry);
void throw(void *data, const void *type);
void endTry();
void pushCleanup(void *data, void (*handler)(void *));
void popCleanup();
