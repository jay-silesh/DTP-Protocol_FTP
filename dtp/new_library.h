struct ltid {
    bool operator() (unsigned int t1, unsigned int t2) const {
        return (t1 < t2);
    }
};
