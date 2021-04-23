#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<vector>
#include<iostream>
#include<fstream>
#include<string>
#include<unistd.h>

using namespace std;

class Arco {
public:
	int numarc;
	int linha1, linha2;
	int riseout, risein;
	int difin, difout;
	char saida1, saida2;
};
char* bindec(int num, int tam) {
	int x;
	int i = (tam - 1);
	int dimension = tam + 1;
	char* bit = (char*)malloc(sizeof(char) * dimension);

	for (x = 0; x < dimension; x++) {
		bit[x] = '0';
	}
	while (num > 0) {
		if ((num % 2) == 0) bit[i] = '0';

		else bit[i] = '1';

		i = i - 1;
		num = num / 2;
	}
	bit[tam] = '\0';
	return bit;
}
char* conversor(int num, int tam) {
	int numAux, tamAux, dif;
	if (num > 0) {
		return bindec(num, tam);
	}
	else {
		numAux = num * (-1);
		if (tam == 4) {
			tamAux = 8;
		}
		else {
			tamAux = 128;
		}
		dif = tamAux - numAux;
		num = tamAux + dif;
		return bindec(num, tam);
	}
}
int main() {
	int ctl_a = -8, ctl_b = -8;
	int a = 0, b = 0, resultado = 0, cont = 0;
	int i, j, k, dif = 0;
	int col_tab = 0;
	int linha_tab = 0;
	char* vetor_a, * vetor_b, * vetor_r;
	char entrada[256][8], saida[256][8], linha[8];

	vector<Arco> lista;
	Arco meuArc;
	ofstream font, measure, inout;

	for (i = 0; i <= 15; i++) {
		for (j = 0; j <= 15; j++) {
			if (i < 8 && j < 8) {
				a = i;
				b = j;
				resultado = a * b;
				vetor_a = conversor(a, 4);
				vetor_b = conversor(b, 4);
				vetor_r = conversor(resultado, 8);
			}
			else if (i < 8 && j >= 8) {
				a = i;
				b = ctl_b;
				ctl_b++;
				resultado = a * b;
				vetor_a = conversor(a, 4);
				vetor_b = conversor(b, 4);
				vetor_r = conversor(resultado, 8);
			}
			else if (i >= 8 && j < 8) {
				a = ctl_a;
				b = j;
				resultado = a * b;
				vetor_a = conversor(a, 4);
				vetor_b = conversor(b, 4);
				vetor_r = conversor(resultado, 8);
			}
			else if (i >= 8 && j >= 8) {
				a = ctl_a;
				b = ctl_b;
				ctl_b++;
				resultado = a * b;
				vetor_a = conversor(a, 4);
				vetor_b = conversor(b, 4);
				vetor_r = conversor(resultado, 8);
			}
			strcpy(linha, vetor_a);
			strcat(linha, vetor_b);
			for (col_tab = 0; col_tab < 8; col_tab++) {
				entrada[linha_tab][col_tab] = linha[col_tab];
				saida[linha_tab][col_tab] = vetor_r[col_tab];
			}
			linha_tab++;
		}
		ctl_b = -8;
		if (i >= 8) { ctl_a++; }
	}
	free(vetor_a);
	free(vetor_b);
	free(vetor_r);

	//print das tabelas
	//inout.open("inout.txt");
	//for (i = 0; i < 256; i++) {
	//	for (j = 0; j < 8; j++) {
	//		inout << entrada[i][j];
	//	}
	//	inout << "\t";
	//	for (j = 0; j < 8; j++) {
	//		inout << saida[i][j];
	//	}
	//	inout << "\t" << i << endl;
	//}
	//inout.close();

	string pwlh = " PWL (0n 0 2n 0 2.01n 0.7 4n 0.7)";
	string pwhl = " PWL(0n 0.7 2n 0.7 2.01n 0 4n 0)";

	for (i = 0; i < 256; i++) {

		font.open("fontes.txt");
		measure.open("measure.txt");

		font << "*descricao de fontes" << endl;

		//percorre todas as posicoes de uma entrada
		for (j = 0; j < 8; j++) {
			if (entrada[i][j] == '0') {
				font << "vf" << j << " f" << j << "in gnd 0" << endl;
			}
			else {
				font << "vf" << j << " f" << j << "in gnd 0.7" << endl;
			}
		}

		//montagem de measures
		measure << "*measures" << endl << endl;
		measure << ".control" << endl;
		measure << "set noaskquit" << endl << endl;
		measure << "\t" << "run" << endl << endl;

		measure << "*-------------------Operacoes--------------------------" << endl << endl;
		measure << "\t" << "meas tran s7 find v(p7) at=4n" << endl;
		measure << "\t" << "meas tran s6 find v(p6) at=4n" << endl;
		measure << "\t" << "meas tran s5 find v(p5) at=4n" << endl;
		measure << "\t" << "meas tran s4 find v(p4) at=4n" << endl;
		measure << "\t" << "meas tran s3 find v(p3) at=4n" << endl;
		measure << "\t" << "meas tran s2 find v(p2) at=4n" << endl;
		measure << "\t" << "meas tran s1 find v(p1) at=4n" << endl;
		measure << "\t" << "meas tran s0 find v(p0) at=4n" << endl;
		measure << "\t" << "echo resultado_" << i << " $&s7 $&s6 $&s5 $&s4 $&s3 $&s2 $&s1 $&s0 >> resultado.txt" << endl << endl;

		//measure do consumo durante toda a transicao
		measure << endl << "*------------------Energia----------------------------" << endl << endl;
		measure << "\t" << "meas tran energia_" << i << " INTEG i(Vvdd) from=0n to=4n" << endl;
		measure << "\techo energia_" << i << " $&energia_" << i << " >> energia.txt" << endl << endl;

		measure << "quit" << endl;
		measure << ".endc" << endl;

		char arquivo[100] = "ngspice.exe base.txt";
		system(arquivo);
		font.close();
		measure.close();
	}
	cout << "FINISH!!" << endl;
	return 0;
}
