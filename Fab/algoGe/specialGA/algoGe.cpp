#include<iostream>
#include<sstream>
#include<fstream>
#include<stdlib.h>
#include<assert.h>

#define DIM 4
#define LARGEUR 11
#define LEVEL 5

#define RECCORD_REP "reccord/"
#define LAMBDA 2*DIM*(LARGEUR-1)*((LARGEUR+1)/2)
#define GENERATIONS 400000


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
LS parent;
LS best;

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

void initParent()
{
	short indiceCoup[DIM];
	for (short i=0; i<LARGEUR; i++)
	{
		parent.genOneRandom(indiceCoup);
		parent.add(indiceCoup);
	}
	parent.score = parent.computeMinDist();
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
		//if (probaMutationDim==i)
		{
			short valeur1 = nbAdded+(short)((rand () / (RAND_MAX + 1.0)) * (LARGEUR-nbAdded));
			short valeur2 = nbAdded+(short)((rand () / (RAND_MAX + 1.0)) * (LARGEUR-nbAdded));
			short temp;
//#define MIROIR
#ifdef MIROIR
			if (valeur2<valeur1)
			{
				temp=valeur1;
				valeur1=valeur2;
				valeur2=temp;
			}
			short vm = (valeur2-valeur1+1)/2;

			for (short d=0; d<vm; d++)
			{
				temp = ind1.hypercube[valeur1+d][probaMutationDim];
				ind1.hypercube[valeur1+d][probaMutationDim] = ind1.hypercube[valeur2-d][probaMutationDim];
				ind1.hypercube[valeur2-d][probaMutationDim] = temp;
				ind1.score = ind1.computeMinDist();
			}
#else
			temp = ind1.hypercube[valeur1][probaMutationDim];
			ind1.hypercube[valeur1][probaMutationDim] = ind1.hypercube[valeur2][probaMutationDim];
			ind1.hypercube[valeur2][probaMutationDim] = temp;
			ind1.score = ind1.computeMinDist();
#endif
		}
	}
}

void generate(short nbAdded)
{
	short indInPop=0;
	for (short i=0; i<DIM; i++)
	{
		for (short j=0; j<LARGEUR-1; j++)
		{
			for (short k=j+1; k<LARGEUR; k++)
			{
				population[indInPop]=parent;
				short temp = population[indInPop].hypercube[j][i];
				population[indInPop].hypercube[j][i] = population[indInPop].hypercube[k][i];
				population[indInPop].hypercube[k][i] = temp;
				population[indInPop].score = population[indInPop].computeMinDist();
				indInPop++;
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
#define SPECIALE
#ifdef SPECIALE
	sortVector(0, LAMBDA-1);
	parent=population[0];
#else
#if (LAMBA==1)
	if (population[0].score>population[1].score)
	{
		LS temp = population[0];
		population[0]=population[1];
		population[1]=temp;
	}
#else
#define COMMA
#ifdef COMMA
	sortVector(LAMBDA, LAMBDA*2-1);
	for (short i=0; i<LAMBDA; i++)
		population[i]=population[i+LAMBDA];
#else
	sortVector(0, LAMBDA*2-1);
#endif
#endif
#endif
}

void evolve(short nbAdded)
{
	for (short l=0; l<LAMBDA; l++)
		population[l].reinit();
	parent.reinit();
	best=parent;
	initParent();
	int generation=0;
	while (generation<GENERATIONS)
	{
		if (generation%int(GENERATIONS/100)==0)
		{
			cout<<generation/int(GENERATIONS/100)<<"% ("<<parent.score<<" --- "<<population[0].score<<" ; best : "<<best.score<<")"<<endl;
		//	cout<<generation<<" ("<<parent.score<<" --- "<<population[0].score<<")"<<endl;
		}
		generate(nbAdded);
		/**** selection part ****/
		sortVector(0, LAMBDA-1);
		if (parent.score>=population[0].score)
		{
			parent.reinit();
			initParent();
		}
		else
		{
		//	parent.print();
		//	population[0].print();
			parent=population[0];
			if (parent.score>best.score) best=parent;
			parent.updateBest();
		}
		generation++;
	}
}

short scoreBestRollout[LEVEL];
short bestRollout[LEVEL][LARGEUR][DIM];

void LS::nested(short level)
{
	scoreBestRollout[level] = -1000;
	while (nbAdded<LARGEUR)
	{
		if (level == 2)
			cout << "Current : " << nbAdded << endl;

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

	evolve(0);
	cout<<"Best Solutions : "<<endl;
	for (short i=0; i<10; i++)
		population[i].print();
	printingPopulationScores();
}
