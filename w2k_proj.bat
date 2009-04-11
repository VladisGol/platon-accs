rem Скрипт генерирует "правильный" файл проекта для компиляции в среде Windows
qmake -project
copy project.inc platon-accs.tmp
type platon-accs.pro >>platon-accs.tmp
del platon-accs.pro
ren platon-accs.tmp platon-accs.pro
qmake
