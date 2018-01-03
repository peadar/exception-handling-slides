#include <iostream>
#include "raiitype.h"

int i(int a) {
    CleanMe here;
    CleanMe too;
    if (a != 100) {
        throw 9;
    }
    return a + 1;
}


int h(int a) {
    CleanMe here;
    CleanMe too;
    return i(a + 1);
}


int g(int a) {
    CleanMe here;
    CleanMe too;
    return h(a + 1);
}


int f(int a) {
    try {
        return g(a + 1);
    }
    catch (...) {
        std::clog << "f caught it " << std::endl;
    }
    return -1;
}

int
main()
{
    return f(1);
}
