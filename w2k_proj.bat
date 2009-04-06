rem Скрипт генерирует "правильный" файл проекта для компиляции в среде Windows
qmake -project
copy project.inc+platon-accs.pro platon-accs.pro
qmake
