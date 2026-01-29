@echo off

title 文件同步助手

echo 正在监控文件变化
echo 目录: %cd%
echo ------------------------------------------

cd /d "%USERPROFILE%\Desktop\website\syh"

node sync.js

pause