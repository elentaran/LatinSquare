#include<iostream>
#include<sstream>
#include<fstream>
#include<stdlib.h>
#include<assert.h>
#include<cmath>
#include<cstring>
#include<vector>
#include<algorithm>

using namespace std;

#ifndef DIM
    #define DIM 4 
#endif
#ifndef LARGEUR
    #define LARGEUR 25 
#endif

#define RECCORD_REP "reccord/"

#define FAKESCORE
#define PHI         // FAKESCORE  must be defined
//#define SENDMAIL

#ifndef TEST
    #define TEST 10 
#endif

extern int nbEval;

class LS
{
    public:
        short hypercube[LARGEUR][DIM];
        short legals[LARGEUR][DIM];
        short nbLegals[DIM];
        short nbAdded;
        short score;
        double fakeScore;

        LS();
        LS clone();
        string toString();
        void print();
        void reinit();
        void add(short []);
        void genOneRandom(short []);
        void initDiagonal();
        void updateBest(); 
        void initRand();
        void writeBest(string fileName ="");
        double readBest(string fileName ="");
        short computeDist(const short [], const short []);
        short computeMinDist();
        double computeFakeScore();
        double computeScorePhi();
        double computeDistLS(LS other);
        void computeScores();
        void mutate();
        void mutate1();
        void mutate2();
        void mutate3();
        void mutate4();
        void mutateRand();
        void perturb(); // perturbation corresponding to the ILS paper

        void mutateStrong();
        short getARandomCriticalPoint();
        vector<short> getCriticalPoints();

};
