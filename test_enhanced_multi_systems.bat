@echo off
echo 正在编译和测试增强的多卫星系统展示...
qmake satellite_app.pro
if %errorlevel% equ 0 (
    echo qmake 成功
    nmake
    if %errorlevel% equ 0 (
        echo 编译成功！
        echo.
        echo === 增强的多卫星系统展示测试 ===
        echo.
        echo 🛰️ 卫星雷达图优化：
        echo - 更鲜明的多系统配色方案
        echo - GPS: 森林绿 (#228B22)
        echo - BDS: 深红色 (#DC143C)  
        echo - GLONASS: 道奇蓝 (#1E90FF)
        echo - Galileo: 蓝紫色 (#8A2BE2)
        echo - QZSS: 橙色 (#FFA500)
        echo - SBAS: 灰色 (#808080)
        echo - 右侧显示系统统计信息面板
        echo - 显示各系统卫星数量和使用状态
        echo.
        echo 📊 载噪比分析优化：
        echo - 动态显示有数据的系统
        echo - 2x2布局自适应调整
        echo - 与卫星雷达图颜色保持一致
        echo - 改进的图表标题和标签
        echo.
        echo 📋 NMEA消息详情优化：
        echo - 按系统优先级排序显示
        echo - GPS → BDS → GLONASS → Galileo → QZSS → SBAS
        echo - 统一的浅蓝色标题样式
        echo - 详细的多系统信息展示
        echo.
        echo 🧪 测试数据包含：
        echo - GPS: 8颗卫星 (ID: 1,2,12,14,15,18,24,25)
        echo - BDS: 6颗卫星 (ID: 97,98,99,100,101,102)
        echo - GLONASS: 5颗卫星 (ID: 33,34,35,36,37)
        echo - Galileo: 6颗卫星 (ID: 65,66,67,68,69,70)
        echo - 多系统组合: 20颗卫星混合
        echo.
        echo 请加载 test_multi_systems.nmea 文件进行测试！
        echo.
        debug\satellite_app.exe
    ) else (
        echo 编译失败
    )
) else (
    echo qmake 失败
)
pause
