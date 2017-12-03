gcc Codif.c -o Codif.exe
if %ERRORLEVEL% == 0 ( GOTO EXEC )
GOTO OUT
:EXEC
Codif.exe Teste.tvf Sample01_000.bmp Sample01_001.bmp Sample01_002.bmp Sample01_003.bmp
:OUT
ECHO %ERRORLEVEL%
