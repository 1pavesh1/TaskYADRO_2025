#include <iostream>
#include "include/ClassComputerClub.h"
using namespace std;

int main(int argc, const char* argv[])
{
    ComputerClub computer_club;

    // Если при запуске программы аргумент (текстовый файл) не был передан, то завершаем программу
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " <input_file>\n";
        exit(1);
    }

    // Читаем данные из файла
    computer_club.readFile(argv[1]);
    // Запускаем компьютерный клуб
    computer_club.Work();

    return 0;
}