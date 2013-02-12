#include "population.h"


int main(int nbArgs, char* args[]) {

	srand(time(NULL));

    Pop myPop;
	myPop.evolve();
    myPop.print();
}
