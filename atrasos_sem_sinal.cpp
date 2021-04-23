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
		int numarc; //numero da saida influenciada
		int linha1, linha2; //configuracoes de entrada que formam um arco
		int riseout,risein; //subida de saida, subida de entrada
		int difin,difout; //bit diferente na entrada, bit diferente na saida
		char saida1, saida2;
};

//conversor binario
char* bindec(int num,int tam){         
	int x;
	int i = (tam-1);
	int dimension = tam+1;
	//aloca espaco para tamanho+1
	char* bit = (char*)malloc(sizeof(char)*dimension);

	//preenche vetor com zeros		
 	for(x=0;x<dimension;x++){
	 	bit[x] = '0';
	 }
	//conversao
 	while(num>0){

	 	if((num%2)==0) bit[i]= '0';

	 	else bit[i] = '1';

	 	i = i-1;
	 	num = num/2;
	}
 	bit[tam] = '\0';
  	return bit;

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
				//pega um par de numeros, faz a multiplicacao deles
				//converte os numeros e o resultado para binario			
				a = i;
				b = j;
				resultado = a * b;

				vetor_a = bindec(a,4);
				vetor_b = bindec(b,4);
				vetor_r = bindec(resultado,8);

			//coloca os binarios das duas entradas em um vetor
			strcpy(linha,vetor_a);
			strcat(linha,vetor_b);
			
			for(col_tab=0;col_tab<8;col_tab++){
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
		//COMPARANDO TODAS AS ENTRADAS						
	for(i = 0; i<256; i++){                               
 		for(j = 0; j<256; j++){
			if(i!=j){
				//SE UMA POSICAO DA LINHA I FOR DIFERENTE DA LINHA J  INCREMENTA CONT E SALVA INDICE DESTA LINHA
				for(k=0; k<8; k++){
					if(entrada[i][k] != entrada[j][k]) {        
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
		
	//ondas para colocar na fonte de transicao (4-8)
	//tensao max 0.7
		string pwlh = " PWL (0n 0 2n 0 2.01n 0.7 4n 0.7)";
	string pwhl = " PWL(0n 0.7 2n 0.7 2.01n 0 4n 0)";
	
	for(i = 0;i<lista.size();i++){   //pra todos os arcos na lista		
			
			//abri os arquivos de fonte e measure
			font.open("fontes.txt"); 
			measure.open("measure.txt");
							
			font << "*descricao de fontes"<< endl;
			
			//percorre todas as posicoes de uma entrada
			for(j=0;j<8;j++){
					
					//se a posicao j for o bit de transicao de um arco
					if(j == lista[i].difin){
							//monta fonte com risein ou com riseout                                             
							if(lista[i].risein ==1) font << "vf" << j << " f" << j << "in gnd" << pwlh << endl;
							else font << "vf" << j << " f" << j << "in gnd" << pwhl << endl;
					}
					//se nao for bit de transicao coloca fonte normal
					//fonte com 0V ou com 0.7V
					else{
							if (entrada [lista[i].linha1][j] == '0') font << "vf" << j << " f" << j << "in gnd 0" << endl;
							else font << "vf" << j << " f" << j << "in gnd 0.7" << endl;	
					}		
			}
			//montagem de measures
			measure << "*measures\n\n.control" << endl << "set noaskquit" <<"\n\n\trun"<< endl << endl;			
			measure << "*-------------------Atrasos---------------------------" << endl << endl;
			
			//subida na entrada subida na saida
			//montagem de measures linha1 e linha2 sao as configuracoes de entrada que compoem um arco
			//difin é o bit diferente entre elas, numArc é a saida influenciada pela transicao
			if(lista[i].risein == 1 && lista[i].riseout == 1){
				measure << "\tmeas tran TP_LH_" << lista[i].linha1 << "_" << lista[i].linha2 << " trig v(f" << lista[i].difin << ") val = 0.35 rise=1";
				measure << " targ v(p" << lista[i].numarc << ") val = 0.35 rise=1" << endl;
				measure << "\techo TP_LH_" << lista[i].linha1 << "_" << lista[i].linha2 << " $&TP_LH_" << lista[i].linha1 << "_"<<lista[i].linha2; 
				measure << " >> Atrasos.txt"<<endl<<endl;
			}
			//descida na entrada descida na saida
			else if(lista[i].risein == 0 && lista[i].riseout == 0){
				measure << "\tmeas tran TP_HL_" << lista[i].linha1 << "_" << lista[i].linha2 << " trig v(f" << lista[i].difin << ") val = 0.35 fall=1";
				measure << " targ v(p" << lista[i].numarc << ") val = 0.35 fall=1" << endl;
				measure << "\techo TP_HL_" << lista[i].linha1 << "_" << lista[i].linha2 << " $&TP_HL_" << lista[i].linha1 << "_"<<lista[i].linha2;
				measure << " >> Atrasos.txt"<<endl<<endl;
			}
			//subida na entrada, descida na saida
			else if(lista[i].risein == 1 && lista[i].riseout == 0){
				measure << "\tmeas tran TP_HL_" << lista[i].linha1 << "_" << lista[i].linha2 << " trig v(f" << lista[i].difin << ") val = 0.35 rise=1";
				measure << " targ v(p" << lista[i].numarc << ") val = 0.35 fall=1" << endl;
				measure << "\techo TP_HL_" << lista[i].linha1 << "_" << lista[i].linha2 << " $&TP_HL_" << lista[i].linha1 << "_"<<lista[i].linha2; 
				measure << " >> Atrasos.txt"<<endl<<endl;
			}
			//descida na entrada, subida na saida
			else if(lista[i].risein == 0 && lista[i].riseout == 1){
				measure << "\tmeas tran TP_LH_" << lista[i].linha1 << "_" << lista[i].linha2 << " trig v(f" << lista[i].difin << ") val = 0.35 fall=1";
				measure << " targ v(p" << lista[i].numarc << ") val = 0.35 rise=1" << endl;
				measure << "\techo TP_LH_" << lista[i].linha1 << "_" << lista[i].linha2 << " $&TP_LH_" << lista[i].linha1 << "_"<<lista[i].linha2;
				measure << " >> Atrasos.txt"<<endl<<endl;
			}
			
			//measure do consumo durante toda a transicao
			measure << endl << "*------------------Energia----------------------------" << endl << endl;
			
			measure << "\tmeas tran energia_" << lista[i].linha1 << "_" <<lista[i].linha2 << " INTEG i(Vvdd) from=0n to=4n"<<endl;
			measure <<"\techo energia_" << lista[i].linha1 << "_" << lista[i].linha2 << " $&energia_" << lista[i].linha1 <<"_" <<lista[i].linha2 << " >> energia.txt"<<endl<<endl;
			 
			measure << "\n\n"<<"quit"<<"\n.endc" << endl;
			
			//invocacao do ngspice, mudar o caminho 
			char arquivo[100]= "ngspice_con base.txt";
			system(arquivo);
			//fechando arquivos
			font.close();
			measure.close();
	}
	cout << "FINISH!!" << endl;
	return 0;
}
	
