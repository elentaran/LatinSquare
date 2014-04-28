#include "population.h"


float percentBest=0.;
float percentGood=1.;

int nbEval=0;

void usage() {
    cerr << "latinSquare -pBest 0.5 -pGood 0.5 -GEN 1000" << endl;
    exit(0);
}

int main(int nbArgs, char* args[]) {


    for (int i = 1; i < nbArgs; i++) {
        if (!strcmp (args [i], "-pBest")) {
            if (sscanf (args [++i], "%f", & percentBest) != 1)
                usage();
        }
        else if (!strcmp (args [i], "-pGood")) {
            if (sscanf (args [++i], "%f", & percentGood) != 1)
                usage();
        }
        else {
            cerr << "argument non reconnu: " << args[i] << endl;
            usage();
        }
    }

	srand(time(NULL));
    /*LS* temp = new LS();
    temp->initRand();
    temp->print();
    temp->computeScorePhi();
    exit(0);*/
    Pop myPop;
	myPop.evolve();
    myPop.print();
    cout << "nb eval: " << nbEval << endl;
}

