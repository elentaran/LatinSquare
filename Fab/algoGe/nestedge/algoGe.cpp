#include<iostream>
#include<sstream>
#include<fstream>
#include<stdlib.h>
#include<assert.h>

#define DIM 4
#define LARGEUR 22
#define LEVEL 6

#define RECCORD_REP "reccord/"
#define LAMBDA 1
#define GENERATIONS 20000


using namespace std;

class LS
{
	public:
		short hypercube[LARGEUR][DIM];
		short legals[LARGEUR][DIM];
                short nbLegals[DIM];
                short nbAdded;
		short score;
		short indicesHistory[LARGEUR][DIM];

		LS();
		string toString();
		void print();
		void reinit();
		void add(short []);
		void addCoup(short []);
                void genOneRandom(short []);
		void initDiagonal();
		void nested(short);
		void updateBest(); 
		void writeBest(string fileName ="");
		double readBest(string fileName ="");
		short computeDist(const short [], const short []);
		short computeMinDist();
};

LS population[LAMBDA*2];

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

void LS::reinit()
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

void LS::addCoup(short coup[])
{
	for (short i=0; i<DIM; i++)
	{
		if (coup[i] < 0)
		{
			cout << "problem" << endl;
			cout << coup[i] << endl;
		}
		assert(coup[i]>=0);
		// On joue une tour en coup 
		hypercube[nbAdded][i]=coup[i];

		// mise à jour de legals
		for (short j=0; j<=nbLegals[i]; j++)
		{
			if (legals[j][i] == coup[i])
			{
				legals[j][i]=legals[nbLegals[i]][i];
				legals[nbLegals[i]][i]=-1;
				nbLegals[i]--;
			}
		}
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

void LS::initDiagonal()
{
	for (short i=0; i<LARGEUR; i++)
		for (short j=0; j<DIM; j++)
			hypercube[i][j]=i;
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
	cout << endl << "Score : "<<score<<endl;
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

	ofstream myfile;
	myfile.open(fileName.c_str());
	if (myfile.is_open())
	{
		myfile << score << endl;
		myfile << toString() << endl;
		myfile.close();
	}
	else
	{
		cerr << "could not white the file: "<<fileName.c_str() << endl;
	}
}

// load the result from the file or return 0 if no file
double LS::readBest(string fileName /*=""*/) {
	if (fileName == "") {
		stringstream res;
		res << RECCORD_REP << DIM << "_" << LARGEUR << ".best";
		fileName = res.str();
	}

	short res=0;
	ifstream myfile;
	myfile.open(fileName.c_str());
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
	for (int i=0; i<(LARGEUR-1); i++)
	{
		for (int j=i+1; j<LARGEUR; j++)
		{
			curDist = computeDist(hypercube[i],hypercube[j]);
			if (curDist < minDist)
				minDist = curDist;
		}
	}

	return minDist;
}

void initPopulation()
{
        short indiceCoup[DIM];
	for (short p=0; p<LAMBDA; p++)
	{
		for (short i=0; i<LARGEUR; i++)
		{
			population[p].genOneRandom(indiceCoup);
			population[p].add(indiceCoup);
		}
		population[p].score = population[p].computeMinDist();
	}
	for (short p=LAMBDA; p<LAMBDA*2; p++)
		population[p].initDiagonal();
}

void printingPopulationScores()
{
	for (short p=0; p<LAMBDA; p++)
		cout<<population[p].score<<" ";
	cout<<" ---- ";
	for (short p=LAMBDA; p<LAMBDA*2; p++)
		cout<<population[p].score<<" ";
	cout<<endl;
}

void mutate(LS &ind1, short nbAdded)
{
	for (short i=0; i<DIM; i++)
	{
		short probaMutationDim = (short)((rand () / (RAND_MAX + 1.0)) * (DIM));

		//if ((probaMutationDim==i)||(probaMutationDim==i+1)||(probaMutationDim==i-1)||(probaMutationDim==i+2)||(probaMutationDim==i-2))
		//if ((probaMutationDim==i+1)||(probaMutationDim==i-1))
		if ((probaMutationDim==i)||(probaMutationDim==i+1)||(probaMutationDim==i-1))
		{
			short valeur1 = nbAdded+(short)((rand () / (RAND_MAX + 1.0)) * (LARGEUR-nbAdded));
			short valeur2 = nbAdded+(short)((rand () / (RAND_MAX + 1.0)) * (LARGEUR-nbAdded));

			short temp;
			temp = ind1.hypercube[valeur1][probaMutationDim];
			ind1.hypercube[valeur1][probaMutationDim] = ind1.hypercube[valeur2][probaMutationDim];
			ind1.hypercube[valeur2][probaMutationDim] = temp;

			ind1.score = ind1.computeMinDist();
		}
	}
}

void generate(short nbAdded)
{
	short j=0;
	short compteur=1;
	for (short i=0; i<LAMBDA; i++)
	{
		if ((LAMBDA==1)||(i<LAMBDA/8))
		{
			population[i+LAMBDA]=population[0];
			mutate(population[i+LAMBDA], nbAdded);
		}
		else
		{
			if (i<1*(LAMBDA/4))
			{
			//	population[i+LAMBDA]=population[j++];
				population[i+LAMBDA]=population[1];
				mutate(population[i+LAMBDA], nbAdded);
		//		if ((compteur++%10)==0) 
		//		{
		//			j++;
		//			compteur=1;
		//		}
			}
			else
			{
				population[i+LAMBDA].reinit();
				short indiceCoup[DIM];
				for (short j=0; j<LARGEUR; j++)
				{
					population[i+LAMBDA].genOneRandom(indiceCoup);
					population[i+LAMBDA].add(indiceCoup);
				}
				population[i+LAMBDA].score = population[i+LAMBDA].computeMinDist();

			}
		}
	}
}

int part(int p, int r)
{
	short compt=p;
	short pivot=population[p].score;
	int i;

	for (i=p+1; i<=r; i++)
	{
		if ( population[i].score > pivot )
		{
			compt++;
			LS temp;
			temp = population[i];
			population[i] = population[compt];
			population[compt] = temp;
		}
	}
	LS temp;
	temp = population[p];
	population[p] = population[compt];
	population[compt] = temp;
	return compt;
}

void sortVector(int p, int r)
{
	int q;
	if (p<r)
	{
		q = part(p, r);
		sortVector(p, q-1);
		sortVector(q+1, r);
	}
}

void selection()
{
#if (LAMBA==1)
	if (population[0].score>population[1].score)
	{
		LS temp = population[0];
		population[0]=population[1];
		population[1]=temp;
	}
#else
//#define COMMA
#ifdef COMMA
	sortVector(LAMBDA, LAMBDA*2-1);
	for (short i=0; i<LAMBDA; i++)
		population[i]=population[i+LAMBDA];
#else
	sortVector(0, LAMBDA*2-1);
#endif
#endif
}

void evolve(short nbAdded)
{
	//for (short restart=0; restart<1000; restart++)
	//{
	//	cerr<<"Restart"<<population[0].score<<endl;
		for (short l=0; l<LAMBDA; l++)
			population[l].reinit();
		initPopulation();
		int generation=0;
	//	short previous = population[0].score;
	//	short stagne = 0;
	//	while (stagne<1000)
		while (generation<GENERATIONS)
		{
	//		if (generation%int(GENERATIONS/100)==0)
	//		{
	//			cout<<generation/int(GENERATIONS/100)<<"% ("<<population[0].score<<")"<<endl;
	//		}
			generate(nbAdded);
			selection();
			population[0].updateBest();
			generation++;
	//		if (population[0].score==previous)
	//		{
	//			stagne++;
	//		}
	//		else
	//		{
	//			previous=population[0].score;
	//			stagne=0;
	//		}
		}
	//}
}

short scoreBestRollout[LEVEL];
short bestRollout[LEVEL][LARGEUR][DIM];

void LS::nested(short level)
{
        scoreBestRollout[level] = -1000;
        while (nbAdded<LARGEUR)
	{
		if (level >= 2)
		{
			for (short ll=2; ll<level; ll++)
				cout<<'\t';
			cout << "Lvl : "<<level<<", Current : " << nbAdded << endl;
		}

		for (int curPlayout=0; curPlayout<1; curPlayout++)
		{
			LS currentLS = *this;
			if (level == 1)
			{
				evolve(nbAdded);
				currentLS=population[0];
				if (currentLS.score > scoreBestRollout[level])
				{
					scoreBestRollout[level]=currentLS.score;
					for (int i=0; i<LARGEUR; i++)
						for (int j=0; j<DIM; j++)
							bestRollout[level][i][j]=currentLS.hypercube[i][j];
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
							bestRollout[level][i][j]=currentLS.hypercube[i][j];
				}
			}
		}
		addCoup(bestRollout[level][nbAdded]);
	}
        score=scoreBestRollout[level];
}

int main()
{
	srand(time(NULL));

	LS test;
	test.nested(5);
	//cout<<"Best Solutions : "<<endl;
	//for (short i=0; i<10; i++)
	//	population[i].print();
	printingPopulationScores();
}
