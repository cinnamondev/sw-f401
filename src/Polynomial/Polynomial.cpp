struct PolyTerm {
    int k;
    int e;
};
struct Polynomial {
    int order;
    PolyTerm* terms;
};