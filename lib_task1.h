#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//Primul interviu - Sharpe ratio

extern int randamentFaraRisc;
extern double randamentMediu;
extern int numarObservatii;

//lista simplu inlantuita
struct Nod{
	double valoare;
	double randament;
	struct Nod* urmator;
};
typedef struct Nod portofoliuMomentT;

void adaugaNod(portofoliuMomentT **cap, double valoareNoua);
void freePtrLista(portofoliuMomentT *cap);