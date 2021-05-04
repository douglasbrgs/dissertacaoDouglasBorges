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
	int numarc; //numero da saida influenciada
	int linha1, linha2; //configuracoes de entrada que formam um arco
	int riseout, risein; //subida de saida, subida de entrada
	int difin, difout; //bit diferente na entrada, bit diferente na saida
	char saida1, saida2;
};

//conversor binario
char* bindec(int num, int tam) {
	int x;
	int i = (tam - 1);
	int dimension = tam + 1;
	//aloca espaco para tamanho+1
	char* bit = (char*)malloc(sizeof(char) * dimension);

	//preenche vetor com zeros		
	for (x = 0; x < dimension; x++) {
		bit[x] = '0';
	}
	//conversao
	while (num > 0) {

		if ((num % 2) == 0) bit[i] = '0';

		else bit[i] = '1';

		i = i - 1;
		num = num / 2;
	}
	bit[tam] = '\0';
	return bit;

}

int main() {
	//int ctl_a = -8, ctl_b = -8;        //variaveis de controle para realizar as multiplicacoes
	int a = 0, b = 0, resultado = 0; //variaveis de controle para realizar as multiplicacoes
	int i, j, k;                        //cursores
	int col_tab = 0;
	int linha_tab = 0;
	char* vetor_a, * vetor_b, * vetor_r;
	char entrada[256][8], saida[256][8], linha[8];

	ofstream font, measure, inout;

	for (i = 0; i <= 15; i++) {                       /*256 COMBINACOES */
		for (j = 0; j <= 15; j++) {
			//pega um par de numeros, faz a multiplicacao deles
			//converte os numeros e o resultado para binario			
			a = i;
			b = j;
			resultado = a * b;

			vetor_a = bindec(a, 4);
			vetor_b = bindec(b, 4);
			vetor_r = bindec(resultado, 8);

			//coloca os binarios das duas entradas em um vetor
			strcpy(linha, vetor_a);
			strcat(linha, vetor_b);

			for (col_tab = 0; col_tab < 8; col_tab++) {
				//ccada linha da matriz guarda a concatenacao das duas entradas
				entrada[linha_tab][col_tab] = linha[col_tab];      //COLOCANDO OS VALORES NAS MATRIZES
				//guarda o binario correspondente ao resultado
				saida[linha_tab][col_tab] = vetor_r[col_tab];
			}
			linha_tab++;
		}
		//		ctl_b = -8;
		//		if(i>=8){ctl_a++;}
	}
	//libera a memoria ocupada pelos vetores
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

	//ondas para colocar na fonte de transicao (4-8)
	//tensao max 0.7
	string pwlh = " PWL (0n 0 2n 0 2.01n 0.7 4n 0.7)";
	string pwhl = " PWL(0n 0.7 2n 0.7 2.01n 0 4n 0)";

	for (i = 0; i < 256; i++) {   //para todas as entradas possiveis

		//abri os arquivos de fonte e measure
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

		//invocacao do ngspice, mudar o caminho 
		char arquivo[100] = "ngspice.exe base.txt";
		system(arquivo);
		//fechando arquivos
		font.close();
		measure.close();
	}
	cout << "FINISH!!" << endl;
	return 0;
}

