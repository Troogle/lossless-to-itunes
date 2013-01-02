@ECHO off 
cls 
color 0A 
ECHO   欢迎使用 DAEMON Tools Lite 4.40.2.131!
ECHO        免启动免安装即点即用绿色版!
ECHO        ☆ 绿化制作：fzy2008 ☆
ECHO       http://hi.baidu.com/23070205
ECHO =========================================
ECHO 程序运行中，请稍等。。。。。。 
pause
RUNDLL32.EXE SETUPAPI.DLL,InstallHinfSection DefaultInstall 128 .\1.inf
start DTLite.exe