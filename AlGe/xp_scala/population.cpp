#include "population.h"


extern float percentBest;
extern float percentGood;

Pop::Pop() {
    init();
}

void Pop::init()
{
	for (short p=0; p<LAMBDA; p++)
        population[p].initRand();
	for (short p=LAMBDA; p<LAMBDA*2; p++)
		population[p].initDiagonal();
}

// method used instead of restart from start
void Pop::perturb()
{
    for (short i=0; i<LAMBDA; i++)
    {
        population[i].mutateStrong();
    }
}

void Pop::print()
{
    //cout<<"Score: " << population[0].score<<endl;
    cout<<"best Score: " << bestScore<<endl;
    //cout<<"Fake: " << population[0].fakeScore<<endl;
}


void Pop::evolve()
{
#ifdef RESTART
	for (short restart=0; restart<RESTART; restart++)
	{
		//cerr<<"Restart"<<endl;
   //perturb();
    //double bestVal = population[0].readBest();
	for (short l=0; l<LAMBDA; l++)
		population[l].reinit();
	init();
#endif
	int generation=0;

#ifdef STAGN
    double previous = population[0].fakeScore;
	short stagne = 0;
    while (stagne<STAGN)
#else
	while (generation<GEN)
#endif
	{
		generate();
		select();
#ifdef SAVEBEST
		population[0].updateBest();
#endif
        if (population[0].score > bestScore)
            bestScore = population[0].score;
		generation++;
		if (generation%int(GEN/100)==0)
		{
            //cout<<generation/int(GENERATIONS/100)<<"% (real: "<<population[0].score<<"/"<<bestVal<<")(fake: " << population[0].fakeScore << ")"<<endl;
            //cout<<"Score: "<<generation<< endl;
            //cout<<"Score: "<<population[0].score<<endl;
                //cout<<"i: " << i << " Score: "<<population[i].score << "(" << bestScore << ")"  << "(Fake: " << population[i].fakeScore << ")" << " / stagn: " << stagne <<endl;
                //cout<< " Score: "<<population[0].score << "(" << bestScore << ")"  << "(Fake: " << population[0].fakeScore << ")" << " / stagn: " << stagne <<endl;
               // cout<< " Score: "<<population[0].score << "(" << bestScore << ")"  << "(Fake: " << population[0].fakeScore << ")" << " / stagn: " << stagne << " / restart: " << restart <<endl;
                
               // cout<< " Score: "<<population[0].score << "(" << bestScore << ")"  << "(Fake: " << population[0].fakeScore << ")" <<endl;

		}

#ifdef STAGN
#ifdef FAKESCORE
#ifdef PHI
        if (population[0].fakeScore>=previous)
#else
        if (population[0].fakeScore<=previous)
#endif
#else
        if (population[0].score<=previous)
#endif
        {
            stagne++;
        }
        else
        {
#ifdef FAKESCORE
            previous=population[0].fakeScore;
#else
            previous=population[0].score;
#endif
            stagne=0;
        }
#endif
    }
#ifdef RESTART
    }
#endif
}


void Pop::generate()
{
    srand(time(NULL));
    short nbGenFromBest = percentBest * LAMBDA;
    short nbGenFromGood = percentGood * LAMBDA;

    for (short i=0; i<LAMBDA; i++)
    {
        if (i<nbGenFromGood)
        {
            population[i+LAMBDA]=population[i];
            switch(MUTATION) {
                case 1:
                    population[i+LAMBDA].mutate1();
                    break;
                case 2:
                    population[i+LAMBDA].mutate2();
                    break;
                case 3:
                    population[i+LAMBDA].mutate3();
                    break;
                case 4:
                    population[i+LAMBDA].mutate4();
                    break;
                case 5:
                    population[i+LAMBDA].mutateRand();
                    break;
                
            }
            //population[i+LAMBDA].mutate();
            //population[i+LAMBDA].mutate3();
            //population[i+LAMBDA].mutateRand();
        }
        else if (i<nbGenFromGood + nbGenFromBest)
        {
            population[i+LAMBDA]=population[0];
            //population[i+LAMBDA].mutate();
            population[i+LAMBDA].mutate3();
        } 
        else 
        {
            population[i+LAMBDA].initRand();
        }
    }
}


int Pop::part(int p, int r)
{
    short compt=p;
#ifdef FAKESCORE
    double pivot=population[p].fakeScore;
#else
    short pivot=population[p].score;
#endif
    int i;

    for (i=p+1; i<=r; i++)
    {
#ifdef FAKESCORE
#ifdef PHI
        if ( population[i].fakeScore < pivot )
#else
        if ( population[i].fakeScore > pivot )
#endif
#else
        if ( population[i].score > pivot )
#endif
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


void Pop::sortVector(int p, int r)
{
    int q;
    if (p<r)
    {
        q = part(p, r);
        sortVector(p, q-1);
        sortVector(q+1, r);
    }
}

void Pop::select()
{
    //#define COMMA
#ifdef COMMA
    sortVector(LAMBDA, LAMBDA*2-1);
    for (short i=0; i<LAMBDA; i++)
        population[i]=population[i+LAMBDA];
#else
    sortVector(0, LAMBDA*2-1);
#endif
}

