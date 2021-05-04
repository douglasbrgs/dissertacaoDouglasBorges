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

                Console.Write("Digite o valor mínimo de voltagem para high: ");
                Decimal minValueForHigh;
                while (!Decimal.TryParse(Console.ReadLine().Replace(".", ","), out minValueForHigh))
                {
                    Console.Write("Valor inválido.");
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

                        while (!readerFile.EndOfStream)
                        {
                            fileLine = readerFile.ReadLine().Trim();
                            currentLine = fileLine.Replace('.', ',');
                            sourcesVector = currentLine.Split(' ');

                            using (StreamWriter writer = new StreamWriter(Path.Combine(outFilePath, "output.txt"), false))
                            {
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
                Boolean signedBinary = false;

                Console.Write("Digite o caminho do arquivo de entrada: ");
                string filePath = Console.ReadLine();

                string option = string.Empty;
                do
                {
                    Console.WriteLine("Utilizar binários em complemento de 2?");
                    option = Console.ReadLine();

                } while (!option.Equals("s") && !option.Equals("n"));

                Console.WriteLine("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");

                if (File.Exists(filePath))
                {
                    using (StreamReader readerFile = File.OpenText(filePath))
                    {
                        long decimalOutput;
                        string fileLine = String.Empty;

                        while (!readerFile.EndOfStream)
                        {
                            fileLine = readerFile.ReadLine().Trim();

                            using (StreamWriter writer = new StreamWriter(outFilePath, true))
                            {
                                if (!string.IsNullOrWhiteSpace(fileLine.ToString()))
                                {
                                    if (!signedBinary)
                                    {
                                        decimalOutput = Convert.ToInt64(fileLine.ToString(), 2);
                                    }
                                    else
                                    {
                                        decimalOutput = ConverteBinarioSinal(fileLine.ToString());
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
                }
                else
                {
                    Console.WriteLine(" O arquivo " + filePath + "não foi localizado.");
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
        private static Int32 ConverteBinarioSinal(string sBinario)
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
