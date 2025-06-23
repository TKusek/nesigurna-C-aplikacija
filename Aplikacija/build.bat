@echo off
echo Kompilacija zapocela...

gcc main.c sqlite3.c -o main.exe

if %ERRORLEVEL% equ 0 (
    echo Kompilacija uspjesna. Pokrecem aplikaciju...
    main.exe
) else (
    echo Doslo je do pogreske pri kompilaciji.
)

pause
