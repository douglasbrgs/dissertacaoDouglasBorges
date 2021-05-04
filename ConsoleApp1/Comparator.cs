using System.Collections.Generic;
using System.IO;

namespace BinaryTools
{
    class Comparator
    {
        /// <summary>Compare two text files line by line.</summary>
        /// <param name="filePath1"></param>
        /// <param name="filePath2"></param>
        public static void CompareFiles(string filePath1, string filePath2)
        {
            List<int> differentLines = new List<int>();
            string fileLine = string.Empty;
            string fileLine2 = string.Empty;

            using (StreamReader readerFile = File.OpenText(filePath1))
            {
                using (StreamReader readerFiel2 = File.OpenText(filePath2))
                {
                    int lineNumber = 1;
                    while (!readerFile.EndOfStream || !readerFiel2.EndOfStream)
                    {
                        if (!readerFile.EndOfStream)
                        {
                            fileLine = readerFile.ReadLine();

                        }
                        if (!readerFiel2.EndOfStream)
                        {
                            fileLine2 = readerFiel2.ReadLine();
                        }

                        if (fileLine != fileLine2)
                        {
                            differentLines.Add(lineNumber);
                        }

                        fileLine = string.Empty;
                        fileLine2 = string.Empty;
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
    }
}
