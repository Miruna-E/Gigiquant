#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <float.h>

struct interval {
    double inceput, sfarsit;
};
typedef struct interval interval;

struct fractie{
    long long numarator;
    long long numitor;
};
typedef struct fractie fractie;

struct nodIntervalPreturi{
    interval intervalPreturi;
    fractie probabilitate;
    struct nodIntervalPreturi* urmator;
};
typedef struct nodIntervalPreturi nodIntervalPreturi;

int doubleEgale(double a, double b);
double inceputInterval(double PCrt, double PStart, double dimensiuneInterval);
void adaugaNodListaIntervalePreturi(nodIntervalPreturi **cap, double inceputIntervalNou, double dimensiuneInterval);
void freeLista(nodIntervalPreturi *cap);
int indexIntervalPreturi(nodIntervalPreturi *cap, double inceputCrt);
void grafDinLista(nodIntervalPreturi **graf, nodIntervalPreturi *listaIntervaleExistente, nodIntervalPreturi *listaIntervaleDif, int nrNoduriGraf, double dimensiuneInterval);
void freeGraf(nodIntervalPreturi **graf, int nrDif);
long long cmmdc(long long x, long long y);
void simplificare(fractie *x);
fractie sumaFractii(fractie x, fractie y);
fractie inmultireFractii(fractie x, fractie y);
void modificaProbabilitati(nodIntervalPreturi **graf, int nrNoduriGraf);
void markov(nodIntervalPreturi **graf, nodIntervalPreturi *listaIntervaleDif, int nrNoduriGraf, fractie *probabilitati);