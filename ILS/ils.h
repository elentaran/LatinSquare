#include "fastLS.h"


//#define SAVEBEST
//

#ifndef NBEVAL
    #define NBEVAL 100000
#endif

class Ils{

    public:
    LS bestLS;
    double bestScore;

    Ils();
    void run();
    void runILS();
    void print();
    void updateBest(LS other);
    LS onePlusOne(LS other);
    LS localSearch(LS other); // localsearch corresponding to the ILS paper
    


};
