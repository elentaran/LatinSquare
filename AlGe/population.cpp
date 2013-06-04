#include "population.h"

extern double percentBest;
extern double percentGood;

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

void Pop::print()
{
    cout<<"Score: " << population[0].score<<endl;
    //cout<<"Fake: " << population[0].fakeScore<<endl;
}


void Pop::evolve()
{
	//for (short restart=0; restart<1000; restart++)
	//{
	//	cerr<<"Restart"<<population[0].score<<endl;
    double bestVal = population[0].readBest();
	for (short l=0; l<LAMBDA; l++)
		population[l].reinit();
	init();
	int generation=0;
    double previous = population[0].fakeScore;
	short stagne = 0;
    while (stagne<500)
	//while (generation<GENERATIONS)
	{
		if (generation%int(GENERATIONS/100)==0)
		{
            cout<<generation/int(GENERATIONS/100)<<"% (real: "<<population[0].score<<"/"<<bestVal<<")(fake: " << population[0].fakeScore << ")"<<endl;
		}
		generate();
		select();
		population[0].updateBest();
		generation++;
				if (population[0].fakeScore<=previous)
				{
					stagne++;
				}
				else
				{
					previous=population[0].fakeScore;
					stagne=0;
				}
	}
	//}
}

void Pop::generate()
{
    short nbGenFromBest = percentBest * LAMBDA;
    short nbGenFromGood = percentGood * LAMBDA;
    //short nbGenFromBest = 0.5 * LAMBDA;
    //short nbGenFromGood = 0.5 * LAMBDA;

	for (short i=0; i<LAMBDA; i++)
	{
		if (i<nbGenFromGood)
		{
            population[i+LAMBDA]=population[i];
            population[i+LAMBDA].mutate();
		}
		else if (i<nbGenFromGood + nbGenFromBest)
        {
			population[i+LAMBDA]=population[0];
			population[i+LAMBDA].mutate();
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
		if ( population[i].fakeScore > pivot )
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

