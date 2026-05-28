#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

struct elemPiataActiune{
	double pret;
	struct elemPiataActiune *urmator;
};
typedef struct elemPiataActiune elemPiataActiune;

struct stivaPiataActiune{
	elemPiataActiune *stivaTop;
	char *nume;
};
typedef struct stivaPiataActiune stivaPiataActiune;

void push(elemPiataActiune **top, double pretCrt);
void pop(elemPiataActiune **top);
void stergeStiva(elemPiataActiune **top);

int floatsEgale(double a, double b);