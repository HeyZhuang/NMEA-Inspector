#include "satelliteview.h"
#include <QDebug>
#include <cmath>
#include <qmath.h>

// 兼容性函数
inline qreal deg2rad(qreal deg) { return deg * (M_PI / 180.0); }

SatelliteView::SatelliteView(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("🛰️ 卫星雷达图");
    setMinimumSize(600, 600);
    resize(700, 700);

    // 设置窗口标志 - 作为普通控件
    setWindowFlags(Qt::Widget);
    
    // 设置绘制属性 - 确保不透明
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setAutoFillBackground(true);
    
    // 设置背景色 - 确保不透明
    setStyleSheet("background-color: white;");
    
    // 使用QPalette设置背景
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(255, 255, 255));
    setPalette(palette);
    
    // 初始化系统颜色 - 更鲜明的多系统配色
    m_systemColors["GPS"] = QColor(34, 139, 34);     // 森林绿
    m_systemColors["BDS"] = QColor(220, 20, 60);     // 深红色
    m_systemColors["GLN"] = QColor(30, 144, 255);    // 道奇蓝
    m_systemColors["GAL"] = QColor(138, 43, 226);    // 蓝紫色
    m_systemColors["QZSS"] = QColor(255, 165, 0);    // 橙色
    m_systemColors["SBAS"] = QColor(128, 128, 128);  // 灰色
    
    setupUI();
    addTestData();
    
    // 强制重绘和显示
    update();
    repaint();
    show();
    
    // 调试信息
    // qDebug() << "SatelliteView构造函数完成 - 大小:" << size()
    //          << "可见性:" << isVisible()
    //          << "窗口标志:" << windowFlags()
    //          << "父控件:" << parent();
}

SatelliteView::~SatelliteView()
{
}

void SatelliteView::setupUI()
{
    // 简化布局，避免与paintEvent冲突
    // 不创建复杂的布局结构，让paintEvent直接绘制整个widget
    
    // 创建系统选择复选框（浮动在绘制区域上方）
    m_gpsCheckBox = new QCheckBox("GPS", this);
    m_bdsCheckBox = new QCheckBox("BDS", this);
    m_glnCheckBox = new QCheckBox("GLN", this);
    m_galCheckBox = new QCheckBox("GAL", this);
    
    // 默认全部选中
    m_gpsCheckBox->setChecked(true);
    m_bdsCheckBox->setChecked(true);
    m_glnCheckBox->setChecked(true);
    m_galCheckBox->setChecked(true);
    
    // 设置复选框位置（浮动在左上角）
    m_gpsCheckBox->move(10, 10);
    m_bdsCheckBox->move(110, 10);
    m_glnCheckBox->move(210, 10);
    m_galCheckBox->move(310, 10);
    
    // 设置复选框样式
    QString checkBoxStyle = "QCheckBox { background-color: rgba(255, 255, 255, 200); "
                           "border: 1px solid #ccc; padding: 2px; }";
    m_gpsCheckBox->setStyleSheet(checkBoxStyle);
    m_bdsCheckBox->setStyleSheet(checkBoxStyle);
    m_glnCheckBox->setStyleSheet(checkBoxStyle);
    m_galCheckBox->setStyleSheet(checkBoxStyle);
    
    // 连接信号
    connect(m_gpsCheckBox, &QCheckBox::toggled, this, &SatelliteView::onSystemToggled);
    connect(m_bdsCheckBox, &QCheckBox::toggled, this, &SatelliteView::onSystemToggled);
    connect(m_glnCheckBox, &QCheckBox::toggled, this, &SatelliteView::onSystemToggled);
    connect(m_galCheckBox, &QCheckBox::toggled, this, &SatelliteView::onSystemToggled);
}

void SatelliteView::updateData(const SatelliteData &data)
{
    m_currentData = data;
    m_visibleSatellites = data.satellites;
    
    qDebug() << "SatelliteView::updateData - 卫星数:" << m_visibleSatellites.size();
    
    // 如果没有数据，添加测试数据
    if (m_visibleSatellites.isEmpty()) {
        addTestData();
    }
    
    update();
}

void SatelliteView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    qDebug() << "SatelliteView::paintEvent - 开始绘制" 
             << "窗口大小:" << size() 
             << "可见性:" << isVisible()
             << "窗口标志:" << windowFlags();
    
    try {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        
        // 绘制明显的测试背景
        painter.fillRect(rect(), QColor(240, 240, 240));  // 浅灰色背景
        
        // 绘制边框
        painter.setPen(QPen(QColor(255, 0, 0), 5));
        painter.drawRect(rect().adjusted(5, 5, -5, -5));
        
        // 绘制标题
        painter.setPen(QColor(0, 0, 255));
        QFont titleFont = painter.font();
        titleFont.setPointSize(16);
        titleFont.setBold(true);
        painter.setFont(titleFont);
        painter.drawText(rect().adjusted(0, 0, 0, -height()/2), Qt::AlignCenter, "🛰️ 卫星雷达图");
        
        // 绘制状态信息
        painter.setPen(QColor(0, 150, 0));
        QFont statusFont = painter.font();
        statusFont.setPointSize(12);
        painter.setFont(statusFont);
        QString statusText = QString("卫星数量: %1 | 绘制区域: %2x%3")
                           .arg(m_visibleSatellites.size())
                           .arg(m_chartRect.width())
                           .arg(m_chartRect.height());
        painter.drawText(rect().adjusted(0, height()/2, 0, 0), Qt::AlignCenter, statusText);
        
        // 计算绘制区域（为复选框留出空间）
        int topMargin = 50;  // 为复选框留出空间
        int sideMargin = 60;
        m_chartRect = QRect(sideMargin, topMargin, width() - 2 * sideMargin, height() - topMargin - sideMargin);
        m_centerX = m_chartRect.center().x();
        m_centerY = m_chartRect.center().y();
        m_radius = qMin(m_chartRect.width(), m_chartRect.height()) / 2 - 20;
        
        // 确保绘制区域有效
        if (m_chartRect.width() <= 0 || m_chartRect.height() <= 0 || m_radius <= 0) {
            qDebug() << "绘制区域无效，跳过绘制";
            return;
        }
        
        qDebug() << "绘制区域:" << m_chartRect << "中心:" << m_centerX << m_centerY << "半径:" << m_radius;
        
        // 绘制雷达图
        drawRadarChart(painter);
        
        qDebug() << "SatelliteView::paintEvent - 绘制完成";
        
    } catch (const std::exception& e) {
        qDebug() << "SatelliteView::paintEvent异常:" << e.what();
    } catch (...) {
        qDebug() << "SatelliteView::paintEvent未知异常";
    }
}

void SatelliteView::drawRadarChart(QPainter &painter)
{
    // 绘制整个窗口背景
    painter.fillRect(rect(), QColor(255, 255, 255));
    
    // 绘制图表背景
    painter.fillRect(m_chartRect, QColor(245, 245, 245));
    
    // 保存painter状态
    painter.save();
    
    // 坐标变换到中心
    painter.translate(m_centerX, m_centerY);
    
    // 绘制参考圆（外圆）
    painter.setPen(QPen(QColor(100, 100, 100), 2));
    painter.drawEllipse(QPointF(0, 0), qreal(m_radius), qreal(m_radius));
    
    // 绘制网格
    drawGrid(painter);
    
    // 绘制卫星
    drawSatellites(painter);
    
    // 恢复painter状态
    painter.restore();
    
    // 绘制标签（在原始坐标系中）
    drawLabels(painter);
    
    // 绘制颜色图例
    drawColorLegend(painter);
}

void SatelliteView::drawGrid(QPainter &painter)
{
    painter.setPen(QPen(QColor(200, 200, 200), 1, Qt::DashLine));
    
    // 绘制同心圆（仰角：10°到80°，每10°一个）
    for (int i = 1; i <= 8; ++i) {
        double elevation = i * 10; // 10°, 20°, ..., 80°
        double radius = m_radius * (90 - elevation) / 90.0;
        painter.drawEllipse(QPointF(0, 0), qreal(radius), qreal(radius));
    }
    
    // 绘制径向线（方位角：每15°一条）
    painter.setPen(QPen(QColor(200, 200, 200), 1, Qt::SolidLine));
    for (int i = 0; i < 24; ++i) {
        double azimuth = i * 15; // 0°, 15°, 30°, ..., 345°
        double rad = deg2rad(azimuth);
        int x = m_radius * sin(rad);
        int y = -m_radius * cos(rad);
        painter.drawLine(0, 0, x, y);
    }
}

void SatelliteView::drawLabels(QPainter &painter)
{
    painter.setPen(QColor(100, 100, 100));
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);
    
    // 绘制仰角标签
    for (int i = 1; i <= 8; ++i) {
        double elevation = i * 10;
        double radius = (elevation / 90.0) * m_radius;
        painter.drawText(m_centerX + radius + 5, m_centerY - 3, 
                        QString("%1°").arg(elevation));
    }
    
    // 绘制方位角标签
    QStringList azimuthLabels = {"N", "15°", "30°", "45°", "60°", "75°", "E", 
                                "105°", "120°", "135°", "150°", "165°", "S", 
                                "195°", "210°", "225°", "240°", "255°", "W", 
                                "285°", "300°", "315°", "330°", "345°","360°"};
    
    for (int i = 0; i < 24; ++i) {
        double azimuth = i * 15;
        double rad = azimuth * 3.14159265359 / 180.0;
        int x = m_centerX + (m_radius + 20) * sin(rad);
        int y = m_centerY - (m_radius + 20) * cos(rad);
        
        // 安全访问QStringList，避免越界
        QString label;
        if (i < azimuthLabels.size()) {
            label = azimuthLabels[i];
        } else {
            label = QString("%1°").arg(azimuth);
        }
        
        QRect textRect = painter.fontMetrics().boundingRect(label);
        painter.drawText(x - textRect.width()/2, y + textRect.height()/2, label);
    }
}

void SatelliteView::drawSatellites(QPainter &painter)
{
    qDebug() << "绘制卫星，数量:" << m_visibleSatellites.size();
    
    for (const SatelliteInfo &satellite : m_visibleSatellites) {
        // 检查系统是否被选中
        bool systemVisible = false;
        if (satellite.system == "GPS" && m_gpsCheckBox->isChecked()) systemVisible = true;
        if (satellite.system == "BDS" && m_bdsCheckBox->isChecked()) systemVisible = true;
        if (satellite.system == "GLN" && m_glnCheckBox->isChecked()) systemVisible = true;
        if (satellite.system == "GAL" && m_galCheckBox->isChecked()) systemVisible = true;
        
        if (!systemVisible) continue;
        
        // 计算卫星位置（在变换后的坐标系中）
        double radius = m_radius * (90 - satellite.elevation) / 90.0;
        double azimuthRad = deg2rad(satellite.azimuth);
        int x = radius * sin(azimuthRad);
        int y = -radius * cos(azimuthRad);
        
        // 设置颜色
        QColor color = m_systemColors.value(satellite.system, QColor(128, 128, 128));
        if (!satellite.used) {
            color = QColor(200, 200, 200); // 未使用的卫星显示为灰色
        }
        
        // 绘制卫星圆圈
        painter.setPen(QPen(color, 2));
        painter.setBrush(QBrush(color));
        painter.drawEllipse(x - 8, y - 8, 16, 16);
        
        // 绘制卫星ID
        painter.setPen(QColor(255, 255, 255));
        QFont font = painter.font();
        font.setPointSize(8);
        font.setBold(true);
        painter.setFont(font);
        
        QString satId = QString("%1%2").arg(satellite.system.left(1)).arg(satellite.id);
        QRect textRect(x - 8, y - 8, 16, 16);
        painter.drawText(textRect, Qt::AlignCenter, satId);
        
        // 绘制信噪比
        if (satellite.snr > 0) {
            painter.setPen(QColor(0, 0, 0));
            font.setPointSize(6);
            painter.setFont(font);
            painter.drawText(x + 12, y - 5, QString::number(satellite.snr));
        }
        
        qDebug() << "绘制卫星" << satellite.id << "系统" << satellite.system 
                 << "位置(" << x << "," << y << ")";
    }
}

void SatelliteView::drawColorLegend(QPainter &painter)
{
    // 绘制多系统统计信息
    int legendX = m_chartRect.right() - 120;
    int legendY = m_chartRect.top() + 20;
    int itemHeight = 20;
    int itemSpacing = 5;
    
    // 统计各系统卫星数量
    QMap<QString, int> systemCount;
    QMap<QString, int> systemUsedCount;
    for (const SatelliteInfo &sat : m_currentData.satellites) {
        systemCount[sat.system]++;
        if (sat.used) {
            systemUsedCount[sat.system]++;
        }
    }
    
    // 绘制背景
    painter.setPen(QPen(QColor(240, 240, 240), 1));
    painter.setBrush(QBrush(QColor(250, 250, 250, 200)));
    int legendHeight = (systemCount.size() + 1) * (itemHeight + itemSpacing) + 10;
    painter.drawRoundedRect(legendX - 5, legendY - 5, 115, legendHeight, 5, 5);
    
    // 绘制标题
    painter.setPen(QColor(50, 50, 50));
    QFont titleFont = painter.font();
    titleFont.setPointSize(10);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(legendX, legendY + 15, "🛰️ 卫星系统统计");
    
    int currentY = legendY + 25;
    
    // 绘制各系统信息
    QFont font = painter.font();
    font.setPointSize(9);
    painter.setFont(font);
    
    for (auto it = systemCount.begin(); it != systemCount.end(); ++it) {
        QString systemName = it.key();
        int totalCount = it.value();
        int usedCount = systemUsedCount.value(systemName, 0);
        
        // 绘制系统颜色标识
        QColor systemColor = m_systemColors.value(systemName, QColor(128, 128, 128));
        painter.setPen(QPen(systemColor, 2));
        painter.setBrush(QBrush(systemColor));
        painter.drawEllipse(legendX, currentY - 8, 12, 12);
        
        // 绘制系统名称和数量
        painter.setPen(QColor(50, 50, 50));
        QString systemInfo = QString("%1: %2/%3").arg(systemName).arg(usedCount).arg(totalCount);
        painter.drawText(legendX + 18, currentY + 3, systemInfo);
        
        currentY += itemHeight + itemSpacing;
    }
    
    // 绘制总计信息
    painter.setPen(QColor(30, 30, 30));
    font.setBold(true);
    painter.setFont(font);
    int totalSatellites = m_currentData.satellites.size();
    int totalUsed = 0;
    for (int count : systemUsedCount.values()) {
        totalUsed += count;
    }
    painter.drawText(legendX, currentY + 3, QString("总计: %1/%2").arg(totalUsed).arg(totalSatellites));
}

QPoint SatelliteView::calculateSatellitePosition(double elevation, double azimuth)
{
    // 将仰角转换为半径（0°=边缘，90°=中心）
    double radius = m_radius * (90 - elevation) / 90.0;
    
    // 将方位角转换为弧度
    double azimuthRad = deg2rad(azimuth);
    
    // 计算位置（注意Y轴需要翻转）
    int x = m_centerX + radius * sin(azimuthRad);
    int y = m_centerY - radius * cos(azimuthRad);
    
    return QPoint(x, y);
}

void SatelliteView::addTestData()
{
    qDebug() << "添加测试卫星数据";
    
    m_visibleSatellites.clear();
    
    // 添加测试卫星
    SatelliteInfo sat1;
    sat1.id = 1;
    sat1.elevation = 45;
    sat1.azimuth = 0;
    sat1.snr = 35;
    sat1.system = "GPS";
    sat1.used = true;
    m_visibleSatellites.append(sat1);
    
    SatelliteInfo sat2;
    sat2.id = 2;
    sat2.elevation = 30;
    sat2.azimuth = 90;
    sat2.snr = 42;
    sat2.system = "GPS";
    sat2.used = true;
    m_visibleSatellites.append(sat2);
    
    SatelliteInfo sat3;
    sat3.id = 3;
    sat3.elevation = 60;
    sat3.azimuth = 180;
    sat3.snr = 38;
    sat3.system = "BDS";
    sat3.used = true;
    m_visibleSatellites.append(sat3);
    
    SatelliteInfo sat4;
    sat4.id = 4;
    sat4.elevation = 75;
    sat4.azimuth = 270;
    sat4.snr = 45;
    sat4.system = "GLN";
    sat4.used = true;
    m_visibleSatellites.append(sat4);
    
    SatelliteInfo sat5;
    sat5.id = 5;
    sat5.elevation = 20;
    sat5.azimuth = 45;
    sat5.snr = 28;
    sat5.system = "GAL";
    sat5.used = false;
    m_visibleSatellites.append(sat5);
    
    qDebug() << "测试数据添加完成，共" << m_visibleSatellites.size() << "个卫星";
}

void SatelliteView::onSystemToggled()
{
    qDebug() << "系统选择改变，重新绘制";
    update();
    repaint();
}

void SatelliteView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    qDebug() << "SatelliteView::resizeEvent - 新大小:" << size();
    
    // 重新计算绘制区域
    int topMargin = 50;
    int sideMargin = 60;
    m_chartRect = QRect(sideMargin, topMargin, width() - 2 * sideMargin, height() - topMargin - sideMargin);
    m_centerX = m_chartRect.center().x();
    m_centerY = m_chartRect.center().y();
    m_radius = qMin(m_chartRect.width(), m_chartRect.height()) / 2 - 20;
    
    // 强制重绘
    update();
}

