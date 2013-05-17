#include<iostream>
#include<sstream>
#include<fstream>
#include<stdlib.h>
#include<assert.h>
#include<cmath>

using namespace std;

#define DIM 4
#define LARGEUR 22

#define RECCORD_REP "reccord/"

#define FAKESCORE


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
        void mutate();
};
