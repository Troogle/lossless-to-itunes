@ECHO off 
cls 
color 0A 
ECHO   ��ӭʹ�� DAEMON Tools Lite 4.40.2.131!
ECHO        �������ⰲװ���㼴����ɫ��!
ECHO        �� �̻�������fzy2008 ��
ECHO       http://hi.baidu.com/23070205
ECHO =========================================
ECHO ���������У����Եȡ����������� 
pause
RUNDLL32.EXE SETUPAPI.DLL,InstallHinfSection DefaultInstall 128 .\1.inf
start DTLite.exe