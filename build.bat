@echo off
echo 正在编译卫星应用软件...

REM 清理之前的构建文件
if exist debug rmdir /s /q debug
if exist release rmdir /s /q release
if exist Makefile del Makefile
if exist Makefile.Debug del Makefile.Debug
if exist Makefile.Release del Makefile.Release

REM 运行qmake
qmake satellite_app.pro

REM 编译
make

if %errorlevel% == 0 (
    echo 编译成功！
    echo 可执行文件位置: debug/SatelliteApp.exe
) else (
    echo 编译失败！
    echo 请检查错误信息
)

pause
