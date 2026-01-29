@echo off
set PROJECT_DIR=C:\Users\SYH\Desktop\website\syh
cd /d "%PROJECT_DIR%"

:: 1. 创建并执行临时隐藏启动脚本
echo [1/3] 正在执行 npm run dev...

echo Set WshShell = CreateObject("WScript.Shell") > hide_run.vbs
echo WshShell.Run "cmd /c npm run dev -- --open", 0, False >> hide_run.vbs

wscript.exe hide_run.vbs
del hide_run.vbs

echo.
echo 请在确认网页效果无误后，返回此处。
echo.

:: 2. 询问并 Build
set /p choice=确认代码无误，执行build吗? (Y/N): 
if /i "%choice%" neq "Y" exit

:: 3. 关闭后台 Node 进程
taskkill /f /im node.exe >nul 2>&1

echo.
echo [2/3] 正在执行 npm run build...
call npm run build

if %errorlevel% neq 0 (
    echo [错误] 打包失败！
    pause
    exit
)

:: 4. 同步文件
echo.
echo [3/3] 正在同步文件至根目录...
del /q index.html 2>nul
if exist assets rd /s /q assets
xcopy "dist\*" ".\" /e /y /i

echo.
echo ---------------------------------------
echo [成功] 文件已同步！
echo.

if exist "%LocalAppData%\GitHubDesktop\GitHubDesktop.exe" (
    start "" "%LocalAppData%\GitHubDesktop\GitHubDesktop.exe"
)

pause