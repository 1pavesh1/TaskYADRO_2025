#pragma once
#include <string>
using namespace std;

// Класс менеджер времени работает с преобразованием времени
class ManagerTime
{
public:
    // Разделение времени с строки "19:00 10:00" на "19:00" или "10:00"
    string splitTime(const string& time, bool flag)
    {
        size_t space_pos = time.find(' ');
        if (flag)
        {
            return time.substr(0, 5);
        }
        else
        {
            return time.substr(space_pos + 1, 5);
        }
    }

    // Преобразование количества минут во время формата XX:XX
    string timeToString(const unsigned short int& time)
    {
        unsigned short int timeHour = time / 60;
        unsigned short int timeMin = time % 60;
        return to_string(timeHour / 10) + to_string(timeHour % 10) + ":" +
            to_string(timeMin / 10) + to_string(timeMin % 10);
    }

    // Преобразование времени формата XX:XX в минуты
    unsigned short int timeToMinutes(const string& time)
    {
        return stoi(time.substr(0, 2)) * 60 + stoi(time.substr(3, 2));
    }

    // Раcсчет количества минут проведенные за столом
    unsigned short int tableOccupiedMinutes(const string& begin_time, const string& end_time)
    {
        return (timeToMinutes(end_time) - timeToMinutes(begin_time) + 24 * 60) % (24 * 60);
    }
};