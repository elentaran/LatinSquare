#include<iostream>
#include<sstream>
#include<fstream>
#include<stdlib.h>
#include<assert.h>

//#define ONLY_ALGOGE 

#define DIM 4
#define LARGEUR 22
#define LEVEL 10

#define LAMBDA 2
#define GENERATIONS 100

#define NB_TRY_GENMOVE 5

#define RECCORD_REP "reccord/"


using namespace std;

class LS
{
	public:
		short hypercube[LARGEUR][DIM];
		short indicesHistory[LARGEUR][DIM];
		short legals[LARGEUR][DIM];

		short nbLegals[DIM];
		short nbAdded;

		short score;
        short max_score;
		
		LS();
        void reinit();
        void initDiagonal();
		void add(short []);
        void addCoup(short []);
		void genOneRandom(short []);
		void getNextCoup(short [], int, int, int);
        string toString();
		void print();
        void updateBest(); 
        void writeBest(string fileName ="");
        double readBest(string fileName ="");
        void playout(short);
		short computeDist(const short [], const short []);
		short computeMinDist();
        short computeMinDist(short []);
		void nested(short,bool);
        void getCoup(short [], short []);
};
