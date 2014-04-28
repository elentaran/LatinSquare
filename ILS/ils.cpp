#include "ils.h"

extern float percentBest;
extern float percentGood;
extern int nbRun;
extern int probaKeep;


Ils::Ils() {
    bestLS = LS();
    bestLS.initRand();
    bestScore = bestLS.score;
}

LS Ils::onePlusOne(LS other) {
    LS currentLS = other;
    LS bestLS = other;
    LS newLS;
    for (int i=0; i<nbRun; i++) {
        newLS = currentLS;
        newLS.mutate3();
        if (newLS.score > bestLS.score) {
            bestLS = newLS;
        }
        if (newLS.fakeScore < currentLS.fakeScore || rand()%1000<probaKeep) {
            currentLS = newLS;
        }

    }
    return bestLS;
}

LS Ils::localSearch(LS other) { // localsearch corresponding to the ILS paper
    bool improved = true;

    LS currentLS = other;
    LS newLS;
    int temp;
    vector<short> critPoints = currentLS.getCriticalPoints();
    while (improved) {
        improved = false;
        for (int i=0; i<LARGEUR; i++) {
            for (int j=0; j<LARGEUR; j++) {
                //if (i!=j && (find(critPoints.begin(),critPoints.end(),i)!=critPoints.end() || find(critPoints.begin(),critPoints.end(),j) != critPoints.end())) {
                if (i!=j) {
                    for (int k=0; k<DIM; k++) {
                        newLS = currentLS;
                        temp = newLS.hypercube[i][k];
                        newLS.hypercube[i][k] = newLS.hypercube[j][k];
                        newLS.hypercube[j][k] = temp;
                        newLS.computeScores();
#ifdef FAKESCORE
#ifdef PHI 
                        if (newLS.fakeScore < currentLS.fakeScore) {
#else
                        if (newLS.fakeScore > currentLS.fakeScore) {
#endif
#else
                        if (newLS.score > currentLS.score) {
#endif
                            currentLS = newLS;
                            critPoints = currentLS.getCriticalPoints();
                            improved = true;
                        }
                        if (nbEval >= NBEVAL) 
                            return currentLS;
                    }
                }
            }
        }
    }
    return currentLS;
}


void Ils::run() {

    LS currentLS;
    currentLS.initRand();
    LS newLS;
    for (int i=0;i<10;i++) {
        newLS.initRand();
        newLS = onePlusOne(newLS);
        updateBest(newLS);
        //if (newLS.fakeScore < currentLS.fakeScore) {
        if (newLS.score > currentLS.score) {
            currentLS = newLS;
        }
    }
}

void Ils::runILS() {

    LS currentLS;
    currentLS.initRand();
    LS newLS;
    //for (int i=0;i<100;i++) {
    while (true) {
        newLS = currentLS.clone();
        newLS.perturb();
        newLS = localSearch(newLS);
        updateBest(newLS);
        //if (newLS.fakeScore < currentLS.fakeScore) {
        if (newLS.score > currentLS.score) {
            currentLS = newLS;
        }
        if (nbEval >= NBEVAL) 
            return;
        //if (i%10 == 0)
        //cout << "best: " << currentLS.score << endl;
    }
}

void Ils::updateBest(LS other) {
    if (other.score>bestLS.score) {
        bestLS=other;
        bestScore=bestLS.score;
    }
}

void Ils::print() {
    cout<<"best Score: " << bestScore<<endl;
}

