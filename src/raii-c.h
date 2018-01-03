#include <stdio.h>
struct CleanMe {
};

static inline void cleanme_init(struct CleanMe *cm)
{
    fprintf(stderr, "init cleanme %p\n", (void *)cm);
}

static inline void cleanme_fini(struct CleanMe *cm)
{
    fprintf(stderr, "fini cleanme %p\n", (void *)cm);
}

static inline void cleanme_fini_ex(void *cm)
{
    fprintf(stderr, "fini cleanme (from exception) %p\n", (void *)cm);
}
