#include"fastLS.h"

using namespace std;

LS::LS()
{
	score=-1;
	nbAdded=0;
	for (short i=0; i<DIM; i++)
	{
		for (short j=0; j<LARGEUR; j++)
		{
			hypercube[j][i]=-1;
			legals[j][i]=j;

		}
		nbLegals[i]=LARGEUR-1;
	}
}

void LS::add(short indiceCoup[])
{
	for (short i=0; i<DIM; i++)
	{
		// On joue une tour en coup 
		hypercube[nbAdded][i]=legals[indiceCoup[i]][i];

		// on met a jour indicesHistory
		indicesHistory[nbAdded][i]=indiceCoup[i];

		// mise à jour de legals
		legals[indiceCoup[i]][i]=legals[nbLegals[i]][i]; 
		legals[nbLegals[i]][i]=0;
		nbLegals[i]--;
	}
	nbAdded++;
}

void LS::genOneRandom(short indiceCoup[])
{
	short monrand;

	for (short i=0; i<DIM; i++)
	{
		monrand=(short)((rand () / (RAND_MAX + 1.0)) * (nbLegals[i]+1));
		indiceCoup[i]=monrand;
		if (monrand<0) cerr<<"nbL "<<nbLegals[i]<<" monrand : "<<monrand<<" et nbAdded "<<nbAdded<<endl;
		assert(monrand>=0);
		assert(monrand<LARGEUR);
	}
}
//
//void LS::genOnePoolRave(short indiceCoup[])
//{
//	short monrand;
//
//	double somme;
//	double tintouin[LARGEUR];
//
//	for (short i=0; i<DIM; i++)
//	{
//		for (short j=0; j<nbLegals[j]; j++)
//		{
//			tintouin[]
//		}
//	}
//
//	double somme=tintouin[0];for (unsigned int i=1; i<tintouin.size(); i++) somme+=tintouin[i];
//	double monrand=(rand()/(double)RAND_MAX )*(somme);
//
//	int i=0;
//	double sommecourante=tintouin[0];
//	while (sommecourante<monrand)
//	{
//		i++;
//		if (i>=tintouin.size()) { fprintf(stderr," monrand=%f, somme=%f\n",monrand,somme);assert(0); exit(-1);}
//		sommecourante+=tintouin[i];
//	}
//
//
//
//
//
//
//	for (short i=0; i<DIM; i++)
//	{
//		monrand=(short)((rand () / (RAND_MAX + 1.0)) * (nbLegals[i]+1));
//		indiceCoup[i]=monrand;
//		if (monrand<0) cerr<<"nbL "<<nbLegals[i]<<" monrand : "<<monrand<<" et nbAdded "<<nbAdded<<endl;
//		assert(monrand>=0);
//		assert(monrand<LARGEUR);
//	}
//}

void LS::playout()
{
	short indiceCoup[DIM];
	for (short i=nbAdded; i<LARGEUR; i++)
	{
		genOneRandom(indiceCoup);
		//genOnePoolRave(indiceCoup);
		add(indiceCoup);
	}
	score=computeMinDist();
}


string LS::toString() {
	stringstream res;
	for (int i=0;i<LARGEUR;i++) {
		res << "[";
		for (int j=0;j<DIM;j++) {
			res << hypercube[i][j]; 
			if (j < (DIM-1))
				res << ",";
		}
		res << "] ";
	}
	return res.str();
}

void LS::print()
{
	cout<< "Printing a solution"<<endl;
	cout << toString();
	cout << endl << "Score : "<<computeMinDist()<<endl;
}

// compute the value of the current LS
// update the file containing the best value if there is a new reccord
void LS::updateBest() {
	double bestVal = readBest();
	if (score > bestVal) {
		cout << "new reccord!!: " << score << endl;
		writeBest();
	}
}

// save the current result in a file
// used to track the current best
void LS::writeBest(string fileName /*=""*/) {
	if (fileName == "") {
		stringstream res;
		res << RECCORD_REP << DIM << "_" << LARGEUR << ".best";
		fileName = res.str();
	}

	ofstream myfile(fileName.c_str());
	myfile << score << endl;
	myfile << toString() << endl;
	myfile.close();

}

// load the result from the file or return 0 if no file
double LS::readBest(string fileName /*=""*/) {
	if (fileName == "") {
		stringstream res;
		res << RECCORD_REP << DIM << "_" << LARGEUR << ".best";
		fileName = res.str();
	}

	short res=0;
	ifstream myfile(fileName.c_str());
	if (myfile.is_open())
	{
		myfile >> res;
		myfile.close();
	} else {
		cerr << "could not read the file" << endl;
	}

	return res;
}

short LS::computeDist(const short point1[], const short point2[])
{
	short res=(point1[0]-point2[0])*(point1[0]-point2[0]);
	for (int i=1; i<DIM; i++)
		res+=short(point1[i]-point2[i])*(point1[i]-point2[i]);
	return res;
}

short LS::computeMinDist()
{
	short curDist;
	short minDist = 10000;
	for (int i=0; i<(nbAdded-1); i++)
	{
		for (int j=i+1; j<nbAdded; j++)
		{
			curDist = computeDist(hypercube[i],hypercube[j]);
			if (curDist < minDist)
				minDist = curDist;
		}
	}

	return minDist;
}

short scoreBestRollout[LEVEL];
short bestRollout[LEVEL][LARGEUR][DIM];

void LS::nested(short level)
{
	scoreBestRollout[level] = -1000;
	while (nbAdded<LARGEUR)
	{
		for (int curPlayout=0; curPlayout<100; curPlayout++) {
			LS currentLS = *this;
			if (level == 1)
			{
				currentLS.playout();
				if (currentLS.score > scoreBestRollout[level])
				{
					scoreBestRollout[level]=currentLS.score;
					for (int i=0; i<LARGEUR; i++)
						for (int j=0; j<DIM; j++)
							bestRollout[level][i][j]=currentLS.indicesHistory[i][j];
				}
			}
			else
			{
				currentLS.nested(level-1);
				if (currentLS.score > scoreBestRollout[level])
				{
					scoreBestRollout[level]=currentLS.score;
					for (int i=0; i<LARGEUR; i++)
						for (int j=0; j<DIM; j++)
							bestRollout[level][i][j]=currentLS.indicesHistory[i][j];
				}
			}
		}
		add(bestRollout[level][nbAdded]);
	}
	score=scoreBestRollout[level];
}

int main()
{
	srand(time(NULL));
	LS ls;
	ls.nested(4);
	ls.print();
	ls.updateBest();
}
