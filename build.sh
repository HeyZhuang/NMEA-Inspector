#!/bin/bash

echo "正在编译AI串口NMEA解析器..."

# 检查Qt环境
if ! command -v qmake &> /dev/null; then
    echo "错误: 未找到qmake，请确保Qt环境已正确安装并添加到PATH"
    exit 1
fi

# 清理之前的编译文件
echo "清理之前的编译文件..."
rm -f Makefile Makefile.Debug Makefile.Release
rm -rf debug release

# 生成Makefile
echo "正在生成Makefile..."
qmake AI-serial-NEMA.pro
if [ $? -ne 0 ]; then
    echo "错误: qmake执行失败"
    exit 1
fi

# 编译项目
echo "正在编译项目..."
make
if [ $? -ne 0 ]; then
    echo "错误: 编译失败"
    exit 1
fi

echo "编译完成！"
echo "可执行文件位置: debug/AI-serial-NEMA"
