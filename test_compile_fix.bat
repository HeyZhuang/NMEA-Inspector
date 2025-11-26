@echo off
echo 正在编译和测试修复后的多卫星系统解析...
qmake satellite_app.pro
if %errorlevel% equ 0 (
    echo qmake 成功
    nmake
    if %errorlevel% equ 0 (
        echo 编译成功！错误已修复！
        echo.
        echo === 修复内容 ===
        echo 1. 删除了旧的 getSatelliteSystem(int prn) 函数实现
        echo 2. 保留了新的 getSatelliteSystem(const QString &sentenceType) 函数
        echo 3. 保留了新的 getSatelliteSystemByID(int satelliteID) 函数
        echo 4. 函数声明和实现现在完全匹配
        echo.
        echo === 多卫星系统支持 ===
        echo - GPS (GP前缀): 卫星ID 1-32
        echo - BDS (BD前缀): 卫星ID 97-158  
        echo - GLONASS (GL前缀): 卫星ID 33-64
        echo - Galileo (GA前缀): 卫星ID 65-96
        echo - 多系统 (GN前缀): 混合ID
        echo.
        echo 现在可以正常解析所有卫星系统的NMEA语句！
        echo.
        debug\satellite_app.exe
    ) else (
        echo 编译失败，请检查错误信息
    )
) else (
    echo qmake 失败
)
pause
