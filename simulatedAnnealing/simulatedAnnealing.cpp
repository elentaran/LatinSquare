#include"simulatedAnnealing.h"

int nbEval=0;

SimulatedAnnealing::SimulatedAnnealing() {
	solution.initRand();
}

SimulatedAnnealing::SimulatedAnnealing(LS sol) {
	solution = sol;
}

float SimulatedAnnealing::getEnergy() {
	//return solution.score;
#ifdef FAKESCORE
	return solution.fakeScore;
#else
    return solution.score;
#endif
}

void SimulatedAnnealing::moveOperator() {
	//solution.mutate1();
    solution.mutate3();
 /*	short lamut = rand()%4;
	if (lamut==0)
 		solution.mutate1();
 	else if (lamut==1)
 		solution.mutate2();
 	else if (lamut==2)
 		solution.mutate3();
 	else if (lamut==3)
 		solution.mutate4();*/
}

void SimulatedAnnealing::print() {
	solution.print();
}

SimulatedAnnealing SimulatedAnnealing::clone() {
	SimulatedAnnealing sa(this->solution.clone());
	return sa;
}

void SimulatedAnnealing::perform(int indiceRestart, float probaStart, float probaEnd) {
	float tStart = (-1./log(probaStart));
	float tEnd = (-1./log(probaEnd));
	float temp = (tEnd/tStart);
	float temp2 = (1. / (nbOfCycles - 1.));
	float fractionalReduction =  pow(temp, temp2);

	// initialization
	SimulatedAnnealing xi = this->clone();
	float na = nbOfAcceptedSolutions+1;

	// curret best result 
	SimulatedAnnealing xc = this->clone();
	SimulatedAnnealing xbest = this->clone();
	float fc = this->getEnergy();

	// current temperature
	float t = tStart;

	// deltaE average
	float deltaEAvg = 0.;
	float deltaE = 0.;
	float p = 0.;
	bool accept;

#ifdef SLOW
	xi.solution.updateBest();
	xc.solution.updateBest();
#endif

	for (int i=0; i<nbOfCycles; i++) {
		//printf("Cycle: %d with temperature: %f and energy: %f (and score %f) (best: %f) \n",i,t,fc, (float)xc.solution.score, (float)xbest.solution.score);
		for (int j=0; j<nbOfCyclesPerTrial; j++) {
			// generate a new trial
			xi = xc.clone();
			xi.moveOperator();
#ifdef SLOW
			xi.solution.updateBest();
#endif
			if (xi.solution.score>xbest.solution.score)
				xbest=xi.clone();

#ifdef POINTS_PER_CURVE
            if (nbEval > limPPC) {
                cout << "nb eval: " << nbEval << endl;
                cout << "Score: " << xbest.solution.score << endl;
                limPPC += incPPC;
            }
#endif
			deltaE = abs(xi.getEnergy() - fc);

#ifdef FAKESCORE
#ifdef PHI
			if (xi.getEnergy() > fc) {
#else
			if (xi.getEnergy() < fc) {
#endif
#else
			if (xi.getEnergy() < fc) {
#endif
				// a worse solution is found
				// initialize deltaEAvg if first iteration
				if (i==0 && j==0)
					deltaEAvg = deltaE; 

				// generate probability of acceptance
				p = exp(-deltaE/(deltaEAvg*t));

				// acceptance or not
				float proba = rand()/(RAND_MAX+1.);
				if (proba<p) {
					accept=true;
				}
				else {
					accept=false;
				}
			}
			else {
				// better solution
				accept = true;
			}
			if (accept) {
				// update current solution
				xc = xi.clone();
				fc = xc.getEnergy();

				// increment tje number of accepted solutions
				na++;

				// update deltaEAvg
				deltaEAvg = ((deltaEAvg * (na-1) + deltaE) / na);
			}
		}
		//	printf("DeltaE : %f deltaEAVG: %f temperature: %f proba: %f \n",deltaE,deltaEAvg,t,p);

		t = fractionalReduction * t;
	}
	//xbest.print();
}

void SimulatedAnnealing::performWithRestart() {
	for (int i=0; i<NB_RESTARTS; i++) {
	//	float ratio = (i/(NB_RESTARTS))/2.;
		float ratio = 0.;
		perform(i, 0.3-ratio, 0.1-ratio);
	}
}

int main() {
	srand(time(NULL));
	SimulatedAnnealing sa;

	sa.performWithRestart();
    cout << "nbEval: " << nbEval << endl;
    cout << "test:" << incPPC << endl;
//	
//	LS test;
//	cout<<"Res : "<<test.checkeur("caca")<<endl;
}
