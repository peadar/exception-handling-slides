struct CleanMe {
    CleanMe() { std::clog << "construct cleanme@" << this << std::endl; }
    ~CleanMe() { std::clog << "destruct cleanme@" << this << std::endl; }
};
