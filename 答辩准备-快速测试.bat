@echo off
echo ========================================
echo    AI串口NMEA解析器 - 答辩准备测试
echo ========================================
echo.

echo 正在清理旧的编译文件...
if exist debug rmdir /s /q debug
if exist release rmdir /s /q release
if exist build rmdir /s /q build

echo.
echo 正在生成Makefile...
qmake satellite_app.pro
if %errorlevel% neq 0 (
    echo ❌ qmake 失败！
    pause
    exit /b 1
)

echo.
echo 正在编译项目...
nmake
if %errorlevel% neq 0 (
    echo ❌ 编译失败！
    pause
    exit /b 1
)

echo.
echo ✅ 编译成功！
echo.

echo ========================================
echo           项目功能测试
echo ========================================
echo.

echo 📋 项目信息：
echo - 项目名称: AI串口NMEA解析器
echo - 开发语言: C++
echo - 开发框架: Qt 5.15+
echo - 代码量: 约4000行
echo - 模块数: 5个核心模块
echo - 支持系统: GPS/BDS/GLONASS/Galileo
echo.

echo 🎯 团队分工：
echo - 成员A: 项目负责人 + 串口通信专家
echo - 成员B: 数据可视化与图表渲染专家  
echo - 成员C: 用户界面与系统集成专家
echo - 成员D: 数据分析与算法专家
echo - 成员E: 测试与质量保证专家
echo.

echo 🚀 核心功能：
echo - ✅ 多卫星系统支持 (GPS/BDS/GLONASS/Galileo)
echo - ✅ 实时串口数据接收和解析
echo - ✅ 信噪比柱状图可视化
echo - ✅ 卫星雷达图 (极坐标显示)
echo - ✅ 运动轨迹图
echo - ✅ 数据保存和离线回放
echo - ✅ 跨平台支持 (Windows/Linux/macOS)
echo.

echo 📊 测试数据：
echo - test_data.nmea: 基础GPS数据
echo - test_multi_systems.nmea: 多卫星系统数据
echo - 0407.nmea: 实际GNSS数据
echo.

echo 🎨 界面特色：
echo - 响应式布局设计
echo - 实时状态显示
echo - 直观的操作流程
echo - 多系统颜色区分
echo.

echo ========================================
echo           答辩准备检查清单
echo ========================================
echo.

echo 📋 技术文档：
echo - [ ] 项目架构设计文档
echo - [ ] 技术实现细节文档  
echo - [ ] API接口文档
echo - [ ] 用户使用手册
echo - [ ] 测试报告
echo.

echo 🎨 演示材料：
echo - [ ] 答辩PPT (成员A负责)
echo - [ ] 功能演示视频
echo - [ ] 技术架构图
echo - [ ] 界面截图
echo - [ ] 性能测试结果
echo.

echo 💻 软件环境：
echo - [ ] 编译好的可执行文件
echo - [ ] 测试数据文件
echo - [ ] 演示脚本
echo - [ ] 备用演示方案
echo.

echo 📝 个人材料：
echo - [ ] 个人技术贡献说明
echo - [ ] 学习收获总结
echo - [ ] 技术难点解决方案
echo - [ ] 未来学习计划
echo.

echo ========================================
echo           启动应用程序
echo ========================================
echo.

echo 正在启动应用程序...
echo 请测试以下功能：
echo 1. 加载 test_multi_systems.nmea 文件
echo 2. 检查多卫星系统显示
echo 3. 测试各种图表功能
echo 4. 验证数据保存和加载
echo.

debug\SatelliteApp.exe

echo.
echo 测试完成！
pause

