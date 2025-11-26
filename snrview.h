#ifndef SNRVIEW_H
#define SNRVIEW_H

#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMap>
#include "satellitedata.h"

class SNRView : public QWidget
{
    Q_OBJECT

public:
    explicit SNRView(QWidget *parent = nullptr);
    ~SNRView();

    void updateData(const SatelliteData &data);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void setupUI();
    void drawSNRCharts(QPainter &painter);
    void drawSystemChart(QPainter &painter, const QString &system, 
                        const QList<SatelliteInfo> &satellites, 
                        const QRect &rect, const QColor &color);
    
    // 添加测试数据
    void addTestData();

private:
    // UI组件（简化为浮动标签）
    QLabel *m_usedLabel;
    QLabel *m_viewLabel;
    
    // 数据
    SatelliteData m_currentData;
    QMap<QString, QList<SatelliteInfo>> m_systemSatellites;
    
    // 绘制参数
    QRect m_chartArea;
    
    // 系统颜色
    QMap<QString, QColor> m_systemColors;
};

#endif // SNRVIEW_H
