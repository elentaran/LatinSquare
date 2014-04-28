#include "fastLS.h"


#define LAMBDA 2000

#define STAGN 500
#define RESTART 5
#define SAVEBEST

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
