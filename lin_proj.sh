#!/bin/bash
# Скрипт генерирует "правильный" файл проекта для компиляции в среде Linux
qmake -project
mv platon-accs.pro platon-accs.tmp
cat project.inc >platon-accs.pro
cat platon-accs.tmp >>platon-accs.pro
rm platon-accs.tmp

