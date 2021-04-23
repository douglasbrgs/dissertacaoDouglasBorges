#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<vector>
#include<iostream>
#include<fstream>
#include<string>
#include<unistd.h>

using namespace std;

class Arco{
	public:
		int numarc;
		int linha1, linha2;
		int riseout,risein;
		int difin,difout;
		char saida1, saida2;
		
};

char* bindec(int num,int tam){         /*RECEBE UM INTEIRO E RETORNA UMA STRING CONTENDO O BINARIO CORRESPONDENTE*/
	int x;
	int i = (tam-1);
	int dimension = tam+1;
	char* bit = (char*)malloc(sizeof(char)*dimension);

 	for(x=0;x<dimension;x++){
	 	bit[x] = '0';
	 }

 	while(num>0){

	 	if((num%2)==0) bit[i]= '0';

	 	else bit[i] = '1';

	 	i = i-1;
	 	num = num/2;
	}
 	bit[tam] = '\0';
  	return bit;

}

char* conversor(int num, int tam){       /*VERIFICA SE O NUMERO EH POSITIVO OU NEGATIVO E CHAMA BINDEC*/
	int numAux,tamAux,dif;

	if(num>0){                                 /*SE NUMERO POSITIVO CHAMA BINDEC*/
			return bindec(num,tam);
	}
	else{                                        /*SE NUMERO NEGATIVO CHAMA BINDEC COM O POSITIVO DE SEQUENCIA EQUIVALENTE*/
			numAux = num*(-1);
			if(tam==4){
					tamAux = 8;
			}
			else{
					tamAux = 128;
			}

			dif = tamAux - numAux;
			num = tamAux + dif;

			return bindec(num,tam);

	}
}

int main(){
	int ctl_a =-8, ctl_b = -8;        //variaveis de controle para realizar as multiplicacoes
	int a=0,b=0,resultado=0,cont=0;    //variaveis de controle para realizar as multiplicacoes
   	int i,j,k,dif =0;                          //cursores
   	int col_tab=0;
	int linha_tab=0;
	char *vetor_a,*vetor_b,*vetor_r;
   	char entrada[256][8],saida[256][8],linha[8];
   	
	vector<Arco> lista;
   	Arco meuArc;
   	
  	
	ofstream font,measure;
	   	
	for(i = 0; i<=15; i++){                       /*256 COMBINACOES */
		for(j = 0; j<=15; j++){
			if(i<8 && j<8){
				a = i;
				b = j;
				resultado = a * b;

				vetor_a = conversor(a,4);
				vetor_b = conversor(b,4);
				vetor_r = conversor(resultado,8);

			}
			else if(i<8 && j>=8){
				a = i;
				b = ctl_b;
				ctl_b++;
				resultado = a * b;

				vetor_a = conversor(a,4);
				vetor_b = conversor(b,4);
				vetor_r = conversor(resultado,8);
								
			}
			else if(i>=8 && j<8){
				a = ctl_a;
				b = j;
				resultado = a * b;
				vetor_a = conversor(a,4);
				vetor_b = conversor(b,4);
				vetor_r = conversor(resultado,8);
			}
			else if(i>=8 && j>=8){
				a = ctl_a;
				b = ctl_b;
				ctl_b++;
				resultado = a * b;

				vetor_a = conversor(a,4);
				vetor_b = conversor(b,4);
				vetor_r = conversor(resultado,8);
			}
			strcpy(linha,vetor_a);
			strcat(linha,vetor_b);
			
			for(col_tab=0;col_tab<8;col_tab++){
				entrada[linha_tab][col_tab] = linha[col_tab];      //COLOCANDO OS VALORES NAS MATRIZES
				saida[linha_tab][col_tab] = vetor_r[col_tab];
			}
			linha_tab++;
		}
		ctl_b = -8;
		if(i>=8){ctl_a++;}
	}
	free(vetor_a);
	free(vetor_b);
	free(vetor_r);
			
	 //ofstream teste;
	//teste.open("testando.txt");	
	for(i = 0; i<256; i++){                               //COMPARANDO TODAS AS ENTRADAS
 		for(j = 0; j<256; j++){
			if(i!=j){
			
				for(k=0; k<8; k++){
					if(entrada[i][k] != entrada[j][k]) {        //SE UMA POSICAO DA LINHA I FOR DIFERENTE DA LINHA J  INCREMENTA CONT
						cont++;
						dif = k;
						
					 }			 	 												
				}
			}
											
			if(cont==1){                                     //SE O CONTADOR FOR 1, OU SEJA, 1 BIT DE DIFERENCA COMPARA AS RESPECTIVAS SAIDAS(I,J)
				
					for(k=0;k<8;k++){
							if(saida[i][k] != saida[j][k]){    		
								
									meuArc.linha1 = i;                       //ATRIBUI OS VALORES DO ARCO A UMA CLASSE E COLOCA ELA NO VECTOR LISTA                   
									meuArc.linha2 = j;
									
									meuArc.difin = dif;
									meuArc.difout = k;
																	
									if(entrada[i][dif] =='0'){
										meuArc.risein = 1;
									
									}
									else meuArc.risein = 0;
									
									meuArc.numarc = (k-7)*(-1);
									//meuArc.saida1 = saida[i][k];
									//meuArc.saida2 = saida[j][k];
									
									if(saida[i][k] =='0'){
										meuArc.riseout = 1;
									}
									else meuArc.riseout = 0;														
									
									lista.push_back(meuArc);
									
							
							}
					}
	
			}
			cont = 0;			
		}
	}
	
	//for(i=0; i < lista.size();i++){
	//teste << lista[i].linha1 << "_" << lista[i].linha2 <<"_" << lista[i].numarc << "_" << lista[i].risein << "_" << lista[i].riseout << endl;
	//}
	
	string pwlh = " PWL (0n 0 2n 0 2.01n 0.7 4n 0.7)";
	string pwhl = " PWL(0n 0.7 2n 0.7 2.01n 0 4n 0)";
	
	
	
	for(i = 0;i<lista.size();i++){   //pra todos os arcos na lista		
			font.open("fontes.txt"); 
			measure.open("measure.txt");
					
			
			font << "*descricao de fontes"<< endl;
			for(j=0;j<8;j++){
					if(j == lista[i].difin){                                               //escevendo os valores das fontes
							if(lista[i].risein ==1) font << "vf" << j << " f" << j << "in gnd" << pwlh << endl;
							else font << "vf" << j << " f" << j << "in gnd" << pwhl << endl;
					}
					else{
							if (entrada [lista[i].linha1][j] == '0') font << "vf" << j << " f" << j << "in gnd 0" << endl;
							else font << "vf" << j << " f" << j << "in gnd 0.7" << endl;	
					}		
			}
			
			measure << "*measures\n\n.control" << endl << "set noaskquit" <<"\n\n\trun"<< endl << endl;
			
			measure << "*-------------------Atrasos---------------------------" << endl << endl;
			
			if(lista[i].risein == 1 && lista[i].riseout == 1){
				measure << "\tmeas tran TP_LH_" << lista[i].linha1 << "_" << lista[i].linha2 << " trig v(f" << lista[i].difin << ") val = 0.35 rise=1";
				measure << " targ v(p" << lista[i].numarc << ") val = 0.35 rise=1" << endl;
				measure << "\techo TP_LH_" << lista[i].linha1 << "_" << lista[i].linha2 << " $&TP_LH_" << lista[i].linha1 << "_"<<lista[i].linha2; 
				measure << " >> Atrasos.txt"<<endl<<endl;
			}
			else if(lista[i].risein == 0 && lista[i].riseout == 0){
				measure << "\tmeas tran TP_HL_" << lista[i].linha1 << "_" << lista[i].linha2 << " trig v(f" << lista[i].difin << ") val = 0.35 fall=1";
				measure << " targ v(p" << lista[i].numarc << ") val = 0.35 fall=1" << endl;
				measure << "\techo TP_HL_" << lista[i].linha1 << "_" << lista[i].linha2 << " $&TP_HL_" << lista[i].linha1 << "_"<<lista[i].linha2;
				measure << " >> Atrasos.txt"<<endl<<endl;
			}
			else if(lista[i].risein == 1 && lista[i].riseout == 0){
				measure << "\tmeas tran TP_HL_" << lista[i].linha1 << "_" << lista[i].linha2 << " trig v(f" << lista[i].difin << ") val = 0.35 rise=1";
				measure << " targ v(p" << lista[i].numarc << ") val = 0.35 fall=1" << endl;
				measure << "\techo TP_HL_" << lista[i].linha1 << "_" << lista[i].linha2 << " $&TP_HL_" << lista[i].linha1 << "_"<<lista[i].linha2; 
				measure << " >> Atrasos.txt"<<endl<<endl;
			}
			else if(lista[i].risein == 0 && lista[i].riseout == 1){
				measure << "\tmeas tran TP_LH_" << lista[i].linha1 << "_" << lista[i].linha2 << " trig v(f" << lista[i].difin << ") val = 0.35 fall=1";
				measure << " targ v(p" << lista[i].numarc << ") val = 0.35 rise=1" << endl;
				measure << "\techo TP_LH_" << lista[i].linha1 << "_" << lista[i].linha2 << " $&TP_LH_" << lista[i].linha1 << "_"<<lista[i].linha2;
				measure << " >> Atrasos.txt"<<endl<<endl;
			}
			
			measure << endl << "*------------------Energia----------------------------" << endl << endl;
			
			measure << "\tmeas tran energia_" << lista[i].linha1 << "_" <<lista[i].linha2 << " INTEG i(Vvdd) from=0n to=4n"<<endl;
			measure <<"\techo energia_" << lista[i].linha1 << "_" << lista[i].linha2 << " $&energia_" << lista[i].linha1 <<"_" <<lista[i].linha2 << " >> energia.txt"<<endl<<endl;
			
			
			 
			 
			measure << "\n\n"<<"quit"<<"\n.endc" << endl;
			 
			char arquivo[100] = "ngspice_con base.txt";	
			system(arquivo);
			font.close();
			measure.close();
	}
	cout << "FINISH!!" << endl;
	return 0;
}
	
			

		
			
			
		
			
		









