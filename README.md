# Test
Программа, выводящая содержимое .Xml файла и бинарного файла. 

Приложение открывает и загружает данные из XML файла.
- Основная область окна приложения содержит виджет отображающий маркеры.
- Начало координат для маркеров всегда в центре виджета.
- Возможность масштабирования содержимого виждета с помощью колеса прокрутки. (не работает при вертикальном изменении размеров приложения)
- Автоматическое масштабирование виджета при изменении размеров окна приложения.
- Приложение разработано на языке программирования C++ с использованием Qt5.

- чтение и вывод содержимого бинарного файла (координаты точек, которые рисуются на главном окне)
- это чтение организовано в отдельном потоке порциями по 1000 точек.
