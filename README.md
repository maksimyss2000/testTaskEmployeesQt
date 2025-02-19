Необходимо разработать приложение на C++ с использованием Qt, которое будет выполнять следующие функции:

1. Создание пользовательского интерфейса:
   - Разработайте форму с двумя кнопками: "Записать файл" и "Проверить файл".
   - Добавьте поля ввода для следующей информации: ФИО сотрудника, пол, возраст, стаж и его телефонный номер. Должна быть возможность из интерфейса (сделать соответствующие кнопки) динамически добавлять и удалять сотрудников, а также переключаться между существующими. Данные о каждом сотруднике должны храниться в структуре (классе) в соответствующих полях.
   - Добавьте нередактируемое текстовое поле для отображения информации о контрольной сумме файла.
   - Добавьте текстовое поле для ввода числового значения, которое будет использоваться при обработке данных (целое число от 0 до 255).

2. Обработка и запись данных в файл:
   - При нажатии на кнопку "Записать файл", приложение должно выполнить следующие шаги:
     1. Прочитать данные, введённые пользователем.
     2. Преобразовать (сериализовать) данные в последовательный массив байтов.
     3. Выполнить побитовую операцию XOR над каждым байтом с введенным числовым значением.
     4. Записать результат обработки в файл, выбрав имя файла через стандартный диалог сохранения файлов.
     5. После записи данных, приложение должно автоматически сгенерировать контрольную сумму (например, CRC32) для этого файла и сохранить её в отдельный файл с тем же именем, но с расширением .crc.

3. Проверка и чтение файла:
   - При нажатии на кнопку "Проверить файл", пользователь должен выбрать файл через стандартный диалог открытия файлов.
   - Приложение должно выполнить следующие шаги:
     1. Прочитать содержимое выбранного файла и вычислить его контрольную сумму.
     2. Проверить, совпадает ли эта контрольная сумма с той, что хранится в соответствующем .crc файле.
     3. Если контрольная сумма совпадает, приложение должно:
        - Выполнить обратную операцию XOR с каждым байтом данных, используя числовое значение, введенное пользователем.
        - Преобразовать (десериализовать) полученные байты обратно в данные.
        - Заполнить форму со списком сотрудников полученными данными.
     4. Должно выводиться сообщение об успехе или неудаче при проверке контрольной суммы.


Условия:
1. Файл должен содержать данные в бинарном формате после предварительной обработки.
2. Контрольная сумма должна генерироваться и проверяться с использованием алгоритма CRC32.
3. Программа должна быть написана с использованием стандартов C++11 или новее.
4. Должен использоваться Qt для реализации пользовательского интерфейса и работы с диалоговыми окнами для выбора файлов.
5. Код должен быть чистым, читаемым и хорошо структурированным.
6. Реализовать обработку ошибок (например, если контрольная сумма не совпадает или файл повреждён).

Кандидат должен предоставить исходный код приложения, который включает в себя:
- Исходные файлы .cpp и .h.
- Файл проекта Qt (.pro файл).
- Краткие инструкции по сборке и запуску приложения.


# Build
 sudo apt install qt5-qmake qtbase5-dev
 
 cd src
 
 qmake
 
 make
 
 ./EfimovDrone  


# Example of use

[my.webm](https://github.com/user-attachments/assets/0cf03296-4aff-42af-9d02-22893ccd12fd)
