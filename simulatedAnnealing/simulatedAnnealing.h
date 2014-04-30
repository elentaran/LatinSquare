#include"fastLS.h"

#define NB_RESTARTS 1
#define nbOfCycles 200 
#define nbOfCyclesPerTrial 1000
float nbOfAcceptedSolutions = 0.0;

//#define SLOW
#define POINTS_PER_CURVE 100
#ifdef POINTS_PER_CURVE
    float limPPC=0;
    float incPPC=(nbOfCycles * nbOfCyclesPerTrial * (DIM+1))/POINTS_PER_CURVE;
#endif

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
