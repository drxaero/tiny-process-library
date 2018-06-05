@ECHO OFF
REM exit the code based on user's command arg
ECHO This batch file will exit with code: %~1

REM Since the environment variable %ERRORLEVEL% cannot be set, so use an external batch file to set it.
REM The '~' ensures double quotes are stripped if they were on the command line argument
ECHO @exit /b %~1 > ret.cmd
CALL ret.cmd

