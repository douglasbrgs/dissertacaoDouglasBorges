using System;
using System.IO;
using System.Text;

namespace BinaryTools
{
    class Converter
    {
        /// <summary> Generate a file with binary numbers from a file with voltage sources.</summary>
        public static void SourceToBinary()
        {
            try
            {
                string outFilePath = Environment.GetFolderPath(Environment.SpecialFolder.DesktopDirectory);

                Console.Write("Digite o caminho do arquivo de entrada: ");
                string sourcePath = Console.ReadLine();

                Console.Write("Digite a voltagem mínima para sinal alto (1 lógico): ");
                Decimal minValueForHigh;
                while (!Decimal.TryParse(Console.ReadLine().Replace(".", ","), out minValueForHigh))
                {
                    Console.WriteLine("Valor inválido.");
                    Console.Write("Digite a voltagem mínima para sinal alto (1 lógico): ");
                }

                Console.WriteLine("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");

                if (File.Exists(sourcePath))
                {
                    using (StreamReader readerFile = File.OpenText(sourcePath))
                    {
                        string fileLine;
                        string currentLine;
                        string[] sourcesVector;
                        StringBuilder binaryLineOutput;

                        using (StreamWriter writer = new StreamWriter(Path.Combine(outFilePath, "binaryout.txt"), false))
                        {
                            while (!readerFile.EndOfStream)
                            {
                                fileLine = readerFile.ReadLine().Trim();
                                currentLine = fileLine.Replace('.', ',');
                                sourcesVector = currentLine.Split(' ');

                                binaryLineOutput = new StringBuilder();

                                foreach (string sourceValue in sourcesVector)
                                {
                                    if (!string.IsNullOrWhiteSpace(sourceValue))
                                    {
                                        Decimal decimalSourceValue = Convert.ToDecimal(sourceValue);

                                        if (decimalSourceValue >= minValueForHigh)
                                        {
                                            binaryLineOutput.Append("1");
                                        }
                                        else
                                        {
                                            binaryLineOutput.Append("0");
                                        }
                                    }
                                }

                                if (!string.IsNullOrWhiteSpace(binaryLineOutput.ToString()))
                                {
                                    writer.WriteLine(binaryLineOutput.ToString());
                                }
                                else
                                {
                                    writer.WriteLine(string.Empty.PadRight(30, '-'));
                                }
                            }
                        }
                    }
                    Console.WriteLine("Arquivo gerado com sucesso.");
                }
                else
                {
                    Console.WriteLine("O arquivo " + sourcePath + " não foi localizado.");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Exceção: " + ex.Message);
            }
        }

        /// <summary>generates a file with decimal numbers from a file with binary numbers.</summary>
        public static void BinaryToDecimal()
        {
            try
            {
                string outFilePath = Environment.GetFolderPath(Environment.SpecialFolder.DesktopDirectory);

                Console.Write("Digite o caminho do arquivo de entrada: ");
                string filePath = Console.ReadLine();

                string twoComplement = string.Empty;
                do
                {
                    Console.Write("Utilizar binários em complemento de 2? (s/n): ");
                    twoComplement = Console.ReadLine();

                } while (!twoComplement.Equals("s") && !twoComplement.Equals("n"));

                Console.WriteLine("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");

                if (File.Exists(filePath))
                {
                    using (StreamReader readerFile = File.OpenText(filePath))
                    {
                        long decimalOutput;
                        string fileLine = String.Empty;

                        using (StreamWriter writer = new StreamWriter(Path.Combine(outFilePath, "decimalout.txt"), false))
                        {
                            while (!readerFile.EndOfStream)
                            {
                                fileLine = readerFile.ReadLine().Trim();

                                if (!string.IsNullOrWhiteSpace(fileLine.ToString()))
                                {
                                    if (twoComplement.Equals("s"))
                                    {
                                        decimalOutput = ConvertSignedBinary(fileLine.ToString());
                                    }
                                    else
                                    {
                                        decimalOutput = Convert.ToInt64(fileLine.ToString(), 2);
                                    }
                                    writer.WriteLine(decimalOutput);
                                }
                                else
                                {
                                    writer.WriteLine(string.Empty.PadLeft(30, '-'));
                                }
                            }
                        }
                    }
                    Console.WriteLine("Arquivo gerado com sucesso.");
                }
                else
                {
                    Console.WriteLine(" O arquivo " + filePath + " não foi localizado.");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Exceção: " + ex.Message);
            }
        }

        /// <summary> Converts binary to signed decimal.</summary>
        /// <param name="sBinario"></param>
        /// <returns></returns>
        private static Int32 ConvertSignedBinary(string sBinario)
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
    }
}
