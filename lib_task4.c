#include "lib_task4.h"

int doubleEgale(double a, double b){
	return fabs(a - b) < 0.0001;
}

long long cmmdc(long long x, long long y){
    x = llabs(x); //llabs = abs ptr long long
    y = llabs(y);
    if(y == 0) return x;
    return cmmdc(y, x % y);
}

void simplificare(fractie *x){
    if (x->numarator == 0) {
        x->numitor = 1;
        return;
    }
    long long impartePrin = cmmdc(x->numarator, x->numitor);
    if(impartePrin != 0){
        x->numarator /= impartePrin;
        x->numitor /= impartePrin;
    }
}

fractie sumaFractii(fractie x, fractie y){
    if (x.numarator == 0) {
        return y;
    } 
    if (y.numarator == 0){
        return x;
    }
    fractie rezultat;
    long long cmmc = (x.numitor / cmmdc(x.numitor, y.numitor)) * y.numitor; 
    rezultat.numarator = x.numarator * (cmmc/x.numitor) + y.numarator * (cmmc/y.numitor);
    rezultat.numitor = cmmc;
    simplificare(&rezultat);
    return rezultat;
}

fractie inmultireFractii(fractie x, fractie y){
    fractie rezultat;
    rezultat.numarator = x.numarator * y.numarator;
    rezultat.numitor = x.numitor * y.numitor;
    simplificare(&rezultat);
    return rezultat;
}

double inceputInterval(double PCrt, double PStart, double dimensiuneInterval){
    int intermediar = floor(PCrt / dimensiuneInterval); 
    return intermediar * dimensiuneInterval;
}

//lista:

void adaugaNodListaIntervalePreturi(nodIntervalPreturi **cap, double inceputIntervalNou, double dimensiuneInterval){
    if(!cap) return;
    nodIntervalPreturi* nodNou = (nodIntervalPreturi*)malloc(sizeof(nodIntervalPreturi));
    if(nodNou == NULL){
        return;
    }
    nodNou->intervalPreturi.inceput = inceputIntervalNou;
    nodNou->intervalPreturi.sfarsit = inceputIntervalNou + dimensiuneInterval;
    nodNou->probabilitate.numarator = 1;
    nodNou->probabilitate.numitor = 1;
    nodNou->urmator = NULL;

	if(*cap == NULL){
		*cap = nodNou;
	} else {
		nodIntervalPreturi *crt;
		for(crt = *cap; crt->urmator != NULL; crt = crt->urmator){
            //merg la ultimul element din lista
		}
		crt->urmator = nodNou;
	}
}

void freeLista(nodIntervalPreturi *cap){
	while(cap != NULL){
		nodIntervalPreturi *crt = cap;
		cap = cap->urmator;
		free(crt); //free lista, element cu element
	}
}

//graf:

int indexIntervalPreturi(nodIntervalPreturi *cap, double inceputCrt){
    for(int i = 0; cap != NULL; i++, cap = cap->urmator){
        if(doubleEgale(cap->intervalPreturi.inceput, inceputCrt)){
            return i;
        }
    }
    return -1;
}

//=> graf orientat cu ponderi
void grafDinLista(nodIntervalPreturi **graf, nodIntervalPreturi *listaIntervaleExistente, nodIntervalPreturi *listaIntervaleDif, int nrNoduriGraf, double dimensiuneInterval){
    if(listaIntervaleExistente == NULL || listaIntervaleExistente->urmator == NULL){
        return;
    }
    for(nodIntervalPreturi *nodCrt = listaIntervaleExistente; nodCrt->urmator != NULL; nodCrt = nodCrt->urmator){
        double nodPlecareInceput = nodCrt->intervalPreturi.inceput;
        double nodSosireInceput = nodCrt->urmator->intervalPreturi.inceput;
        
        int u = indexIntervalPreturi(listaIntervaleDif, nodPlecareInceput);
        if(u != -1){
            nodIntervalPreturi *muchieCrt = graf[u];
            short int existaMuchie = 0;
            while(muchieCrt != NULL){
                if(doubleEgale(nodSosireInceput, muchieCrt->intervalPreturi.inceput)){
                    muchieCrt->probabilitate.numarator++;
                    existaMuchie = 1;
                    break;
                }
                muchieCrt = muchieCrt->urmator;
            }

            if(!existaMuchie){
                nodIntervalPreturi* nodNou = (nodIntervalPreturi*)malloc(sizeof(nodIntervalPreturi));
                if(nodNou == NULL){
                    return;
                }
                nodNou->intervalPreturi.inceput = nodSosireInceput;
                nodNou->intervalPreturi.sfarsit = nodSosireInceput + dimensiuneInterval;
                nodNou->probabilitate.numarator = 1;
                nodNou->probabilitate.numitor = 1; 
                nodNou->urmator = graf[u];
                graf[u] = nodNou;
            }
        }
    }
}

//=> probabilitati ca fractii simple
void modificaProbabilitati(nodIntervalPreturi **graf, int nrNoduriGraf){
    for (int i = 0; i < nrNoduriGraf; i++){
        int totalTranzitii = 0;
        nodIntervalPreturi *muchieCrt = graf[i];
        while (muchieCrt != NULL){
            totalTranzitii += muchieCrt->probabilitate.numarator;
            muchieCrt = muchieCrt->urmator;
        }
        for (muchieCrt = graf[i]; muchieCrt != NULL; muchieCrt = muchieCrt->urmator){
            muchieCrt->probabilitate.numitor = totalTranzitii;
            simplificare(&(muchieCrt->probabilitate));
        }
    }
}

void markov(nodIntervalPreturi **graf, nodIntervalPreturi *listaIntervaleDif, int nrNoduriGraf, fractie *probabilitati){
    fractie *probabilitateViitoare = (fractie*)malloc(nrNoduriGraf * sizeof(fractie));
    for (int i = 0; i < nrNoduriGraf; i++){
        probabilitateViitoare[i].numarator = 0;
        probabilitateViitoare[i].numitor = 1;
    }

    for (int iNodCrt = 0; iNodCrt < nrNoduriGraf; iNodCrt++){
        if(probabilitati[iNodCrt].numarator != 0){
            for (nodIntervalPreturi *muchieCrt = graf[iNodCrt]; muchieCrt != NULL; muchieCrt = muchieCrt->urmator){
                int iAdiacent = indexIntervalPreturi(listaIntervaleDif, muchieCrt->intervalPreturi.inceput);
                if(iAdiacent != -1){
                    fractie temp = inmultireFractii(probabilitati[iNodCrt], muchieCrt->probabilitate);
                    probabilitateViitoare[iAdiacent] = sumaFractii(probabilitateViitoare[iAdiacent], temp);
                }
            }
        }
    }
    
    for (int i = 0; i < nrNoduriGraf; i++){ //mergem la nodurile adiacente (NU recursiv)
        probabilitati[i] = probabilitateViitoare[i];
    }
    free(probabilitateViitoare);
}

void freeGraf(nodIntervalPreturi **graf, int nrDif){ //ca in laborator
    for (int i = 0; i < nrDif; i++){
        nodIntervalPreturi* nodCrt = graf[i];
        while (nodCrt != NULL){
            nodIntervalPreturi* temp = nodCrt;
            nodCrt = nodCrt->urmator;
            free(temp); //free graf, element cu element
        }
    } 
}