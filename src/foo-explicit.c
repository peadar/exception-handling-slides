#include <stdlib.h>
#include "raii-c.h"

extern void *exceptionThrown;
extern const char *exceptionType;
extern void cleanme_fini(struct CleanMe *);
extern void cleanme_init(struct CleanMe *);
extern void exception_init(struct CleanMe *);
struct exception {};

void h() {
    if (random()) {
        exceptionType = "std::exception";
        exceptionThrown = malloc(sizeof (struct exception));
        exception_init(exceptionThrown);
    }
}

void g() {
    struct CleanMe here;
    cleanme_init(&here);
    h();
    if (exceptionThrown)
        goto cleanup_here;

    struct CleanMe there;
    cleanme_init(&here);
    if (exceptionThrown)
        goto cleanup_there;
    h();
    if (exceptionThrown)
        goto cleanup_there;
    if (exceptionThrown) {
        goto cleanup_there;
    }

cleanup_there:
    cleanme_fini(&there);
cleanup_here:
    cleanme_fini(&here);
}

int f() {
    g();
    if (exceptionThrown) {
        exceptionThrown = NULL;
        fprintf(stderr, "caught exception\n");
        return 1;
    }
    return 0;
}

int main() { return f(); }
