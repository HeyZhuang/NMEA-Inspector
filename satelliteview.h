#ifndef SATELLITEVIEW_H
#define SATELLITEVIEW_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QCheckBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMap>
#include "satellitedata.h"

class SatelliteView : public QWidget
{
    Q_OBJECT

public:
    explicit SatelliteView(QWidget *parent = nullptr);
    ~SatelliteView();

    void updateData(const SatelliteData &data);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onSystemToggled();

private:
    void setupUI();
    void drawRadarChart(QPainter &painter);
    void drawGrid(QPainter &painter);
    void drawLabels(QPainter &painter);
    void drawSatellites(QPainter &painter);
    void drawColorLegend(QPainter &painter);
    
    // 计算卫星在雷达图上的位置
    QPoint calculateSatellitePosition(double elevation, double azimuth);
    
    // 添加测试数据
    void addTestData();

private:
    // UI组件（简化为浮动复选框）
    QCheckBox *m_gpsCheckBox;
    QCheckBox *m_bdsCheckBox;
    QCheckBox *m_glnCheckBox;
    QCheckBox *m_galCheckBox;
    
    // 数据
    SatelliteData m_currentData;
    QList<SatelliteInfo> m_visibleSatellites;
    
    // 绘制参数
    QRect m_chartRect;
    int m_centerX;
    int m_centerY;
    int m_radius;
    
    // 系统颜色
    QMap<QString, QColor> m_systemColors;
};

#endif // SATELLITEVIEW_H
