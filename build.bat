@echo off
cc -std=c17 -Wall -Wextra -pedantic ansi/ansi_escapes.c main.c -o BlackJack.exe
echo Build erfolgreich!