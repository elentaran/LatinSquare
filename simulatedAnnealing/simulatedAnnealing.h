#include"fastLS.h"

#define NB_RESTARTS 1
#define nbOfCycles 25
#define nbOfCyclesPerTrial 400
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
