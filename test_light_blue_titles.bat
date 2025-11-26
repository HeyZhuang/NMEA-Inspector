@echo off
echo 正在编译和测试浅蓝色标题...
qmake satellite_app.pro
if %errorlevel% equ 0 (
    echo qmake 成功
    nmake
    if %errorlevel% equ 0 (
        echo 编译成功！
        echo.
        echo === 浅蓝色标题测试 ===
        echo 请检查以下统一的浅蓝色标题：
        echo.
        echo 1. 分类标题行 (=== 基本信息 === 等)：
        echo    - 背景：浅蓝色 (#add8e6) - Light Blue
        echo    - 文字：深蓝色 (#191970) - Midnight Blue
        echo    - 字体：Arial 10pt 粗体
        echo.
        echo 2. 子系统标题行 (--- GPS系统 --- 等)：
        echo    - 背景：浅蓝色 (#add8e6) - Light Blue
        echo    - 文字：深蓝色 (#191970) - Midnight Blue
        echo    - 字体：Arial 9pt 粗体
        echo.
        echo 3. 空行分隔：
        echo    - 背景：浅灰色 (#ecf0f1)
        echo    - 文字：深灰色 (#2c3e50)
        echo.
        echo 4. 普通数据行：
        echo    - 背景：白色 (#ffffff)
        echo    - 文字：深灰色 (#2c3e50)
        echo.
        echo 5. 重要数据 (卫星数、PDOP等)：
        echo    - 背景：白色 (#ffffff)
        echo    - 文字：绿色 (#27ae60) 粗体
        echo.
        echo 所有标题现在都使用统一的浅蓝色背景！
        echo 文字清晰可见，颜色搭配协调！
        echo.
        debug\satellite_app.exe
    ) else (
        echo 编译失败
    )
) else (
    echo qmake 失败
)
pause
