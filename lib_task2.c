#include "lib_task2.h"

//Al doilea interviu - Arbitraj

void push(elemPiataActiune **top, double pretCrt){
	elemPiataActiune *nodNou = (elemPiataActiune*)malloc(sizeof(elemPiataActiune));
	if(nodNou == NULL){
		return;
	}
	nodNou->pret = pretCrt;
	nodNou->urmator = *top;
	*top = nodNou;
}

void pop(elemPiataActiune **top){
	if(*top == NULL){
		return;
	}
	elemPiataActiune *temp = (*top);
	*top = (*top)->urmator;
	free(temp);
}

void stergeStiva(elemPiataActiune **top){
	while((*top) != NULL){
		elemPiataActiune *temp = *top;
		*top = (*top)->urmator;
		free(temp);
	}
}

int floatsEgale(double a, double b){
	if(a > b){
		return (a - b) < 0.0001;
	} else {
		return (b - a) < 0.0001;
	}
}