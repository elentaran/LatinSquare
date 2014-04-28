#include "fastLS.h"



LS::LS()
{
    reinit();
}

LS LS::clone() {
    LS ls;
    ls.score=score;
    ls.fakeScore=fakeScore;
    ls.nbAdded=nbAdded;
    for (short i=0; i<DIM; i++)
    {   
        for (short j=0; j<LARGEUR; j++)
        {   
            ls.hypercube[j][i]=hypercube[j][i];
            ls.legals[j][i]=legals[j][i];

        }
        ls.nbLegals[i]=nbLegals[i];
    }
    return ls;
}


void LS::reinit()
{
    score=-1000;
    fakeScore=-1000;
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
#ifdef FAKESCORE
    fakeScore=computeFakeScore();
#endif
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
#ifdef FAKESCORE
    cout << "Fake Score : "<<fakeScore<<endl;
#endif
}

// compute the value of the current LS
// update the file containing the best value if there is a new reccord
void LS::updateBest() {
    double bestVal = readBest();
    if (score > bestVal) {
        cerr << "new reccord!!: " << score << endl;
        writeBest();
#ifdef SENDMAIL
        stringstream message;
        message << "./sendMail.rb ";
        message << "'new value: "<< score << "'";
        message << " 'new highscore LS - dim " << DIM << " - size " << LARGEUR << "'"  ;
        system(message.str().c_str());
        cerr << "mail sent" << endl;
#endif
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

// compute the distance between the 2 LS
double computeDistLS(LS other) {
    return 0;
}

short part(short myVec[], short beg, short end) {
    short compt=beg;
    short pivot=myVec[beg];
    int i;

    for (i=beg+1; i<=end; i++)
    {
        if ( myVec[i] > pivot )
        {
            compt++;
            short temp = myVec[i];
            myVec[i] = myVec[compt];
            myVec[compt] = temp;
        }
    }
    short temp;
    temp = myVec[beg];
    myVec[beg] = myVec[compt];
    myVec[compt] = temp;
    return compt;
}

void quicksort(short myVec[], short beg, short end) {
    short q;
    if (beg<end)
    {
        q = part(myVec, beg, end);
        quicksort(myVec, beg, q-1);
        quicksort(myVec, q+1, end);
    }
}

double LS::computeScorePhi() {
    int max = LARGEUR*LARGEUR*DIM;
    short curDist;
    int param_p = TEST;
    vector<int> listDist;
    for (int i=0; i<(LARGEUR-1); i++)
    {
        for (int j=i+1; j<LARGEUR; j++)
        {
            curDist = computeDist(hypercube[i],hypercube[j]);
            listDist.push_back(curDist);
        }
    }

    sort(listDist.begin(),listDist.end());
    double score=0;
    int count=1;
    double curScore=listDist[0];
    for (int i=1; i<listDist.size(); i++) {
        assert (curScore != 0) ;
        if (curScore == listDist[i]) {
            count++;
        } else {
            // cout << "count: " << count << " score: " << curScore << endl;
            // cout << "ajout: " << count/pow(curScore,param_p) << endl;
            score += count/pow(curScore,param_p);
            count=1;
        }
        curScore = listDist[i];

    }
    //cout << "count: " << count << " score: " << curScore << endl;
    //cout << "ajout: " << count/pow(curScore,param_p) << endl;
    score += count/pow(curScore,param_p);
    //score += pow((count/(pow(curScore,param_p))),1./param_p);
    score = pow(score,1.0/param_p);
    //cout << score << endl;
    //nbEval++;
    return score;

}

double LS::computeFakeScore() {
    int max = LARGEUR*LARGEUR*DIM;
    short curDist;
    short minDistAll = 10000;
    short minDist;
    double sumDist = 0;
    short listMinDist[LARGEUR];
    for (int i=0; i<LARGEUR; i++)
    {
        minDist = 10000;
        for (int j=0; j<LARGEUR; j++)
        {
            if (i!=j) {
                curDist = computeDist(hypercube[i],hypercube[j]);
                assert(curDist >= DIM);
                if (curDist < minDist)
                    minDist = curDist;
                if (curDist < minDistAll)
                    minDistAll = curDist;
            }
        }
        listMinDist[i] = minDist;
        //sumDist+=minDist;
    }

    quicksort(listMinDist,0,LARGEUR);
    for (int i=0; i<LARGEUR; i++) {
        sumDist += (double) listMinDist[i]*i;
    }
    return sumDist;

}

void LS::mutateStrong()
{
    int nbMutations = 5; // TODO depends on DIM and LARGEUR
    for (int i=0; i<nbMutations; i++) {
        mutate();
    }
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
        }
    }
    computeScores();
}

vector<short> LS::getCriticalPoints()
{
    std::vector<short> indMinDists;
    short curDist;
    short minDist = 10000;
    for (int i=0; i<(LARGEUR-1); i++)
    {   
        for (int j=i+1; j<LARGEUR; j++)
        {   
            curDist = computeDist(hypercube[i],hypercube[j]);
            if (curDist < minDist)
            {   
                minDist = curDist;
            }
        }
    }

    for (int i=0; i<(LARGEUR-1); i++)
    {   
        for (int j=i+1; j<LARGEUR; j++)
        {   
            if (computeDist(hypercube[i],hypercube[j]) == minDist)
            {   
                indMinDists.push_back(i);
                indMinDists.push_back(j);
            }
        }
    }

    return indMinDists;
}

short LS::getARandomCriticalPoint()
{
    std::vector<short> indMinDists;
    short curDist;
    short minDist = 10000;
    for (int i=0; i<(LARGEUR-1); i++)
    {   
        for (int j=i+1; j<LARGEUR; j++)
        {   
            curDist = computeDist(hypercube[i],hypercube[j]);
            if (curDist < minDist)
            {   
                minDist = curDist;
            }
        }
    }

    for (int i=0; i<(LARGEUR-1); i++)
    {   
        for (int j=i+1; j<LARGEUR; j++)
        {   
            if (computeDist(hypercube[i],hypercube[j]) == minDist)
            {   
                indMinDists.push_back(i);
                indMinDists.push_back(j);
            }
        }
    }

    return indMinDists[rand()%indMinDists.size()];
}

void LS::mutate1() {
    short indicePoint = getARandomCriticalPoint();
    short indiceOtherPoint;

    do {
        indiceOtherPoint = (short)(rand () % LARGEUR);
    } while ( indicePoint == indiceOtherPoint );

    //short numberOfAxisToChange = rand()%((DIM/2)-1)+1;
    short numberOfAxisToChange = 0;

    while (numberOfAxisToChange)
    {   
        short onmute = rand()%DIM;
        short temp;
        temp = hypercube[indicePoint][onmute];
        hypercube[indicePoint][onmute] = hypercube[indiceOtherPoint][onmute];
        hypercube[indiceOtherPoint][onmute] = temp;
        numberOfAxisToChange--;
    }
    computeScores();
}

void LS::mutate2() {
    short indicePoint = getARandomCriticalPoint();
    short indiceOtherPoint;

    do {
        indiceOtherPoint = (short)(rand () % LARGEUR);
    } while ( indicePoint == indiceOtherPoint );

    short onmute = rand()%DIM;
    short temp;
    temp = hypercube[indicePoint][onmute];
    hypercube[indicePoint][onmute] = hypercube[indiceOtherPoint][onmute];
    hypercube[indiceOtherPoint][onmute] = temp;
    computeScores();
}


void LS::mutate3() {
    short indicePoint = getARandomCriticalPoint();
    short indiceOtherPoint;

    do {
        indiceOtherPoint = (short)(rand () % LARGEUR);
    } while ( indicePoint == indiceOtherPoint );

    short mindist = -1000;
    short indiceMin = -1;

    for (short onmute = 0; onmute<DIM; onmute++)
    {
        LS copcop = this->clone();
        short temp = copcop.hypercube[indicePoint][onmute];
        copcop.hypercube[indicePoint][onmute] = copcop.hypercube[indiceOtherPoint][onmute];
        copcop.hypercube[indiceOtherPoint][onmute] = temp;

        short tmin = copcop.computeMinDist();
        if (tmin>mindist || indiceOtherPoint==0) {
            mindist = tmin;
            indiceMin = onmute;
        }
    }
    short ttt;
    ttt = hypercube[indicePoint][indiceMin];
    hypercube[indicePoint][indiceMin] = hypercube[indiceOtherPoint][indiceMin];
    hypercube[indiceOtherPoint][indiceMin] = ttt;
    computeScores();
} 

void LS::mutate4() {
    short indicePoint = (short)(rand () % LARGEUR);
    short indiceOtherPoint;

    do {
        indiceOtherPoint = (short)(rand () % LARGEUR);
    } while ( indicePoint == indiceOtherPoint );

    short onmute = rand()%DIM;
    short temp;
    temp = hypercube[indicePoint][onmute];
    hypercube[indicePoint][onmute] = hypercube[indiceOtherPoint][onmute];
    hypercube[indiceOtherPoint][onmute] = temp;
    computeScores();
}

void LS::mutateRand() {
    short choice = rand()%3;
    switch (choice) {
        case 0:
            mutate1();
            break;
        case 1:
            mutate2();
            break;
        case 2:
            mutate3();
            break;
    }
}

void LS::perturb() { // perturbation corresponding to the ILS paper
    assert(LARGEUR > 2);
    short indicePoint1 = (short)((rand () % (LARGEUR-2))+2);
    short indicePoint2 = (short)(rand () % (indicePoint1-1));

    short indiceDim = rand()%DIM;

    short saveValue = hypercube[indicePoint2][indiceDim];
    for (int i=indicePoint2; i<indicePoint1; i++) {
        hypercube[i][indiceDim] = hypercube[i+1][indiceDim];
    }
    hypercube[indicePoint1][indiceDim] = saveValue;
    computeScores();
}


void LS::initRand() {

    reinit();
    short indiceCoup[DIM];
    for (short i=0; i<LARGEUR; i++)
    {
        genOneRandom(indiceCoup);
        add(indiceCoup);
    }
    computeScores();

}

void LS::computeScores() {
    nbEval++;
    score = computeMinDist();
#ifdef FAKESCORE
#ifdef PHI
    fakeScore = computeScorePhi();
#else
    fakeScore = computeFakeScore();
#endif
#endif
}
