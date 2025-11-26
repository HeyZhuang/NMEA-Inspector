#ifndef CHARTMANAGER_H
#define CHARTMANAGER_H

#include <QObject>
#include <QChart>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QScatterSeries>
#include <QSplineSeries>
#include <QValueAxis>
#include <QCategoryAxis>
#include "gnssdata.h"

QT_CHARTS_USE_NAMESPACE

class ChartManager : public QObject
{
    Q_OBJECT

public:
    explicit ChartManager(QObject *parent = nullptr);
    
    // 获取图表视图
    QChartView* getSNRChartView() const { return m_snrChartView; }
    QChartView* getRadarChartView() const { return m_radarChartView; }
    QChartView* getTrajectoryChartView() const { return m_trajectoryChartView; }
    
    // 更新图表数据
    void updateSNRChart(const QList<SatelliteInfo> &satellites);
    void updateRadarChart(const QList<SatelliteInfo> &satellites);
    void updateTrajectoryChart(const QList<GNSSData> &dataHistory);

private:
    void setupSNRChart();
    void setupRadarChart();
    void setupTrajectoryChart();
    
    // 信噪比柱状图
    QChartView *m_snrChartView;
    QChart *m_snrChart;
    QBarSeries *m_snrBarSeries;
    QBarSet *m_snrBarSet;
    
    // 卫星雷达图
    QChartView *m_radarChartView;
    QChart *m_radarChart;
    QScatterSeries *m_radarSeries;
    
    // 运动轨迹图
    QChartView *m_trajectoryChartView;
    QChart *m_trajectoryChart;
    QSplineSeries *m_trajectorySeries;
};

#endif // CHARTMANAGER_H
