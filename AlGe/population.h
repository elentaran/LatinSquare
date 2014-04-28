#include "fastLS.h"

#ifndef LAMBDA
#define LAMBDA 200
#endif

#ifndef GEN
#define GEN 200*(DIM+1)
//#define GEN 500
#endif

#ifndef MUTATION
#define MUTATION 3
#endif

///#define STAGN 1000
//#define RESTART 5
//#define SAVEBEST

class Pop{

    public:
    LS population[LAMBDA*2];
    double bestScore;

    Pop();
    void evolve();
    void generate();
    void select();
    int part(int p, int r);
    void sortVector(int p, int r);
    void init();
    void perturb();
    void print();
    


};
