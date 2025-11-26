@echo off
echo 正在编译和测试多卫星系统解析...
qmake satellite_app.pro
if %errorlevel% equ 0 (
    echo qmake 成功
    nmake
    if %errorlevel% equ 0 (
        echo 编译成功！
        echo.
        echo === 多卫星系统解析测试 ===
        echo 现在软件支持解析以下卫星系统：
        echo.
        echo 1. GPS系统 (GP前缀):
        echo    - $GPGGA, $GPRMC, $GPGSV, $GPGSA, $GPGLL, $GPVTG, $GPZDA
        echo    - 卫星ID范围: 1-32
        echo.
        echo 2. BDS系统 (BD前缀):
        echo    - $BDGGA, $BDRMC, $BDGSV, $BDGSA, $BDGLL, $BDVTG, $BDZDA
        echo    - 卫星ID范围: 97-158
        echo.
        echo 3. GLONASS系统 (GL前缀):
        echo    - $GLGGA, $GLRMC, $GLGSV, $GLGSA, $GLGLL, $GLVTG, $GLZDA
        echo    - 卫星ID范围: 33-64
        echo.
        echo 4. Galileo系统 (GA前缀):
        echo    - $GAGGA, $GARMC, $GAGSV, $GAGSA, $GAGLL, $GAVTG, $GAZDA
        echo    - 卫星ID范围: 65-96
        echo.
        echo 5. 多系统 (GN前缀):
        echo    - $GNGGA, $GNRMC, $GNGSV, $GNGSA, $GNGLL, $GNVTG, $GNZDA
        echo.
        echo 测试功能：
        echo - 载噪比分析窗口应该显示所有系统的卫星
        echo - 星位视图应该显示所有系统的卫星位置
        echo - NMEA消息详情应该显示所有系统的信息
        echo - 卫星系统识别基于ID范围自动判断
        echo.
        debug\satellite_app.exe
    ) else (
        echo 编译失败
    )
) else (
    echo qmake 失败
)
pause
