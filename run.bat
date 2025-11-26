@echo off
echo 🛰️ 启动卫星应用软件...

REM 检查是否存在可执行文件
if exist "debug\SatelliteApp.exe" (
    echo ✅ 找到可执行文件，正在启动...
    start "" "debug\SatelliteApp.exe"
) else if exist "release\SatelliteApp.exe" (
    echo ✅ 找到可执行文件，正在启动...
    start "" "release\SatelliteApp.exe"
) else (
    echo ❌ 未找到可执行文件！
    echo 请先运行 build.bat 编译项目
    pause
    exit /b 1
)

echo 🚀 软件已启动！
pause
