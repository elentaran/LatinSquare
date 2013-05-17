#include "population.h"

double percentBest;
double percentGood;

int main(int nbArgs, char* args[]) {

    if (nbArgs > 1)
        percentBest=atof(args[1]);
    else
        percentBest=0.5;
    if (nbArgs > 2)
        percentGood=atof(args[2]);
    else
        percentGood=0.5;
    cerr << "best: " << percentBest << endl;
    cerr << "good: " << percentGood << endl;
	srand(time(NULL));
    Pop myPop;
	myPop.evolve();
    myPop.print();
}
