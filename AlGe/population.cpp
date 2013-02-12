#include "population.h"

Pop::Pop() {
    init();
}

void Pop::init()
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

void Pop::print()
{
    cout<<"Score: " << population[0].score<<endl;
}


void Pop::evolve()
{
	//for (short restart=0; restart<1000; restart++)
	//{
	//	cerr<<"Restart"<<population[0].score<<endl;
	for (short l=0; l<LAMBDA; l++)
		population[l].reinit();
	init();
	int generation=0;
	//	short previous = population[0].score;
	//	short stagne = 0;
	//	while (stagne<1000)
	while (generation<GENERATIONS)
	{
		if (generation%int(GENERATIONS/100)==0)
		{
			cout<<generation/int(GENERATIONS/100)<<"% ("<<population[0].score<<")"<<endl;
		}
		generate();
		select();
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

void Pop::generate()
{
	short j=0;
	short compteur=1;
	for (short i=0; i<LAMBDA; i++)
	{
		if ((LAMBDA==1)||(i<LAMBDA/4))
		{
			population[i+LAMBDA]=population[0];
			population[i+LAMBDA].mutate();
		}
		else
		{
			if (i<1*(LAMBDA/2))
			{
				//	population[i+LAMBDA]=population[j++];
				population[i+LAMBDA]=population[i];
                population[i+LAMBDA].mutate();
				//		if ((compteur++%10)==0) 
				//		{
				//			j++;
				//			compteur=1;
				//		}
			}
			else
				//			if (i<1*(LAMBDA/5))
				//			{
				//					population[i+LAMBDA]=population[0];
				//					mutate(population[i+LAMBDA]);
				//				}
				//				else
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


int Pop::part(int p, int r)
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

