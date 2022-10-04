# Как собрать игру
  1. скачиваем репозиторий по [ссылке](https://github.com/millysmask02/coursework/tree/dev)
  2. переходим по каталогу вниз: coursework-dev.zip\coursework-dev\coursework и разархивируем "SFML.zip"
  3. Требуется открыть проект через Visual Studio 2017/2019 года
  4. Подключаем нужные библиотеки:
  
     4.1. Проект -> свойства
     
     4.2. (конфигурация: Активная (Debug)
     
         4.2.1. свойства конфигурации -> C/C++ -> Общие -> Дополнительные каталоги включаемых файлов
                указываем путь до папки "include". Например, C:\Users\source\coursework\SFML\include
         4.2.2. свойства конфигурации -> C/C++ -> Предпроцессор -> Определения предпроцессора
                добавлем в начало: SFML_DYNAMIC
         4.2.3. свойства конфигурации -> Компоновщик -> Общие -> Дополнительные каталоги библиотек
                указывается путь до папки "lib". Например, C:\Users\source\coursework\SFML\lib
         4.2.4. свойства конфигурации -> Компоновщик -> Ввод -> Дополнительные зависимости
                добавляем sfml-graphics-d.lib;sfml-window-d.lib;sfml-system-d.lib;sfml-audio-d.lib;
             
     4.3. Аналогично делаем для (конфигурация: Release)
     
     4.4. Нажимаем "ок"
     
  5. Запускаем