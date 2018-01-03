int f() {
    try {
        g();
    }
    catch (...) {
        std::clog << "caught exception\n";
        return 1;
    }
    return 0;
}

