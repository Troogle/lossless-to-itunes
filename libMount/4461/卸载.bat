@echo off
del "C:\WINDOWS\inf\oem10.inf" /s /q
del "C:\WINDOWS\inf\oem10.PNF" /s /q
cls 
del "C:\WINDOWS\LastGood\INF\oem10.inf" /s /q
del "C:\WINDOWS\LastGood\INF\oem10.PNF" /s /q
cls 
del "C:\WINDOWS\system32\drivers\dtsoftbus01.sys" /s /q
cls 
color 0A 
ECHO             谢谢使用!
ECHO       ☆ 绿化制作：fzy2008 ☆
ECHO    http://hi.baidu.com/23070205
ECHO ===============================
ECHO 按任意键清理消痕。。。。。。 
pause
RUNDLL32.EXE SETUPAPI.DLL,InstallHinfSection DefaultUnInstall 128 .\1.inf
