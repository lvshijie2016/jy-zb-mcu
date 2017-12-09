echo off
echo ---------------------------------------------------------------
echo INPUT APP VERSION, LIKE:2.6.1
set /p var="Version is:"
echo 当前目录是：%cd%
cd Bin
set d=%date:~,4%%date:~5,2%%date:~8,2%
rename app.bin app_v%VAR%_%d%.bin
