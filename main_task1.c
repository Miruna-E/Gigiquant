#include "lib_task1.h"

int main(){

	char *datain[5], *dataout[5]; //path-urile realtive pentru fisierele de input si output

	//Primul interviu - Sharpe ratio

	/*
	Sharpe Ratio -> arata cat de profitabil e riscul asumat de un portofoliu
	ex: despozitul la banca -> risk free (primesti dobanda); randament = dobanda
	randament = profit/investitie, per an
	risc = volatilitatea portofoliului (fluctuatia valorii in timp)
	*/

	datain[0] = "../in/data1.in";
	datain[1] = "../in/data2.in";
	datain[2] = "../in/data3.in";
	datain[3] = "../in/data4.in";
	datain[4] = "../in/data5.in";

	dataout[0] = "../out/data1.out";
	dataout[1] = "../out/data2.out";
	dataout[2] = "../out/data3.out";
	dataout[3] = "../out/data4.out";
	dataout[4] = "../out/data5.out";

	for (int datanr = 0; datanr < 5; datanr++){
		
	FILE *fin = fopen(datain[datanr], "r");
	FILE *fout = fopen(dataout[datanr], "wb");

	if(!fin || !fout){
		exit(1);
	}
	randamentMediu = 0;

	portofoliuMomentT *capListaPortofoliu = NULL;
	double valoareCrt;
	fscanf(fin, "%d", &numarObservatii);
	for (int i = 0; i < numarObservatii; i++){
		fscanf(fin, "%lf ", &valoareCrt);
		adaugaNod(&capListaPortofoliu, valoareCrt);
	}
	if(!capListaPortofoliu){
		exit(1);
	}

	randamentMediu /= (numarObservatii-1);
	double volatilitate = 0;

	double valoareMomentAntecedent = capListaPortofoliu->valoare;
	for(portofoliuMomentT *crt = capListaPortofoliu->urmator; crt != NULL; crt=crt->urmator){
		double randamentCrt = (crt->valoare - valoareMomentAntecedent)/valoareMomentAntecedent;
		valoareMomentAntecedent = crt->valoare;
		volatilitate += pow(randamentCrt - randamentMediu, 2);
	}
	volatilitate = volatilitate/(numarObservatii-1);
	volatilitate = sqrt(volatilitate);
	
	double sharpeRatio = (randamentMediu - randamentFaraRisc)/volatilitate;
	//pret -> 2 zecimale; output -> 3 zecimale; trunchiere
	randamentMediu = (int)(randamentMediu*1000)/1000.0;
	volatilitate = (int)(volatilitate*1000)/1000.0;
	sharpeRatio = (int)(sharpeRatio*1000)/1000.0;
	fprintf(fout, "%.3f\n%.3f\n%.3f\n", randamentMediu, volatilitate, sharpeRatio);
	freePtrLista(capListaPortofoliu);
	fclose(fin);
	fclose(fout);
	}

	return 0;
}