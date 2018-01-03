#include "sjlj.h"
#include "raii-c.h"
#include <stdlib.h>
void g(int depth);
void h(int depth) {
    if (random() % 100 == 0)
        throw("error here", "string");
    else if (depth < 10)
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
        catch();
        fprintf(stderr, "caught exception of type %s (%s)\n",
                ex.type, (const char *)ex.exception);
        return 1;
    }
    return 0;
}
int main() { return f(); }
