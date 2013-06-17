#include "fastLS.h"


#define LAMBDA 200

//#define STAGN
//#define RESTART

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
