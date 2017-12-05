gcc Auxiliar.c -o Au.exe
if %ERRORLEVEL% == 0 ( GOTO EXEC )
GOTO OUT
:EXEC
Au.exe
:OUT
ECHO %ERRORLEVEL%
