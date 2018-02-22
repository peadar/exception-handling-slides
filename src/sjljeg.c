#include "foo-c.h"
void g(int depth);
void h(int depth) {
    if (random() % 50 == 0)
        throw("error here", "string");
    else if (depth < 40)
        g(depth + 1);
}
void g(int depth) {
    struct CleanMe here;
    cleanme_init(&here);
    pushCleanup(&here, cleanme_fini_ex);
    h(depth);
    struct CleanMe there;
    cleanme_init(&there);
    pushCleanup(&there, cleanme_fini_ex);
    h(depth);
    popCleanup();
    cleanme_fini(&there);
    popCleanup();
    cleanme_fini(&here);
}
int f() {
    struct ExceptionState ex;
    if (!setjmp(*try(&ex))) {
        g(0);
        endTry();
    } else {
        fprintf(stderr, "caught %s\n", ex.exception);
        return 1;
    }
    return 0;
}
int main() { return f(); }
