using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.IO;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Xml;

namespace Program
{
    class Program
    {

        static void Main(string[] args)
        {
            string arqEntrada = @"C:\Users\Douglas\Desktop\AMA2_30_LOW\BWR\resultado.txt";
            string arqSaida = @"C:\Users\Douglas\Desktop\AMA2_30_LOW\BWR\resultado[convert].txt";

            SaidaParaBinario(arqEntrada, arqSaida, true);

        }

        public static void CompareFiles(string filePath1, string filePath2)
        {   //lista das linhas diferentes
            List<int> differentLines = new List<int>();
            //linhas do arquivo
            string lineFile1 = string.Empty;
            string lineFile2 = string.Empty;
            //abrindo arquivo 1
            using (StreamReader readerFile1 = File.OpenText(filePath1))
            {//abrindo arquivo 2
                using (StreamReader readerFiel2 = File.OpenText(filePath2))
                {
                    int lineNumber = 1;
                    //enquanto nao é fim de arquivo
                    while (!readerFile1.EndOfStream || !readerFiel2.EndOfStream)
                    {
                        //ler uma linha
                        if (!readerFile1.EndOfStream)
                        {
                            lineFile1 = readerFile1.ReadLine();

                        }
                        if (!readerFiel2.EndOfStream)
                        {
                            lineFile2 = readerFiel2.ReadLine();
                        }

                        //se linhas diferentes escreve o numero da linha
                        if (lineFile1 != lineFile2)
                        {
                            differentLines.Add(lineNumber);
                        }

                        //limpa linhas
                        lineFile1 = string.Empty;
                        lineFile2 = string.Empty;
                        lineNumber++;
                    }
                }
            }
            //escreve quais sao as linhas diferentes 
            using (StreamWriter writer = new StreamWriter("C:\\Users\\Douglas\\Desktop\\dif.txt", true))
            {
                foreach (int item in differentLines)
                {
                    writer.WriteLine(item);
                }
            }
        }

        /// <summary></summary>
        /// <param name="filePath1">Caminho arquivo de entrada</param>
        /// <param name="outFilePath">Caminho do arquivo de saída</param>
        public static void SaidaParaBinario(String filePath1, String outFilePath, Boolean bComSinal = false)
        {
            try
            {
                if (File.Exists(filePath1))
                {
                    string lineFile1 = string.Empty;
                    Int32 saidaDecimal = 0;

                    try
                    {
                        //abre arquivo
                        using (StreamReader readerFile1 = File.OpenText(filePath1))
                        {
                            //ler linhas
                            while (!readerFile1.EndOfStream)
                            {
                                //linha do arquivo
                                lineFile1 = readerFile1.ReadLine();

                                //apaga espaços
                                lineFile1 = lineFile1.Trim();

                                //troca ponto por virgula
                                String linha = lineFile1.Replace('.', ',');

                                //separa a linha em partes
                                String[] vetorSaidas = linha.Split(' ');

                                try
                                {
                                    //escrever o arquivo com os valores convertidos em bin
                                    using (StreamWriter writer = new StreamWriter(outFilePath, true))
                                    {
                                        //armazenara a string final
                                        StringBuilder saidaEmBinario = new StringBuilder();

                                        //cada bit de uma saida
                                        foreach (String saida in vetorSaidas)
                                        {
                                            //se nao for branco, converte
                                            if (!String.IsNullOrWhiteSpace(saida))
                                            {
                                                Decimal valor = Convert.ToDecimal(saida);

                                                //se maior que 0.4 ou 0.23 = 1
                                                if (valor >= 0.23M)
                                                {
                                                    saidaEmBinario.Append("1");
                                                }
                                                else
                                                {
                                                    saidaEmBinario.Append("0");
                                                }
                                            }
                                        }

                                        //nao é nulo nem espaço em branco
                                        if (!String.IsNullOrWhiteSpace(saidaEmBinario.ToString()))
                                        {
                                            if (!bComSinal)
                                            {
                                                saidaDecimal = Convert.ToInt32(saidaEmBinario.ToString(), 2);
                                            }
                                            else
                                            {
                                                saidaDecimal = ConverteBinarioSinal(saidaEmBinario.ToString());
                                            }
                                            writer.WriteLine(saidaEmBinario.ToString() + "\t" + saidaDecimal);
                                        }
                                        else
                                        {
                                            //se for nulo ou branco
                                            writer.WriteLine(String.Empty.PadLeft(30, '-'));
                                        }
                                    }
                                }
                                catch (Exception ex)
                                {

                                    Console.Write("Erro na escrita:" + ex);
                                }

                                //limpa linha
                                lineFile1 = String.Empty;
                            }

                        }
                    }
                    catch (Exception ex)
                    {

                        Console.Write("Erro na leitura:" + ex);
                    }
                }
                else
                {
                    Console.WriteLine(" O arquivo " + filePath1 + "não foi localizado !");
                }

            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }

        /// <summary>Converte binário COM SINAL para decimal </summary>
        /// <param name="sBinario"></param>
        /// <param name="comSinal"></param>
        private static Int32 ConverteBinarioSinal(String sBinario)
        {
            Int32 dec = 0;

            //converter para decimal com sinal
            for (int i = 0; i < sBinario.Length; i++)
            {
                // we start with the least significant digit, and work our way to the left
                if (sBinario[sBinario.Length - i - 1] == '0')
                {
                    continue;
                }

                //quando i representa a posição mais significativa, soma o valor negativo
                if (i == sBinario.Length - 1)
                {
                    dec -= (Int32)Math.Pow(2, i);
                }
                else
                {
                    dec += (Int32)Math.Pow(2, i);
                }
            }
            return dec;
        }

        /// <summary></summary>
        /// <param name="filePath1">Caminho arquivo de entrada</param>
        /// <param name="outFilePath">Caminho do arquivo de saída</param>
        public static void ConversorDecimal(String filePath1, String outFilePath, Boolean bComSinal = false)
        {
            try
            {
                if (File.Exists(filePath1))
                {
                    string lineFile1 = string.Empty;
                    Int32 saidaDecimal = 0;

                    try
                    {
                        //abre arquivo
                        using (StreamReader readerFile1 = File.OpenText(filePath1))
                        {
                            //ler linhas
                            while (!readerFile1.EndOfStream)
                            {
                                //linha do arquivo
                                lineFile1 = readerFile1.ReadLine();

                                //elimina espaços
                                String linha = lineFile1.Trim();

                                try
                                {
                                    using (StreamWriter writer = new StreamWriter(outFilePath, true))
                                    {
                                        //nao é nulo nem espaço em branco
                                        if (!String.IsNullOrWhiteSpace(linha.ToString()))
                                        {
                                            if (!bComSinal)
                                            {
                                                saidaDecimal = Convert.ToInt32(linha.ToString(), 2);
                                            }
                                            else
                                            {
                                                saidaDecimal = ConverteBinarioSinal(linha.ToString());
                                            }
                                            writer.WriteLine(linha.ToString() + "\t" + saidaDecimal);
                                        }
                                        else
                                        {
                                            writer.WriteLine(String.Empty.PadLeft(30, '-'));
                                        }
                                    }
                                }
                                catch (Exception ex)
                                {

                                    Console.Write("Erro na escrita:" + ex);
                                }

                                //limpa linha
                                lineFile1 = String.Empty;
                            }

                        }

                    }
                    catch (Exception ex)
                    {

                        Console.Write("Erro na leitura:" + ex);
                    }
                }
                else
                {
                    Console.WriteLine(" O arquivo " + filePath1 + "não foi localizado !");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }

    }

}
