#include<iostream>
#include<sstream>
#include<fstream>
#include<stdlib.h>
#include<assert.h>

using namespace std;

#define DIM 4
#define LARGEUR 22
#define LEVEL 5

#define RECCORD_REP "reccord/"

class LS
{
    public:
        short hypercube[LARGEUR][DIM];
        short legals[LARGEUR][DIM];
        short nbLegals[DIM];
        short nbAdded;
        short score;

        LS();
        string toString();
        void print();
        void reinit();
        void add(short []);
        void genOneRandom(short []);
        void initDiagonal();
        void updateBest(); 
        void writeBest(string fileName ="");
        double readBest(string fileName ="");
        short computeDist(const short [], const short []);
        short computeMinDist();
        void mutate();
};
