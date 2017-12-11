echo off
echo ---------------------------------------------------------------
IF not EXIST iap.bin echo iap.bin文件不存在
IF not EXIST iap.bin pause
echo INPUT IAP VERSION, LIKE:2.6.1
set /p var="Version is:"
echo 当前目录是：%cd%
set /p type="debug(input 0) or release(input 1):"
cd Bin
set d=%date:~,4%%date:~5,2%%date:~8,2%
if %type% EQU 0 rename iap.bin iap_v%VAR%_%d%_debug.bin
if %type% EQU 1 rename iap.bin iap_v%VAR%_%d%_release.bin
