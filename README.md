# dr-web-test
 dr web test task
# Задание
Необходимо реализовать графическое приложение “Интерполятор” с помощью фреймворка Qt. Приложение должно рисовать кривую линию по заданным точкам в одной плоскости.

Точки задаются двумя способами: из csv файла и через графический интерфейс (таблицу).
Формат csv файла (пример):
x,y
5.1,10.2
6.3,13.54
…

csv файл задаётся через аргумент (необязательный).
Если через аргумент задан файл, но он битый или отсутствует: выводить ошибку и выходить.

Загруженные из файла точки должны быть отображены в таблице, и нарисована кривая. В таблице точки можно редактировать, добавлять и удалять.
После изменения координат точки в таблице, отображение интерполированной кривой обновлять. Расчёты для отрисовки графика производить в отдельном потоке, приложение не должно "фризиться".

Для интерполяции должны быть доступны разные алгоритмы (например "Линейная интерполяция", "Многочлен Лагранжа", “Кубический сплайн” и т.д. можно свой). Для алгоритмов можно брать готовые математические библиотеки, но функции должны быть протестированы, например с помощью google_test

Желательно чтобы проект был реализован на CMake, Qt достаточно 5 версии.
