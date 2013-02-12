#include "fastLS.h"



LS::LS()
{
    reinit();
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

void LS::mutate()
{
	for (short i=0; i<DIM; i++)
	{
		short probaMutationDim = (short)((rand () / (RAND_MAX + 1.0)) * (DIM));

		//if ((probaMutationDim==i)||(probaMutationDim==i+1)||(probaMutationDim==i-1)||(probaMutationDim==i+2)||(probaMutationDim==i-2))
		//if ((probaMutationDim==i+1)||(probaMutationDim==i-1))
		if ((probaMutationDim==i)||(probaMutationDim==i+1)||(probaMutationDim==i-1))
		//if (probaMutationDim==i)
		{
			short valeur1 = (short)((rand () / (RAND_MAX + 1.0)) * LARGEUR);
			short valeur2 = (short)((rand () / (RAND_MAX + 1.0)) * LARGEUR);
			short temp;
			temp = hypercube[valeur1][probaMutationDim];
			hypercube[valeur1][probaMutationDim] = hypercube[valeur2][probaMutationDim];
			hypercube[valeur2][probaMutationDim] = temp;
			score = computeMinDist();
		}
	}
}

