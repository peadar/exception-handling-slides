#include <setjmp.h>
#include <stdlib.h>
#include "raii-c.h"

jmp_buf buffer;
int g(int depth);

int h(int depth) {
    if (random() % 200 == 0)
        longjmp(buffer, 1);
    else if (depth < 10)
        g(depth + 1);
}

int g(int depth) {
    struct CleanMe here;
    h(depth);
    struct CleanMe there;
    h(depth);
}

int f() {
    if (setjmp(buffer) == 0) {
        g(0);
        printf("done\n");
    } else {
        fprintf(stderr, "error!\n");
        return 1;
    }
    return 0;
}

int main() { return f(); }
