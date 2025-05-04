#pragma once
#include <string>
using namespace std;

// Структура входящего события
struct event
{
    unsigned short int      id_event = 0;           // ID события
    unsigned short int      number_table = 0;       // Номер стола
    string                  name_client;            // Имя клиента
    string                  time_event;             // Время события
};

// Структура стола
struct table
{
    unsigned short int      revenue = 0;            // Прибыль
    unsigned short int      total_minutes = 0;      // Общее количество минут проведенное за столом
    string                  client_name;            // Имя клиента, который сидит за столом
    string                  occupied_since;         // Время с которого занят стол
    bool                    is_occupied = false;    // Стол занят?
};