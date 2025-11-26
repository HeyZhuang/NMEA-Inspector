#include "chartmanager.h"
#include <QDebug>

ChartManager::ChartManager(QObject *parent)
    : QObject(parent)
{
    setupSNRChart();
    setupRadarChart();
    setupTrajectoryChart();
}

void ChartManager::setupSNRChart()
{
    m_snrChart = new QChart();
    m_snrChart->setTitle("卫星信噪比 (SNR)");
    m_snrChart->setAnimationOptions(QChart::SeriesAnimations);
    
    m_snrBarSeries = new QBarSeries();
    m_snrBarSet = new QBarSet("信噪比 (dB)");
    m_snrBarSet->setColor(QColor(255, 99, 132)); // 红色
    m_snrBarSeries->append(m_snrBarSet);
    m_snrChart->addSeries(m_snrBarSeries);
    
    // 设置坐标轴
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 50);
    axisY->setTitleText("信噪比 (dB)");
    axisY->setTickCount(6);
    m_snrChart->addAxis(axisY, Qt::AlignLeft);
    m_snrBarSeries->attachAxis(axisY);
    
    QCategoryAxis *axisX = new QCategoryAxis();
    axisX->setTitleText("卫星ID");
    m_snrChart->addAxis(axisX, Qt::AlignBottom);
    m_snrBarSeries->attachAxis(axisX);
    
    m_snrChartView = new QChartView(m_snrChart);
    m_snrChartView->setRenderHint(QPainter::Antialiasing);
    m_snrChartView->setMinimumHeight(200);
}

void ChartManager::setupRadarChart()
{
    m_radarChart = new QChart();
    m_radarChart->setTitle("卫星雷达图");
    m_radarChart->setAnimationOptions(QChart::SeriesAnimations);
    
    m_radarSeries = new QScatterSeries();
    m_radarSeries->setName("卫星");
    m_radarSeries->setMarkerSize(12);
    m_radarSeries->setColor(QColor(54, 162, 235)); // 蓝色
    m_radarChart->addSeries(m_radarSeries);
    
    // 设置坐标轴
    QValueAxis *radarAxisX = new QValueAxis();
    radarAxisX->setRange(-180, 180);
    radarAxisX->setTitleText("方位角 (°)");
    radarAxisX->setTickCount(9);
    m_radarChart->addAxis(radarAxisX, Qt::AlignBottom);
    m_radarSeries->attachAxis(radarAxisX);
    
    QValueAxis *radarAxisY = new QValueAxis();
    radarAxisY->setRange(0, 90);
    radarAxisY->setTitleText("高度角 (°)");
    radarAxisY->setTickCount(10);
    m_radarChart->addAxis(radarAxisY, Qt::AlignLeft);
    m_radarSeries->attachAxis(radarAxisY);
    
    m_radarChartView = new QChartView(m_radarChart);
    m_radarChartView->setRenderHint(QPainter::Antialiasing);
    m_radarChartView->setMinimumHeight(200);
}

void ChartManager::setupTrajectoryChart()
{
    m_trajectoryChart = new QChart();
    m_trajectoryChart->setTitle("运动轨迹图");
    m_trajectoryChart->setAnimationOptions(QChart::SeriesAnimations);
    
    m_trajectorySeries = new QSplineSeries();
    m_trajectorySeries->setName("轨迹");
    m_trajectorySeries->setColor(QColor(75, 192, 192)); // 青色
    m_trajectoryChart->addSeries(m_trajectorySeries);
    
    // 设置坐标轴
    QValueAxis *trajAxisX = new QValueAxis();
    trajAxisX->setTitleText("经度 (°)");
    m_trajectoryChart->addAxis(trajAxisX, Qt::AlignBottom);
    m_trajectorySeries->attachAxis(trajAxisX);
    
    QValueAxis *trajAxisY = new QValueAxis();
    trajAxisY->setTitleText("纬度 (°)");
    m_trajectoryChart->addAxis(trajAxisY, Qt::AlignLeft);
    m_trajectorySeries->attachAxis(trajAxisY);
    
    m_trajectoryChartView = new QChartView(m_trajectoryChart);
    m_trajectoryChartView->setRenderHint(QPainter::Antialiasing);
    m_trajectoryChartView->setMinimumHeight(200);
}

void ChartManager::updateSNRChart(const QList<SatelliteInfo> &satellites)
{
    if (satellites.isEmpty()) {
        return;
    }
    
    // 清空现有数据
    m_snrBarSet->remove(0, m_snrBarSet->count());
    
    // 简化：直接显示所有有效卫星，最多8个
    QStringList categories;
    QList<double> snrValues;
    
    for (const SatelliteInfo &satellite : satellites) {
        if (satellite.snr > 0 && snrValues.size() < 8) {
            snrValues.append(satellite.snr);
            categories.append(QString::number(satellite.id));
        }
    }
    
    // 添加数据到柱状图
    for (double snr : snrValues) {
        *m_snrBarSet << snr;
    }
    
    // 更新X轴标签
    QCategoryAxis *axisX = qobject_cast<QCategoryAxis*>(m_snrChart->axes(Qt::Horizontal).first());
    if (axisX) {
        // 移除所有现有标签
        QStringList existingLabels = axisX->categoriesLabels();
        for (const QString &label : existingLabels) {
            axisX->remove(label);
        }
        
        // 添加新的标签
        for (int i = 0; i < categories.size(); ++i) {
            axisX->append(categories[i], i);
        }
    }
    
    // 自动调整Y轴范围
    if (!snrValues.isEmpty()) {
        double maxSNR = *std::max_element(snrValues.begin(), snrValues.end());
        QValueAxis *axisY = qobject_cast<QValueAxis*>(m_snrChart->axes(Qt::Vertical).first());
        if (axisY) {
            axisY->setRange(0, maxSNR + 5);
        }
    }
}

void ChartManager::updateRadarChart(const QList<SatelliteInfo> &satellites)
{
    if (satellites.isEmpty()) {
        return;
    }
    
    // 清空现有数据
    m_radarSeries->clear();
    
    // 添加所有有效卫星数据
    for (const SatelliteInfo &satellite : satellites) {
        if (satellite.elevation >= 0 && satellite.azimuth >= 0) {
            m_radarSeries->append(satellite.azimuth, satellite.elevation);
        }
    }
    
    // 设置散点样式
    m_radarSeries->setMarkerSize(15);
    m_radarSeries->setColor(QColor(54, 162, 235));
    m_radarSeries->setBorderColor(QColor(255, 255, 255));
    m_radarSeries->setPen(QPen(QColor(54, 162, 235), 2));
}

void ChartManager::updateTrajectoryChart(const QList<GNSSData> &dataHistory)
{
    if (dataHistory.isEmpty()) {
        return;
    }
    
    // 清空现有数据
    m_trajectorySeries->clear();
    
    // 添加轨迹点
    double minLon = 180, maxLon = -180;
    double minLat = 90, maxLat = -90;
    bool hasValidData = false;
    
    for (const GNSSData &data : dataHistory) {
        if (data.latitude != 0.0 && data.longitude != 0.0) {
            m_trajectorySeries->append(data.longitude, data.latitude);
            hasValidData = true;
            
            // 计算边界
            minLon = qMin(minLon, data.longitude);
            maxLon = qMax(maxLon, data.longitude);
            minLat = qMin(minLat, data.latitude);
            maxLat = qMax(maxLat, data.latitude);
        }
    }
    
    // 自动调整坐标轴范围
    if (hasValidData && m_trajectorySeries->count() > 0) {
        double lonRange = maxLon - minLon;
        double latRange = maxLat - minLat;
        
        // 如果范围太小，设置最小范围
        if (lonRange < 0.0001) {
            lonRange = 0.0001; // 最小0.0001度范围
        }
        if (latRange < 0.0001) {
            latRange = 0.0001; // 最小0.0001度范围
        }
        
        // 添加一些边距
        double lonMargin = lonRange * 0.5; // 增加边距到50%
        double latMargin = latRange * 0.5;
        
        QValueAxis *axisX = qobject_cast<QValueAxis*>(m_trajectoryChart->axes(Qt::Horizontal).first());
        QValueAxis *axisY = qobject_cast<QValueAxis*>(m_trajectoryChart->axes(Qt::Vertical).first());
        
        if (axisX) {
            axisX->setRange(minLon - lonMargin, maxLon + lonMargin);
        }
        if (axisY) {
            axisY->setRange(minLat - latMargin, maxLat + latMargin);
        }
        
        // 设置轨迹线样式
        m_trajectorySeries->setPen(QPen(QColor(75, 192, 192), 3));
        m_trajectorySeries->setPointsVisible(true);
    }
}
