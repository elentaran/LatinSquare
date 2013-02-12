#include "fastLS.h"

#define LAMBDA 10000
#define GENERATIONS 10000

class Pop{

    public:
    LS population[LAMBDA*2];

    Pop();
    void evolve();
    void generate();
    void select();
    int part(int p, int r);
    void sortVector(int p, int r);
    void init();
    void print();
    


};
