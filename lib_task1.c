#include "lib_task1.h"

//Primul interviu - Sharpe ratio

int randamentFaraRisc = 0;
double randamentMediu = 0;
int numarObservatii;

void adaugaNod(portofoliuMomentT **cap, double valoareNoua){
	if(!cap) return;
	portofoliuMomentT *nodNou = malloc(sizeof(portofoliuMomentT));
	nodNou->valoare = valoareNoua;
	nodNou->randament = 0;
	nodNou->urmator = NULL;
	if(*cap == NULL){
		*cap = nodNou;
	} else {
		portofoliuMomentT *crt;
		for(crt = *cap; crt->urmator != NULL; crt = crt->urmator){
			//merg la ultimul element din lista
		}
		double valoareMomentAntecedent = crt->valoare;
		crt->urmator = nodNou;
		crt->urmator->randament = (crt->urmator->valoare - valoareMomentAntecedent)/valoareMomentAntecedent;
		randamentMediu += crt->urmator->randament;
	}
}

void freePtrLista(portofoliuMomentT *cap){
	while(cap != NULL){
		portofoliuMomentT *crt = cap;
		cap = cap->urmator;
		free(crt); //free lista, element cu element
	}
}