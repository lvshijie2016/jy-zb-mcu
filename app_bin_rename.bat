echo off
echo ---------------------------------------------------------------
echo INPUT APP VERSION, LIKE:2.6.1
set /p var="Version is:"
echo 当前目录是：%cd%
cd Bin
set d=%date:~,4%%date:~5,2%%date:~8,2%
rename app.bin app_v%VAR%_%d%.bin
=======
echo off
echo ---------------------------------------------------------------
IF not EXIST app.bin echo app.bin文件不存在
IF not EXIST app.bin pause
echo INPUT APP VERSION, LIKE:2.6.1
set /p var="Version is:"
echo 当前目录是：%cd%
set /p type="debug(input 0) or release(input 1):"
cd Bin
set d=%date:~,4%%date:~5,2%%date:~8,2%
if %type% EQU 0 rename app.bin app_v%VAR%_%d%_debug.bin
if %type% EQU 1 rename app.bin app_v%VAR%_%d%_release.bin