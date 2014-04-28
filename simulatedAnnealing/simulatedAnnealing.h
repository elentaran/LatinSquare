#include"fastLS.h"

#define NB_RESTARTS 1
#define nbOfCycles 250
#define nbOfCyclesPerTrial 4000
float nbOfAcceptedSolutions = 0.0;

//#define SLOW

class SimulatedAnnealing {
	private:
		LS solution;

	public:
		SimulatedAnnealing();
		SimulatedAnnealing(LS sol);
		void moveOperator();
		float getEnergy();
		void print();
		SimulatedAnnealing clone();
		void perform(int, float, float);
		void performWithRestart();
};
