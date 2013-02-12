#include<iostream>
#include<sstream>
#include<fstream>
#include<stdlib.h>
#include<assert.h>

#define DIM 6
#define LARGEUR 7
#define LEVEL 10

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

		//double poolravevalues[LARGEUR][DIM];

		LS();
		void add(short []);
		void genOneRandom(short []);
	//	void genOnePoolRave(short []);
		string toString();
		void print();
		void updateBest(); 
		void writeBest(string fileName ="");
		double readBest(string fileName ="");
		void playout();
		short computeDist(const short [], const short []);
		short computeMinDist();
		void nested(short);
};
