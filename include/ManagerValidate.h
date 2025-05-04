#pragma once
#include <iostream>
#include <string>
#include "Struct.h"
#include "ManagerTime.h"
using namespace std;

// Менеджер валидации (проверок), существует для проверки входных данных
class ManagerValidate
{
public:
    // Проверка корректности числа
    bool checkNumber(const string& number_str)
    {
        // Число не должно быть равно 0
        if (number_str[0] == '0')
        {
            return false;
        }

        // Число должно быть от 0 до 9
        for (unsigned short int i = 0; i < number_str.size(); ++i)
        {
            if (number_str[i] < '0' || number_str[i] > '9')
            {
                return false;
            }
        }
        return true;
    }

    // Проверка корректности времени формата XX:XX
    bool isValidTime(const string& time)
    {
        // Проверка длины и разделителя симовала ':'
        if (time.size() != 5 || time[2] != ':')
        {
            return false;
        }

        // Проверка что символы 0, 1, 3, 4 элемента являются цифрами
        for (unsigned short int i : {0, 1, 3, 4})
        {
            if (time[i] < '0' || time[i] > '9')
            {
                return false;
            }
        }

        // Преобразование часов и минут
        unsigned short int hours = (time[0] - '0') * 10 + (time[1] - '0');
        unsigned short int minutes = (time[3] - '0') * 10 + (time[4] - '0');

        // Проверка диапазонов
        return (hours >= 0 && hours <= 23) && (minutes >= 0 && minutes <= 59);
    }

    // Проверка корректности времени формата XX:XX XX:XX
    bool checkTime(const string& time)
    {
        // Проверка является ли 5 элемент строки пробелом в формате времени XX:XX XX:XX 
        if (time.find(' ') == string::npos || time.find(' ') != 5)
        {
            return false;
        }

        // Разделение времени формата XX:XX XX:XX на XX:XX и XX:XX и проверка их корректности
        return isValidTime(time.substr(0, 5)) && isValidTime(time.substr(6, 5));
    }

    // Проверка корректности времени события
    bool checkTimeEvent(const string& time_event)
    {
        // Если строка пустая сразу неправильно
        if (time_event.empty()) return false;
        return isValidTime(time_event);
    }

    // Проверка корректности имени клиента
    bool checkNameClient(const string& name_client)
    {
        // Если строка пустая сразу неправильно
        if (name_client.empty()) return false;

        // Проверка каждого символа имени клиента. Имя должно быть комбинацией
        // символов a..z, 0..9, _, -
        for (unsigned short int i = 0; i < name_client.size(); ++i)
        {
            if (!((name_client[i] >= 'a' && name_client[i] <= 'z') ||
                (name_client[i] >= '0' && name_client[i] <= '9') ||
                (name_client[i] == '_') ||
                (name_client[i] == '-')))
            {
                return false;
            }
        }
        return true;
    }

    // Проверка открыт ли клуб и работает ли он круглосуточно
    bool isClubOpen(const string& current_time, const string& open_time, const string& close_time)
    {
        unsigned short int current = manager_time.timeToMinutes(current_time);
        unsigned short int open = manager_time.timeToMinutes(open_time);
        unsigned short int close = manager_time.timeToMinutes(close_time);

        if (open == close)
        {
            return true;
        }
        else if (open < close)
        {
            return current >= open && current <= close;
        }
        else
        {
            return current >= open || current <= close;
        }
    }

    // Проверка всех данных события
    bool checkEvent(const event& event)
    {
        return checkTimeEvent(event.time_event) && checkNumber(to_string(event.id_event)) && checkNameClient(event.name_client);
    }
private:
    // Объект класса ManagerTime
    ManagerTime manager_time;
};