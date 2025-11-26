@echo off
echo 正在编译和调试多卫星系统显示...
qmake satellite_app.pro
if %errorlevel% equ 0 (
    echo qmake 成功
    nmake
    if %errorlevel% equ 0 (
        echo 编译成功！
        echo.
        echo === 多卫星系统调试 ===
        echo.
        echo 问题分析：
        echo 1. 卫星ID范围判断可能不正确
        echo 2. GSV解析可能没有正确设置卫星系统
        echo 3. 数据更新可能有问题
        echo.
        echo 修复内容：
        echo 1. 修正了卫星ID范围判断
        echo 2. 在GSV解析中根据NMEA前缀设置卫星系统
        echo 3. 确保数据正确传递到各个视图
        echo.
        echo 测试数据包含：
        echo - GPS: ID 1,2,12,14,15,18,24,25
        echo - BDS: ID 97,98,99,100,101,102  
        echo - GLONASS: ID 33,34,35,36,37
        echo - Galileo: ID 65,66,67,68,69,70
        echo.
        echo 请加载 test_multi_systems.nmea 文件测试！
        echo 检查控制台输出中的调试信息。
        echo.
        debug\satellite_app.exe
    ) else (
        echo 编译失败
    )
) else (
    echo qmake 失败
)
pause
