#include "ils.h"
#include <unistd.h>


int nbRun=100000;
int nbEval=0;
int probaKeep=5;

void usage() {
    cerr << "latinSquare -nbRun 1000" << endl;
    exit(0);
}

int main(int nbArgs, char* args[]) {


    for (int i = 1; i < nbArgs; i++) {
        if (!strcmp (args [i], "-nbRun")) {
            if (sscanf (args [++i], "%i", & nbRun) != 1)
                usage();
        }else if (!strcmp (args [i], "-probaKeep")) {
            if (sscanf (args [++i], "%i", & probaKeep) != 1)
                usage();
        }
        else {
            cerr << "argument non reconnu: " << args[i] << endl;
            usage();
        }
    }

	srand(time(NULL));

    Ils myILS;
    myILS.runILS();
    myILS.print();
    cout << "nb eval: " << nbEval << endl;
}

