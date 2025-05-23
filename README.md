# Тестовое задание YADRO 2025 эмуляция компьютерного клуба
Требуется написать прототип системы, которая следит за работой компьютерного клуба, обрабатывает события и подсчитывает выручку за день и время занятости каждого стола.
***
## Тестовые файлы
- В папке TestFromYADRO содержится проверочный тест от компании YADRO.
- В папке CorrectTests содержатся тесты, которые должны выполняться правильно.
- В папке  IncorrectTests содержатся тесты, которые должны выполняться с ошибкой.
***
## Запуск на Windows (компиляция программы с помощью mingw):
Если у вас не установлен mingw, то вот ссылка видео на YouTube, где показана установка mingw https://www.youtube.com/watch?v=MLYNMxUDDRk<br>
Запускаем __Windows PowerShell__ и переходим в папку с проектом, выполняем следующую команду:
```
g++ main.cpp -o task.exe
```
Далее для запуска программы выполним команду:
```
./task.exe TestFromYADRO/test_file.txt
```
Обязательно в качестве аргумента указываем входной файл, иначе произойдёт ошибка.
***
## Запуск на Linux:
Для запуска программы на Linux потребуется выполнить несколько команд в консоли (установка компиляторов), которые необходимы для компиляции программы.
```
sudo apt update
sudo apt install build-essential 
sudo apt install clang
```
После установки переходим в папку с проектом и выполняем одну из следующих команд, в зависимости от того компилятора который хотим использовать.
```
g++ main.cpp -o task
clang++ main.cpp -o task
```
Далее для выполнения программы используем команду:
```
./task TestFromYADRO/test_file.txt
```
Обязательно в качестве аргумента указываем входной файл, иначе произойдёт ошибка.
***
