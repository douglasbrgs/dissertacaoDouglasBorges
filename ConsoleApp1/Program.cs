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
            string arqEntrada = String.Empty;
            string arqSaida = String.Empty;
        }
        /// <summary>
        /// Compara arquivos linha a linha e gera um arquivo com os índices das linhas que diferem
        /// </summary>
        /// <param name="filePath1"></param>
        /// <param name="filePath2"></param>
        public static void CompareFiles(string filePath1, string filePath2)
        {
            List<int> differentLines = new List<int>();
            string lineFile1 = string.Empty;
            string lineFile2 = string.Empty;
            using (StreamReader readerFile1 = File.OpenText(filePath1))
            {
                using (StreamReader readerFiel2 = File.OpenText(filePath2))
                {
                    int lineNumber = 1;
                    while (!readerFile1.EndOfStream || !readerFiel2.EndOfStream)
                    {
                        if (!readerFile1.EndOfStream)
                        {
                            lineFile1 = readerFile1.ReadLine();

                        }
                        if (!readerFiel2.EndOfStream)
                        {
                            lineFile2 = readerFiel2.ReadLine();
                        }

                        if (lineFile1 != lineFile2)
                        {
                            differentLines.Add(lineNumber);
                        }

                        lineFile1 = string.Empty;
                        lineFile2 = string.Empty;
                        lineNumber++;
                    }
                }
            }
            using (StreamWriter writer = new StreamWriter("C:\\dif.txt", true))
            {
                foreach (int item in differentLines)
                {
                    writer.WriteLine(item);
                }
            }
        }

        /// <summary>Lê um arquivo com números decimais e converte cada linha em uma string de 0's e 1's</summary>
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
                        using (StreamReader readerFile1 = File.OpenText(filePath1))
                        {
                            while (!readerFile1.EndOfStream)
                            {
                                lineFile1 = readerFile1.ReadLine().Trim();
                                //troca ponto por virgula
                                String linha = lineFile1.Replace('.', ',');
                                String[] vetorSaidas = linha.Split(' ');

                                try
                                {
                                    using (StreamWriter writer = new StreamWriter(outFilePath, true))
                                    {
                                        StringBuilder saidaEmBinario = new StringBuilder();

                                        foreach (String saida in vetorSaidas)
                                        {
                                            if (!String.IsNullOrWhiteSpace(saida))
                                            {
                                                Decimal valor = Convert.ToDecimal(saida);

                                                //0.7 ou 0.23
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
                                            writer.WriteLine(String.Empty.PadLeft(30, '-'));
                                        }
                                    }
                                }
                                catch (Exception ex)
                                {

                                    Console.Write("Erro na escrita:" + ex);
                                }

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

        /// <summary>Converte binário COM SINAL para decimal</summary>
        /// <param name="sBinario"></param>
        /// <param name="comSinal"></param>
        /// <returns>Número inteiro</returns>
        private static Int32 ConverteBinarioSinal(String sBinario)
        {
            Int32 dec = 0;

            for (int i = 0; i < sBinario.Length; i++)
            {
                if (sBinario[sBinario.Length - i - 1] == '0')
                {
                    continue;
                }

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

        /// <summary> Lê um arquivo com binários e gera um arquivo com as conversões para decimal</summary>
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
                        using (StreamReader readerFile1 = File.OpenText(filePath1))
                        {
                            while (!readerFile1.EndOfStream)
                            {
                                lineFile1 = readerFile1.ReadLine();
                                String linha = lineFile1.Trim();

                                try
                                {
                                    using (StreamWriter writer = new StreamWriter(outFilePath, true))
                                    {
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
