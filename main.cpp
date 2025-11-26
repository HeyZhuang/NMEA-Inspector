#include <QApplication>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "mainwindow.h"
#include "satelliteview.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 设置应用程序信息
    app.setApplicationName("卫星应用软件");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("GNSS可视化工具");
    
    // 设置应用程序样式
    app.setStyle("Fusion");
    
    // 加载样式表
    QFile styleFile(":/styles.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString style = QTextStream(&styleFile).readAll();
        app.setStyleSheet(style);
        styleFile.close();
    }
    
    // 创建主窗口
    MainWindow window;
    window.show();
    
    // 验证脚本：强制提升SatelliteView到最顶层
    SatelliteView* satelliteView = window.findChild<SatelliteView*>();
    if (satelliteView) {
        satelliteView->raise();
        satelliteView->activateWindow();
        qDebug() << "SatelliteView已强制提升到最顶层";
    }
    
    return app.exec();
}