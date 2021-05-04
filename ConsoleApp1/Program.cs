using System;

namespace BinaryTools
{
    class Program
    {
        static void Main(string[] args)
        {
            bool endApp = false;

            Console.WriteLine("Console Conversor");
            Console.WriteLine("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");

            while (!endApp)
            {
                Console.WriteLine("Escolha uma opção da seguinte lista: ");
                Console.WriteLine("\t1 - Converter fontes de saída para binário");
                Console.WriteLine("\t2 - Converter binários para decimal");
                Console.WriteLine("\t3 - Encerrar aplicação\n");

                int option;
                while (!Int32.TryParse(Console.ReadLine(), out option))
                {
                    Console.WriteLine("Opção inválida. Tente novamente.");
                }
                Console.WriteLine("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");

                switch (option)
                {
                    case 1:
                        Converter.SourceToBinary();
                        break;
                    case 2:
                        Converter.BinaryToDecimal();
                        break;
                    case 3:
                        endApp = true;
                        break;
                    default:
                        Console.WriteLine("Opção inválida. Tente novamente.");
                        break;
                }
                Console.WriteLine("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
            }

            return;
        }

    }
}
