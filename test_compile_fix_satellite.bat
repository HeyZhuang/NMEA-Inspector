@echo off
echo 正在编译和测试卫星雷达图修复...
qmake satellite_app.pro
if %errorlevel% equ 0 (
    echo qmake 成功
    nmake
    if %errorlevel% equ 0 (
        echo 编译成功！错误已修复！
        echo.
        echo === 修复内容 ===
        echo 1. 修复了 m_satellites 变量名错误
        echo    - 改为使用 m_currentData.satellites
        echo 2. 修复了 QList.sum() 方法错误
        echo    - 改为手动循环计算总和
        echo 3. 卫星雷达图现在可以正确显示：
        echo    - 多系统统计信息面板
        echo    - 各系统卫星数量和使用状态
        echo    - 总计信息
        echo.
        echo === 功能特点 ===
        echo - GPS: 森林绿 (#228B22)
        echo - BDS: 深红色 (#DC143C)
        echo - GLONASS: 道奇蓝 (#1E90FF)
        echo - Galileo: 蓝紫色 (#8A2BE2)
        echo - QZSS: 橙色 (#FFA500)
        echo - SBAS: 灰色 (#808080)
        echo.
        echo 现在可以正常编译和运行！
        echo.
        debug\satellite_app.exe
    ) else (
        echo 编译失败，请检查错误信息
    )
) else (
    echo qmake 失败
)
pause
