#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <map>
#include "Enum.h"
#include "Struct.h"
#include "ManagerTime.h"
#include "ManagerValidate.h"
using namespace std;

// Класс ComputerClub (компьютерный клуб) в котором реализована основная работа клуба
class ComputerClub
{
public:
    // Чтение файла
    void readFile(const string& name_file)
    {
        file.open(name_file);

        // Если файл не открывается
        if (!file)
        {
            cout << "Error open file !\n";
            exit(1);
        }

        // Если файл пуст
        if (file.peek() == ifstream::traits_type::eof())
        {
            cout << "File is empty !\n";
            exit(1);
        }

        // Чтение строк из файла
        while (getline(file, read_line))
        {
            // В зависимости от строчки выполняется соотвествующая часть кода
            ++count_line;
            switch (count_line)
            {
            case 1:
                // Проверка корректности первой строки <количество столов в компьютерном клубе>
                if (!manager_validate.checkNumber(read_line))
                {
                    printErrorStr(read_line);
                }
                count_tables = stoi(read_line);
                break;
            case 2:
                // Проверка корректности второй строки <время начала работы> <время окончания работы>
                if (!manager_validate.checkTime(read_line))
                {
                    printErrorStr(read_line);
                }
                time = read_line;
                break;
            case 3:
                // Проверка корректности третьей строки <стоимость часа в компьютерном клубе>
                if (!manager_validate.checkNumber(read_line))
                {
                    printErrorStr(read_line);
                }
                cost_hour = stoi(read_line);
                break;
            default:
                // Проверка следующих строчек файла которые являются входящими событиями
                vector_events.push_back(addEvent(read_line, count_tables));
                break;
            }
        }

        // Разделение времени формата XX:XX XX:XX на время открытия и время закрытия клуба
        time_open = manager_time.splitTime(time, true);
        time_close = manager_time.splitTime(time, false);

        // Закрываем файл после чтения
        file.close();
    }

    // Функция заставляющая клуб работать
    void Work()
    {
        printInfWork();
        endWorkDay();
        printInfRevenue();
    }
private:
    // Имитация основной работы клуба (обработка всех входящих событий)
    void printInfWork()
    {
        vector_tables.resize(count_tables + 1);
        cout << time_open << "\n";
        // Обработка всех событий
        for (const event& element_event : vector_events)
        {
            cout << element_event.time_event << " " << element_event.id_event << " " << element_event.name_client << " ";

            // Если номер стола не равен нулю, т.е он участвует в событии выводим его
            if (element_event.number_table != 0)
            {
                cout << element_event.number_table << "\n";
            }
            else
            {
                cout << "\n";
            }

            // Обработка идентификаторов событий
            switch (element_event.id_event)
            {
            case 1:
            {
                // Проверка работы клуба, если клиент пришел раньше то ошибка NotOpenYet, если же клиент
                // уже в компьютерном клубе, то ошибка YouShallNotPass, в противном случае
                // назначаем стол клиенту 0, т.е. он ещё не занял стол но находится в клубе
                if (!manager_validate.isClubOpen(element_event.time_event, time_open, time_close))
                {
                    printEvent(element_event, ErrorsType::NotOpenYet);
                    continue;
                }
                else if (clients.count(element_event.name_client))
                {
                    printEvent(element_event, ErrorsType::YouShallNotPass);
                }
                else
                {
                    clients[element_event.name_client] = 0;
                }
                break;
            }
            case 2:
            {
                // Если клиент не в компьютерном клубе, то генерируется ошибка ClientUnknown
                // Если стол занят (окупирован), то генерируется ошибка PlaceIsBusy
                // В противном случае клиент занимает стол
                if (!clients.count(element_event.name_client))
                {
                    printEvent(element_event, ErrorsType::ClientUnknown);
                    break;
                }
                else if (vector_tables[element_event.number_table].is_occupied)
                {
                    printEvent(element_event, ErrorsType::PlaceIsBusy);
                    break;
                }
                else
                {
                    // Если клиент захотел поменять стол, то рассчитываем выручку и время проведенное за столом
                    if (clients[element_event.name_client] != 0)
                    {
                        table_num = clients[element_event.name_client];
                        minutes_table = manager_time.tableOccupiedMinutes(vector_tables[table_num].occupied_since, element_event.time_event);
                        vector_tables[table_num].total_minutes += minutes_table;
                        vector_tables[table_num].revenue += revenueTable(minutes_table);
                    }
                    clients[element_event.name_client] = element_event.number_table;
                    vector_tables[element_event.number_table].is_occupied = true;
                    vector_tables[element_event.number_table].client_name = element_event.name_client;
                    vector_tables[element_event.number_table].occupied_since = element_event.time_event;

                }
                break;
            }
            case 3:
            {
                // Проверяем существует ли свободное место в массиве столов
                for (unsigned short int i = 1; i <= count_tables; i++)
                {
                    if (!vector_tables[i].is_occupied)
                    {
                        is_table_free = true;
                        break;
                    }
                }
                // Если в клубе есть свободные столы, то генерируется ошибка "ICanWaitNoLonger!"
                // Если размер очереди (queue_waiting) превышает или равен количеству столов, то удаляем клиента из очереди
                // и генерируем исходящее событие 11, что клиент ушел
                // В противном случае помещаем клиента в очередь
                if (is_table_free)
                {
                    printEvent(element_event, ErrorsType::ICanWaitNoLonger);
                }
                else if (queue_waiting.size() >= count_tables)
                {
                    cout << element_event.time_event << " 11 " << element_event.name_client << "\n";
                    clients.erase(element_event.name_client);
                }
                else
                {
                    queue_waiting.push(element_event.name_client);
                }
                is_table_free = false;
                break;
            }
            case 4:
            {
                // Если клиент не в компьютерном клубе, то генерируется ошибка ClientUnknown
                if (!clients.count(element_event.name_client))
                {
                    printEvent(element_event, ErrorsType::ClientUnknown);
                    break;
                }
                // Запоминаем номер стола (table_num) за которым сидел клиент
                table_num = clients[element_event.name_client];
                // Если он не равен нулю, то сразу рассчитываем выручку со стола и
                // сажаем последнего в очереди клиента, после посадки удаляем его из очереди и
                // генерируем исходящее событие 12 для первого клиента в очереди при освобождении любого стола
                // В противном случае если очереди нет, то обращаемся к элементу вектора (т.е. к столу) и присваиваем булевой
                // переменной значений false, т.е. стол не занят
                if (table_num != 0)
                {
                    minutes_table = manager_time.tableOccupiedMinutes(vector_tables[table_num].occupied_since, element_event.time_event);
                    vector_tables[table_num].total_minutes += minutes_table;
                    vector_tables[table_num].revenue += revenueTable(minutes_table);
                    if (!queue_waiting.empty())
                    {
                        string next_client = queue_waiting.front();
                        queue_waiting.pop();
                        clients[next_client] = table_num;
                        vector_tables[table_num].is_occupied = true;
                        vector_tables[table_num].client_name = next_client;
                        vector_tables[table_num].occupied_since = element_event.time_event;
                        cout << element_event.time_event << " 12 " << next_client << " " << table_num << "\n";
                    }
                    else
                    {
                        vector_tables[table_num].is_occupied = false;
                    }
                }
                // Удаляем клиента из клуба (клиент вышел)
                clients.erase(element_event.name_client);
                break;
            }
            default:
                break;
            }
        }
    }

    // Вывод выручки с каждого стола и времени которое было проведено за столом
    void printInfRevenue()
    {
        cout << time_close << "\n";
        for (unsigned short int i = 1; i <= count_tables; i++)
        {
            cout << i << " " << vector_tables[i].revenue << " " << manager_time.timeToString(vector_tables[i].total_minutes) << "\n";
        }
    }

    // Вывод события 11 и подсчет выручки со столов для тех клиентов кто не успел выйти с компьютерного клуба, а
    // так же вывод всех клиентов в алфавитном порядке, которые остались в клубе (конец рабочего дня клуба)
    void endWorkDay()
    {
        if (!clients.empty())
        {
            cout << time_close << " 11 ";
            for (const auto& cleint : clients)
            {
                unsigned short int minutes_table;
                // Если клиент был в клубе и при этом не успел или не смог занять стол
                if (cleint.second == 0)
                {
                    minutes_table = 0;
                }
                else
                {
                    minutes_table = manager_time.tableOccupiedMinutes(vector_tables[cleint.second].occupied_since, time_close);
                }
                vector_tables[cleint.second].total_minutes += minutes_table;
                vector_tables[cleint.second].revenue += revenueTable(minutes_table);
                cout << cleint.first << " ";
            }
            cout << "\n";
        }
    }

    // Вывод ошибки события
    void printEvent(event event, ErrorsType error_type)
    {
        switch (error_type)
        {
        case ErrorsType::YouShallNotPass:
            cout << event.time_event << " 13 YouShallNotPass!\n";
            break;
        case ErrorsType::NotOpenYet:
            cout << event.time_event << " 13 NotOpenYet!\n";
            break;
        case ErrorsType::PlaceIsBusy:
            cout << event.time_event << " 13 PlaceIsBusy!\n";
            break;
        case ErrorsType::ClientUnknown:
            cout << event.time_event << " 13 ClientUnknown!\n";
            break;
        case ErrorsType::ICanWaitNoLonger:
            cout << event.time_event << " 13 ICanWaitNoLonger!\n";
            break;
        default:
            break;
        }
    }

    // Вывод строчки в которой найдена ошибка
    void printErrorStr(const string& error_str)
    {
        cout << "In the line " << error_str << " error !\n";
        exit(1);
    }

    // Добавление события со строки читающей файл (строчка содержит в себе входящее событие)
    event addEvent(string& event_str, unsigned short int& count_tables)
    {
        event                   temp_event;     // Вспомогательное событие
        string                  part_str;       // Часть строки например строка входящего событие 08:48 1 client1 тогда часть client1
        unsigned short int      count   = 0;    // Счетчик
        size_t                  start   = 0;    // Начало части строки
        size_t                  end     = 0;    // Конец части строки

        // Обработка входящей строки на части
        while (1)
        {
            // Поиск конца части строки 
            end = event_str.find(' ', start);

            // Если пробел не найден, то это конец строки
            if (end == string::npos)
            {
                part_str = event_str.substr(start);
            }
            else
            {
                part_str = event_str.substr(start, end - start);
            }

            ++count;

            // В зависимости от части строки входящего события работает соответствующая часть кода
            // Если count == 1 то это время события, если count == 2 то это идентификатор события и т.д.
            switch (count)
            {
            case 1:
                temp_event.time_event = part_str;
                break;
            case 2:
                temp_event.id_event = stoi(part_str);
                if (temp_event.id_event > 4)
                {
                    printErrorStr(event_str);
                }
                break;
            case 3:
                temp_event.name_client = part_str;
                break;
            case 4:
            {
                // Если идентификатор события равен двум, то добавляем в строку события номер стола т.е. четвертую часть
                if (temp_event.id_event == 2)
                {
                    // Если номер стола больше количество всех столов, т.к. столы идут в порядке
                    if (stoi(part_str) > count_tables)
                    {
                        printErrorStr(event_str);
                    }
                    else
                    {
                        temp_event.number_table = stoi(part_str);
                    }
                }
            }
            break;
            default:
                break;
            }

            // Если достигнут конец строки, то выходим из цикла while
            if (end == string::npos)
                break;

            start = end + 1;
        }

        // Если данные входящего события неверны
        if (!manager_validate.checkEvent(temp_event))
        {
            printErrorStr(event_str);
        }

        return temp_event;
    }

    // Рассчет выручки со стола
    unsigned short int revenueTable(const unsigned short int& minutes)
    {
        return (((minutes / 60) + 1) * 10);
    }

    unsigned short int                      count_tables    = 0;      // Количество столов
    unsigned short int                      count_line      = 0;      // Счетчик строк файла
    unsigned short int                      cost_hour       = 0;      // Стоимость за час проведенный за столом
    unsigned short int                      table_num       = 0;      // Вспомогательная переменная номер стола
    unsigned short int                      minutes_table   = 0;      // Вспомогательная переменная минуты проведенные за столом
    string                                  read_line;                // Читающая строка файла
    string                                  time;                     // Время открытие:закрытие формата XX:XX XX:XX
    string                                  time_open;                // Время открытия формата XX:XX
    string                                  time_close;               // Время закрытия формата XX:XX
    bool                                    is_table_free   = false;  // Вспомогательная переменная стол занят ?

    vector  <event>                         vector_events;            // Динамический массив (вектор) событий
    vector  <table>                         vector_tables;            // Динамический массив (вектор) столов
    queue   <string>                        queue_waiting;            // Очередь которая хранит очередь ожидания
    map     <string, unsigned short int>    clients;                  // Map хранит клиентов, которые находяться в компьютерном клубе

    event                                   temp_event;               // Вспомогательное событие

    ManagerTime                             manager_time;             // Объект класса manager_time
    ManagerValidate                         manager_validate;         // Объект класса manager_validate

    ifstream                                file;                     // Объект из библиотеки <fstream> для чтения с файла
};