echo off
echo ---------------------------------------------------------------
echo INPUT IAP VERSION, LIKE:2.6.1
set /p var="Version is:"
echo ��ǰĿ¼�ǣ�%cd%
cd Bin
set d=%date:~,4%%date:~5,2%%date:~8,2%
rename iap.bin iap_v%VAR%_%d%.bin
