#include "lib_task2.h"

int main(){

	char *datain[5], *dataout[5]; //path-urile realtive pentru fisierele de input si output
	//Al doilea interviu - Arbitraj
	/*
	oportunitati de arbitraj: per zi de tranzactionare, se cunosc preturile aceleiasi actiuni pe trei piete diferite. Preturile: P1, P2, P3
	fara oportunitati => P1 = P2 = P3
	oportunitati => 2 preturi egale, al 3-lea diferit
	situatie de evitat => P1 =/= P2 =/= P3 =/= P1
	*/

	// input:
	// -numele a 3 piete, numar necunoscut de preturi in ordine cronologica per piata (ultimul este cel mai vechi)
	// -ultimul pret este din aceeasi zi pentru toate fisierele si nu exista zile lipsa

	datain[0] = "../in/data6.in";
	datain[1] = "../in/data7.in";
	datain[2] = "../in/data8.in";
	datain[3] = "../in/data9.in";
	datain[4] = "../in/data10.in";

	dataout[0] = "../out/data6.out";
	dataout[1] = "../out/data7.out";
	dataout[2] = "../out/data8.out";
	dataout[3] = "../out/data9.out";
	dataout[4] = "../out/data10.out";

	for (int datanr = 0; datanr < 5; datanr++){
	
	FILE *fin = fopen(datain[datanr], "r");
	FILE *fout = fopen(dataout[datanr], "wb");

	if(!fin || !fout){
		exit(1);
	}
	
	stivaPiataActiune *Piata1 = (stivaPiataActiune*)malloc(sizeof(stivaPiataActiune));
	stivaPiataActiune *Piata2 = (stivaPiataActiune*)malloc(sizeof(stivaPiataActiune));
	stivaPiataActiune *Piata3 = (stivaPiataActiune*)malloc(sizeof(stivaPiataActiune));

	Piata1->stivaTop = NULL;
	Piata2->stivaTop = NULL;
	Piata3->stivaTop = NULL;

	double dateCrt;
	char rand[200]; //un buffer => citesc linie cu linie din fisier
	//fgets si sscanf in loc de fscanf ptr a vedea cand se ajunge la urmatoarea piata

	Piata1->nume = (char*)malloc(80 * sizeof(char)); //presupun ca numele are mai putin de 80 de litere
	if(fgets(rand, sizeof(rand), fin)){
		sscanf(rand, " %79[^\n]", Piata1->nume);
		//' ' => fara whitespace
		//[^\n] => include spatii, se opreste la newline; pentru numele de orase cu mai multe cuvinte
	}

	while(fgets(rand, sizeof(rand), fin)){
		if(sscanf(rand, "%lf", &dateCrt) == 1){
			push(&(Piata1->stivaTop), dateCrt);
		} else if(strlen(rand) > 1){
			break;
		}
    }

	Piata2->nume = (char*)malloc(80 * sizeof(char));
	sscanf(rand, " %79[^\n]", Piata2->nume);

	while(fgets(rand, sizeof(rand), fin)){
		if(sscanf(rand, "%lf", &dateCrt) == 1){
			push(&(Piata2->stivaTop), dateCrt);
		} else if(strlen(rand) > 1){
			break;
		}
    }

	Piata3->nume = (char*)malloc(80 * sizeof(char));
	sscanf(rand, " %79[^\n]", Piata3->nume);

	while(fgets(rand, sizeof(rand), fin)){
		if(sscanf(rand, "%lf", &dateCrt) == 1){
			push(&(Piata3->stivaTop), dateCrt);
		} else if(strlen(rand) > 1){
			break;
		}
    }

	fclose(fin);

	// output:
	// -ziua in care a existat oportunitatea
	// -diferenta absoluta intre valoarea diferita si celelalte doua 
	// -numele valorii diferite

	int ziOportunitate = 1;
	double difZileOportunitate;

	while(Piata1->stivaTop != NULL && Piata2->stivaTop != NULL && Piata3->stivaTop != NULL){
		if(floatsEgale(Piata1->stivaTop->pret, Piata2->stivaTop->pret)){
			if(!floatsEgale(Piata1->stivaTop->pret, Piata3->stivaTop->pret)){
				difZileOportunitate = fabs(Piata1->stivaTop->pret - Piata3->stivaTop->pret); //am schimbat abs in fabs (abs ptr int, fabs ptr float)
				fprintf(fout, "ziua %d - %.2lf - %s\n", ziOportunitate, difZileOportunitate, Piata3->nume);
			}
		} else if(floatsEgale(Piata2->stivaTop->pret, Piata3->stivaTop->pret)){
			if(!floatsEgale(Piata1->stivaTop->pret, Piata3->stivaTop->pret)){
				difZileOportunitate = fabs(Piata1->stivaTop->pret - Piata3->stivaTop->pret);
				fprintf(fout, "ziua %d - %.2lf - %s\n", ziOportunitate, difZileOportunitate, Piata1->nume);
			}
		} else if(floatsEgale(Piata1->stivaTop->pret, Piata3->stivaTop->pret)){
			if(!floatsEgale(Piata2->stivaTop->pret, Piata3->stivaTop->pret)){
				difZileOportunitate = fabs(Piata2->stivaTop->pret - Piata3->stivaTop->pret);
				fprintf(fout, "ziua %d - %.2lf - %s\n", ziOportunitate, difZileOportunitate, Piata2->nume);
			}
		}
		ziOportunitate++;
		pop(&(Piata1->stivaTop));
		pop(&(Piata2->stivaTop));
		pop(&(Piata3->stivaTop));
	}

	fflush(fout);
	stergeStiva(&(Piata1->stivaTop));
	free(Piata1->nume);
	free(Piata1);
	stergeStiva(&(Piata2->stivaTop));
	free(Piata2->nume);
	free(Piata2);
	stergeStiva(&(Piata3->stivaTop));
	free(Piata3->nume);
	free(Piata3);
	
	fclose(fout);
	}

	return 0;
}