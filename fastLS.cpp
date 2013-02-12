#include"fastLS.h"

using namespace std;

LS population[LAMBDA];

// test for git

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
}

void erasePopulation()
{
    short indiceCoup[DIM];
    for (short p=0; p<LAMBDA; p++)
    {
        for (short i=0; i<LARGEUR; i++)
        {
            population[p].add(0);
        }
        population[p].score = population[p].computeMinDist();
    }
}

void printingPopulationScores()
{
    for (short p=0; p<LAMBDA; p++)
        cout<<population[p].score<<" ";
    cout<<endl;
}

void mutate(LS &ind1, int nbFixed=0)
{
    for (short i=0; i<DIM; i++)
    {
        //short probaMutationDim = (short)((rand () / (RAND_MAX + 1.0)) * (DIM));
        double probaMutationDim = rand () / (RAND_MAX + 1.0);

        //    if ((probaMutationDim==i)||(probaMutationDim==i))
        //if (probaMutationDim < (2.0/DIM))
        if (probaMutationDim < 0.5)
        {
            short valeur1 = (short)(((rand () / (RAND_MAX + 1.0)) * (LARGEUR-nbFixed))+nbFixed);
            short valeur2 = (short)(((rand () / (RAND_MAX + 1.0)) * (LARGEUR-nbFixed))+nbFixed);

            short temp;
            temp = ind1.hypercube[valeur1][i];
            ind1.hypercube[valeur1][i] = ind1.hypercube[valeur2][i];
            ind1.hypercube[valeur2][i] = temp;

            ind1.score = ind1.computeMinDist();

        }
    }
}

void generate(int nbFixed=0)
{
    short j=0;
    short compteur=1;
    for (short i=0; i<(LAMBDA-1); i++)
    {
        population[1+i]=population[0];
        mutate(population[i+1], nbFixed);
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
    sortVector(0, (LAMBDA-1));
}

void evolve(int nbFixed=0, bool verbose=false)
{
    int generation=0;
    while (generation<GENERATIONS)
    {
        //  if (generation%int(GENERATIONS/100)==0 && verbose)
        //  {
        //      cout<<generation/int(GENERATIONS/100)<<"% ("<<population[0].score<<")"<<endl;
        //      population[0].updateBest();
        //  }
        generate(nbFixed);
        selection();
        generation++;
    }
    population[0].updateBest();
}


LS::LS()
{
    score=-1;
    max_score=10000;
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

void LS::initDiagonal()
{
    for (short i=0; i<LARGEUR; i++)
        for (short j=0; j<DIM; j++)
            hypercube[i][j]=i;
    score=computeMinDist();
}


void LS::add(short indiceCoup[])
{
    for (short i=0; i<DIM; i++)
    {
        assert(legals[indiceCoup[i]]>=0);
        // On joue une tour en coup 
        hypercube[nbAdded][i]=legals[indiceCoup[i]][i];

        // mise à jour de legals
        legals[indiceCoup[i]][i]=legals[nbLegals[i]][i]; 
        legals[nbLegals[i]][i]=-1;
        nbLegals[i]--;
    }
    nbAdded++;
}

void LS::addCoup(short coup[])
{
    for (short i=0; i<DIM; i++)
    {
        if (coup[i] < 0) {
            cout << "problem" << endl;
            cout << coup[i] << endl;
        }
        assert(coup[i]>=0);
        // On joue une tour en coup 
        hypercube[nbAdded][i]=coup[i];

        // mise à jour de legals
        for (short j=0; j<=nbLegals[i]; j++) {
            if (legals[j][i] == coup[i]) {
                legals[j][i]=legals[nbLegals[i]][i]; 
                legals[nbLegals[i]][i]=-1;
                nbLegals[i]--;
            }
        }
    }
    if (nbAdded > 0 ) {
        short currentScore = computeMinDist(coup);
        if (currentScore < max_score)
            max_score = currentScore;
    }
    nbAdded++;
}


void LS::genOneRandom(short indiceCoup[])
{
    short monrand;
    //cout << "genRAND" << endl;

    for (short i=0; i<DIM; i++)
    {
        monrand=(short)((rand () / (RAND_MAX + 1.0)) * (nbLegals[i]+1));
        indiceCoup[i]=monrand;
        if (monrand<0) cerr<<"nbL "<<nbLegals[i]<<" monrand : "<<monrand<<" et nbAdded "<<nbAdded<<endl;
        assert(monrand>=0);
        assert(monrand<=nbLegals[i]);
        //   cout << monrand << "/" << nbLegals[i] << endl;
    }
}

void LS::getCoup(short indiceCoup[], short coup[]) {
    for (short i=0; i<DIM; i++) {
        coup[i] = legals[indiceCoup[i]][i];
    }
}

void LS::playout(short minDist)
{
    short currentScore;
    short indiceCoup[DIM];
    short coup[DIM];
    int j;
    for (short i=nbAdded; i<LARGEUR; i++)
    {
        if (i>0) {
            j=0;
            genOneRandom(indiceCoup);
            getCoup(indiceCoup,coup);
            currentScore = computeMinDist(coup);
            while ((currentScore < minDist) && (j < NB_TRY_GENMOVE)) {
                genOneRandom(indiceCoup);
                getCoup(indiceCoup,coup);
                currentScore = computeMinDist(coup);
                j++;
            }
        } else {
            genOneRandom(indiceCoup);
        }
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
    cout << toString() << endl;
    cout << "Score : "<<computeMinDist()<<endl;
}

// compute the value of the current LS
// update the file containing the best value if there is a new reccord
void LS::updateBest() {
    double bestVal = readBest();
    if (score > bestVal) {
        cerr << endl << "new reccord!!: " << score << endl;
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
    short res=0;
    for (int i=0; i<DIM; i++)
        res+=short(point1[i]-point2[i])*(point1[i]-point2[i]);
    return res;
}

short LS::computeMinDist(short coup[])
{
    if (nbAdded<=0)
        return 0;

    short curDist;
    short minDist = 10000;
    for (int i=0; i<nbAdded; i++)
    {
        curDist = computeDist(hypercube[i],coup);
        if (curDist < minDist)
            minDist = curDist;
    }

    return minDist;
}


short LS::computeMinDist()
{
    if (nbAdded<=1)
        return 0;

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

void LS::getNextCoup(short indiceCoup[], int curPlayout, int nbPlayout, int level) {

    int minDist = 130;
    short coup[DIM];
    genOneRandom(indiceCoup);
    getCoup(indiceCoup,coup);
    short currentScore = computeMinDist(coup);
    if (nbAdded > 1) {
        int j=0;
        while ((currentScore < minDist) && (j < 100)) {
            genOneRandom(indiceCoup);
            getCoup(indiceCoup,coup);
            currentScore = computeMinDist(coup);
            j++;
        }
        if (j == 100)
            cout << "bouh!" << endl;
    }

}

short scoreBestRollout[LEVEL];
short bestRollout[LEVEL][LARGEUR][DIM];


void LS::nested(short level, bool verbose=false)
{
    scoreBestRollout[level] = -1000;
    short indiceCoup[DIM];
    while (nbAdded<LARGEUR)
    {
        if (level >= 1 && verbose)
            cout << endl << endl << "NB ADDED" << nbAdded  <<  " --- " << scoreBestRollout[level] << "/" << max_score << endl;
        //cout << endl << "nb Added: " << nbAdded << endl;
        int nbPlayout = 2;
        for (int curPlayout=0; curPlayout<nbPlayout; curPlayout++) {
            LS currentLS = *this;
            currentLS.getNextCoup(indiceCoup, curPlayout, nbPlayout, level);
            currentLS.add(indiceCoup);

            currentLS.print();
            if (level == 1)
            {
                currentLS.playout(scoreBestRollout[level]);
                //  cout << "currentLS + playout " << endl;
                //  currentLS.print();
                // adding some algo gé to the playout
                population[0] = currentLS;
                if ((nbAdded+1) < LARGEUR)
                    evolve(nbAdded+1);
                //currentLS.score = population[0].score;
                currentLS = population[0];
                // cout << "population 0 optim "  << endl;
                // population[0].print();
                // fin algo gé
                if (currentLS.score >= scoreBestRollout[level])
                {
                    scoreBestRollout[level]=currentLS.score;
                    for (int i=0; i<LARGEUR; i++)
                        for (int j=0; j<DIM; j++)
                            bestRollout[level][i][j]=currentLS.hypercube[i][j];
                    //if (verbose)
                    //    currentLS.print();
                }
            }
            else
            {
                currentLS.nested(level-1, verbose);
                if (currentLS.score >= scoreBestRollout[level])
                {
                    scoreBestRollout[level]=currentLS.score;
                    for (int i=0; i<LARGEUR; i++)
                        for (int j=0; j<DIM; j++)
                            bestRollout[level][i][j]=currentLS.hypercube[i][j];
                    if (verbose)
                        currentLS.print();
                }
            }
        }
        addCoup(bestRollout[level][nbAdded]);
    }
    score=scoreBestRollout[level];
}


int main(int nbargs, char** args)
{
    srand(time(NULL));
#ifdef ONLY_ALGOGE
    srand(time(NULL));
    initPopulation();
    evolve(0,true);
    cout<<"Best Solutions : "<<endl;
    population[0].print();
#else
    LS ls;
    ls.nested(1,true);
    ls.print();
    ls.updateBest();
#endif
}



