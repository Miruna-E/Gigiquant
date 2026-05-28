#include "lib_task4.h"

int main(){

	char *datain[5], *dataout[5]; //path-urile realtive pentru fisierele de input si output

	//Al patrulea interviu - Lanturi Markov

	/*
    obiectiv:
    1. intervale de pret (starile Markov)
    2. matricea de tranzitie T SAU graful cu muchii si noduri (GRAF)
    3. probabilitatea ca actiunea sa ajunga din intervalul lui Pstart in intervalul lui Ptarget, in FIECARE zi, ptr K zile
    P = A/B (ireductibila) <- det algoritmic (simuland trecerea zilelor pe graf)
    */

	datain[0] = "../in/data16.in";
	datain[1] = "../in/data17.in";
	datain[2] = "../in/data18.in";
	datain[3] = "../in/data19.in";
	datain[4] = "../in/data20.in";

	dataout[0] = "../out/data16.out";
	dataout[1] = "../out/data17.out";
	dataout[2] = "../out/data18.out";
	dataout[3] = "../out/data19.out";
	dataout[4] = "../out/data20.out";

	for (int datanr = 0; datanr < 5; datanr++){
		
	FILE *fin = fopen(datain[datanr], "r");
	FILE *fout = fopen(dataout[datanr], "wb"); //wb pentru ca aveam carriage return la final

	if(!fin || !fout){
		exit(1);
	}

	// input:
	// 1. N = nr observatii
	// 2. d = dimensiune interval
	// 3. K = nr zile
	// 4. P_start = pret initial (de aici SIGUR pornim)
	// 5. P_target = pret tinta (calculam probabilitatea ca din P_start sa ajunga in P_target si sa fie acolo in ziua K)
    // 6-... .serie de preturi zilnice: P1, P2...Pn

	char rand[200]; //un buffer => citesc linie cu linie din fisier

	int nrObservatii, nrZile;
	double PStart = DBL_MAX, PTarget, dimensiuneInterval;

	if(!fgets(rand, sizeof(rand), fin) || sscanf(rand, "%d", &nrObservatii) != 1){
		exit(1); //exit daca esueaza oricare din fgets si sscanf
	}

	if(!fgets(rand, sizeof(rand), fin) || sscanf(rand, "%lf", &dimensiuneInterval) != 1){
		exit(1);
	}

	if(!fgets(rand, sizeof(rand), fin) || sscanf(rand, "%d", &nrZile) != 1){
		exit(1);
	}

	if(!fgets(rand, sizeof(rand), fin) || sscanf(rand, "%lf", &PStart) != 1){
		exit(1);
	}

	if(!fgets(rand, sizeof(rand), fin) || sscanf(rand, "%lf", &PTarget) != 1){
		exit(1);
	}

	//citesc preturi -> creez noduri -> adaug in lista de noduri existente
	double PCrt;
	nodIntervalPreturi *listaIntervaleExistente = NULL;

    for (int i = 0; i < nrObservatii; i++){
        if (fscanf(fin, "%lf ", &PCrt) != 1) break;
        double inceputIntervalCrt = inceputInterval(PCrt, PStart, dimensiuneInterval);
        adaugaNodListaIntervalePreturi(&listaIntervaleExistente, inceputIntervalCrt, dimensiuneInterval);
    }

	int nrNoduriGraf = 0;
	nodIntervalPreturi* listaIntervaleDif = NULL;
	for (nodIntervalPreturi* nodCrt = listaIntervaleExistente; nodCrt != NULL; nodCrt = nodCrt->urmator){
		if(indexIntervalPreturi(listaIntervaleDif, nodCrt->intervalPreturi.inceput) == -1){
			adaugaNodListaIntervalePreturi(&listaIntervaleDif, nodCrt->intervalPreturi.inceput, dimensiuneInterval);
			nrNoduriGraf++;
		}
    }

	nodIntervalPreturi** grafIntervale = (nodIntervalPreturi**)malloc(nrNoduriGraf * sizeof(nodIntervalPreturi*));
    for (int i = 0; i < nrNoduriGraf; i++){
        grafIntervale[i] = NULL;
    }

	grafDinLista(grafIntervale, listaIntervaleExistente, listaIntervaleDif, nrNoduriGraf, dimensiuneInterval);
	freeLista(listaIntervaleExistente);	
	modificaProbabilitati(grafIntervale, nrNoduriGraf);
	
	double inceputIntervalPStart = inceputInterval(PStart, PStart, dimensiuneInterval);
	int iStart = indexIntervalPreturi(listaIntervaleDif, inceputIntervalPStart);
	int iTarget = -1;
	for (nodIntervalPreturi* nodCrt = listaIntervaleDif; nodCrt != NULL; nodCrt = nodCrt->urmator) {
	    if (PTarget >= nodCrt->intervalPreturi.inceput && PTarget < nodCrt->intervalPreturi.sfarsit) {
	        iTarget = indexIntervalPreturi(listaIntervaleDif, nodCrt->intervalPreturi.inceput);
	        break;
	    }
	}

	fractie *probabilitati = (fractie*)malloc(nrNoduriGraf * sizeof(fractie));
	for (int i = 0; i < nrNoduriGraf; i++){
		if(i == iStart){
			probabilitati[i].numarator = 1;
		} else {
			probabilitati[i].numarator = 0;
		}
		probabilitati[i].numitor = 1;
	}
	
	if (iTarget == -1){ //nu a fost citit nodul care este cautat, nu exista
		for (int i = 0; i < nrZile; i++){
			fprintf(fout, "0");
			if(i != nrZile - 1){
				fprintf(fout, "\n");
			}
		}
	} else {
        for (int i = 0; i < nrZile; i++){
            if (i > 0) {
				markov(grafIntervale, listaIntervaleDif, nrNoduriGraf, probabilitati);
			}
            fractie ptrAfisare = probabilitati[iTarget];
            if(ptrAfisare.numarator == 0){
                fprintf(fout, "0");
            } else if(ptrAfisare.numarator == ptrAfisare.numitor){
                fprintf(fout, "1");
            } else{
                fprintf(fout, "%lld/%lld", ptrAfisare.numarator, ptrAfisare.numitor);
            }
            if(i != nrZile - 1){ 
                fprintf(fout, "\n");
            }
        }
    }

	free(probabilitati);
	freeLista(listaIntervaleDif);	
	freeGraf(grafIntervale, nrNoduriGraf);
	free(grafIntervale);
	
	fclose(fin);
	fclose(fout);
	}

	return 0;
}