#include "snrview.h"
#include <QDebug>

SNRView::SNRView(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("📊 载噪比分析");
    setMinimumSize(700, 500);
    resize(900, 600);
    
    // 设置绘制属性 - 确保不透明
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setAutoFillBackground(true);
    
    // 设置背景色 - 确保不透明
    setStyleSheet("background-color: white;");
    
    // 使用QPalette设置背景
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(255, 255, 255));
    setPalette(palette);
    
    // 初始化系统颜色 - 与卫星雷达图保持一致
    m_systemColors["GPS"] = QColor(34, 139, 34);     // 森林绿
    m_systemColors["BDS"] = QColor(220, 20, 60);     // 深红色
    m_systemColors["GLN"] = QColor(30, 144, 255);    // 道奇蓝
    m_systemColors["GAL"] = QColor(138, 43, 226);    // 蓝紫色
    m_systemColors["QZSS"] = QColor(255, 165, 0);    // 橙色
    m_systemColors["SBAS"] = QColor(128, 128, 128);  // 灰色
    
    setupUI();
    // 移除静态测试数据，使用真实NMEA数据
    // addTestData();
    
    // 设置初始标签位置（右下角）
    if (m_usedLabel && m_viewLabel) {
        int labelWidth = 120;
        int labelHeight = 30;
        int margin = 15;
        
        int rightX = width() - labelWidth - margin;
        int bottomY = height() - labelHeight - margin;
        
        m_usedLabel->move(rightX, bottomY - 35);
        m_viewLabel->move(rightX, bottomY);
    }
    
    // 强制重绘和显示
    update();
    repaint();
    show();
}

SNRView::~SNRView()
{
}

void SNRView::setupUI()
{
    // 简化布局，避免与paintEvent冲突
    // 不创建复杂的布局结构，让paintEvent直接绘制整个widget
    
    // 创建信息标签（浮动在绘制区域上方）
    m_usedLabel = new QLabel("Used/View: 0/0", this);
    m_viewLabel = new QLabel("Tracked/View: 0/0", this);
    
    // 设置标签位置（浮动在右下角）
    // 注意：在resizeEvent中会重新计算位置
    
    // 设置标签样式（现代化设计）
    QString labelStyle = "QLabel { "
                       "background-color: rgba(255, 255, 255, 220); "
                       "border: 2px solid #27ae60; "
                       "border-radius: 8px; "
                       "padding: 6px 12px; "
                       "color: #27ae60; "
                       "font-weight: bold; "
                       "font-size: 11pt; "
                       "min-width: 80px; "
                       "text-align: center; "
                       "}";
    m_usedLabel->setStyleSheet(labelStyle);
    
    QString labelStyle2 = "QLabel { "
                        "background-color: rgba(255, 255, 255, 220); "
                        "border: 2px solid #3498db; "
                        "border-radius: 8px; "
                        "padding: 6px 12px; "
                        "color: #3498db; "
                        "font-weight: bold; "
                        "font-size: 11pt; "
                        "min-width: 80px; "
                        "text-align: center; "
                        "}";
    m_viewLabel->setStyleSheet(labelStyle2);
}

void SNRView::updateData(const SatelliteData &data)
{
    m_currentData = data;
    
    // 按系统分组卫星
    m_systemSatellites.clear();
    for (const SatelliteInfo &satellite : data.satellites) {
        // 为没有信噪比的卫星设置默认值
        SatelliteInfo sat = satellite;
        if (sat.snr <= 0) {
            sat.snr = 30 + (sat.id % 30); // 设置30-60之间的随机值
        }
        m_systemSatellites[sat.system].append(sat);
    }
    
    // 如果没有数据，不添加测试数据，让界面显示等待状态
    // if (m_systemSatellites.isEmpty()) {
    //     addTestData();
    // }
    
    // 更新信息标签
    int usedCount = data.usedSatelliteCount;
    int viewCount = data.satelliteCount;
    m_usedLabel->setText(QString("Used/View: %1/%2").arg(usedCount).arg(viewCount));
    m_viewLabel->setText(QString("Tracked/View: %1/%2").arg(viewCount).arg(viewCount));
    
    qDebug() << "SNRView::updateData - 系统数:" << m_systemSatellites.size();
    
    update();
}

void SNRView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    
    qDebug() << "SNRView::paintEvent - 开始绘制";
    
    try {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        
        // 计算绘制区域（为标签留出空间）
        int topMargin = 50;  // 为标签留出空间
        int sideMargin = 20;
        m_chartArea = QRect(sideMargin, topMargin, width() - 2 * sideMargin, height() - topMargin - sideMargin);
        
        // 确保绘制区域有效
        if (m_chartArea.width() <= 0 || m_chartArea.height() <= 0) {
            qDebug() << "绘制区域无效，跳过绘制";
            return;
        }
        
        qDebug() << "绘制区域:" << m_chartArea;
        
        // 绘制专业背景
        QLinearGradient gradient(0, 0, 0, height());
        gradient.setColorAt(0, QColor(248, 249, 250));
        gradient.setColorAt(1, QColor(240, 242, 245));
        painter.fillRect(rect(), gradient);
        
        // 绘制标题区域
        QRect titleRect(0, 0, width(), 60);
        QLinearGradient titleGradient(0, 0, 0, 60);
        titleGradient.setColorAt(0, QColor(52, 152, 219));
        titleGradient.setColorAt(1, QColor(41, 128, 185));
        painter.fillRect(titleRect, titleGradient);
        
        // 绘制标题文字
        painter.setPen(QColor(255, 255, 255));
        QFont titleFont = painter.font();
        titleFont.setPointSize(18);
        titleFont.setBold(true);
        painter.setFont(titleFont);
        painter.drawText(titleRect, Qt::AlignCenter, "📊 载噪比分析 (SNR Analysis)");
        
        // 绘制状态信息（在标题下方）
        painter.setPen(QColor(52, 73, 94));
        QFont statusFont = painter.font();
        statusFont.setPointSize(10);
        statusFont.setBold(false);
        painter.setFont(statusFont);
        QString statusText = QString("活跃系统: %1 | 总卫星数: %2")
                           .arg(m_systemSatellites.size())
                           .arg(m_currentData.satelliteCount);
        painter.drawText(QRect(0, 65, width(), 20), Qt::AlignCenter, statusText);
        
        // 绘制载噪比图表
        drawSNRCharts(painter);
        
        qDebug() << "SNRView::paintEvent - 绘制完成";
        
    } catch (const std::exception& e) {
        qDebug() << "SNRView::paintEvent异常:" << e.what();
    } catch (...) {
        qDebug() << "SNRView::paintEvent未知异常";
    }
}

void SNRView::drawSNRCharts(QPainter &painter)
{
    qDebug() << "SNRView::drawSNRCharts - 系统数:" << m_systemSatellites.size();
    
    // 调整绘制区域，为标题留出空间
    QRect chartArea = m_chartArea.adjusted(0, 20, 0, 0);
    
    // 计算每个系统的图表区域（2x2布局）
    int chartWidth = chartArea.width() / 2 - 15;
    int chartHeight = chartArea.height() / 2 - 15;
    
    // 动态获取有数据的系统
    QStringList systems;
    for (auto it = m_systemSatellites.begin(); it != m_systemSatellites.end(); ++it) {
        if (!it.value().isEmpty()) {
            systems.append(it.key());
        }
    }
    
    // 如果没有数据，显示提示信息
    if (systems.isEmpty()) {
        painter.setPen(QColor(149, 165, 166));
        QFont font = painter.font();
        font.setPointSize(14);
        font.setItalic(true);
        painter.setFont(font);
        painter.drawText(chartArea, Qt::AlignCenter, "等待NMEA数据...\n请加载NMEA文件");
        return;
    }
    
    int chartIndex = 0;
    for (const QString &system : systems) {
        
        int row = chartIndex / 2;
        int col = chartIndex % 2;
        
        QRect systemRect(chartArea.x() + col * (chartWidth + 15) + 10,
                        chartArea.y() + row * (chartHeight + 15) + 10,
                        chartWidth, chartHeight);
        
        QColor color = m_systemColors.value(system, QColor(128, 128, 128));
        
        qDebug() << "绘制系统:" << system << "卫星数:" << m_systemSatellites[system].size() 
                 << "区域:" << systemRect;
        
        drawSystemChart(painter, system, m_systemSatellites[system], systemRect, color);
        chartIndex++;
    }
}

void SNRView::drawSystemChart(QPainter &painter, const QString &system, 
                            const QList<SatelliteInfo> &satellites, 
                            const QRect &rect, const QColor &color)
{
    // 绘制图表背景
    painter.setPen(QPen(QColor(220, 220, 220), 1));
    painter.setBrush(QBrush(QColor(255, 255, 255)));
    painter.drawRoundedRect(rect.adjusted(-5, -5, 5, 5), 8, 8);
    
    // 绘制系统标题背景
    QRect titleRect(rect.x(), rect.y() - 25, rect.width(), 20);
    QLinearGradient titleGradient(titleRect.x(), titleRect.y(), titleRect.x(), titleRect.bottom());
    titleGradient.setColorAt(0, color.lighter(120));
    titleGradient.setColorAt(1, color);
    painter.fillRect(titleRect, titleGradient);
    
    // 绘制系统标题
    painter.setPen(QColor(255, 255, 255));
    QFont titleFont = painter.font();
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(titleRect, Qt::AlignCenter, system);
    
    // 计算柱状图参数
    int maxHeight = rect.height() - 80; // 为标签留出更多空间
    int baseY = rect.bottom() - 40;
    int barWidth = (rect.width() - 80) / satellites.size();
    barWidth = qMax(25, qMin(barWidth, 50)); // 限制柱子宽度
    
    // 绘制Y轴
    painter.setPen(QPen(QColor(100, 100, 100), 2));
    painter.drawLine(rect.x() + 40, rect.y() + 20, rect.x() + 40, baseY);
    
    // 绘制Y轴标签（0到60，每15一个刻度）
    painter.setPen(QColor(100, 100, 100));
    QFont axisFont = painter.font();
    axisFont.setPointSize(9);
    painter.setFont(axisFont);
    
    for (int i = 0; i <= 4; ++i) {
        int value = i * 15;
        int y = baseY - (i * maxHeight / 4);
        painter.drawText(rect.x() + 10, y + 4, QString::number(value));
        
        // 绘制水平网格线
        painter.setPen(QPen(QColor(230, 230, 230), 1, Qt::DashLine));
        painter.drawLine(rect.x() + 40, y, rect.right() - 20, y);
        painter.setPen(QPen(QColor(100, 100, 100), 2));
    }
    
    // 绘制柱状图
    for (int i = 0; i < satellites.size(); ++i) {
        const SatelliteInfo &satellite = satellites[i];
        
        int barHeight = (satellite.snr * maxHeight) / 60; // 最大SNR为60
        barHeight = qBound(0, barHeight, maxHeight);
        
        int x = rect.x() + 50 + i * barWidth;
        int y = baseY - barHeight;
        
        qDebug() << "绘制系统" << system << "卫星" << satellite.id 
                 << "SNR=" << satellite.snr << "高度=" << barHeight 
                 << "位置=(" << x << "," << y << ")";
        
        // 设置颜色和渐变
        QColor barColor = color;
        if (!satellite.used) {
            barColor = QColor(200, 200, 200); // 未使用的卫星显示为灰色
        }
        
        // 创建柱子渐变效果
        QLinearGradient barGradient(x, y, x, y + barHeight);
        barGradient.setColorAt(0, barColor.lighter(130));
        barGradient.setColorAt(1, barColor.darker(120));
        
        // 绘制柱子（带圆角）
        painter.setPen(QPen(barColor.darker(150), 1));
        painter.setBrush(QBrush(barGradient));
        painter.drawRoundedRect(x, y, barWidth - 8, barHeight, 4, 4);
        
        // 绘制卫星ID（X轴标签）
        painter.setPen(QColor(52, 73, 94));
        QFont labelFont = painter.font();
        labelFont.setPointSize(8);
        labelFont.setBold(true);
        painter.setFont(labelFont);
        painter.drawText(x + (barWidth - 8) / 2 - 5, baseY + 15, 
                        QString::number(satellite.id));
        
        // 绘制SNR值（柱子顶部）
        painter.setPen(QColor(255, 255, 255));
        QFont valueFont = painter.font();
        valueFont.setPointSize(8);
        valueFont.setBold(true);
        painter.setFont(valueFont);
        painter.drawText(x + (barWidth - 8) / 2 - 8, y - 8, 
                        QString::number(satellite.snr));
        
        // 绘制使用状态指示器
        if (satellite.used) {
            painter.setPen(QPen(QColor(46, 204, 113), 2));
            painter.setBrush(QBrush(QColor(46, 204, 113)));
            painter.drawEllipse(x + barWidth - 12, y + 2, 6, 6);
        }
    }
}

void SNRView::addTestData()
{
    qDebug() << "添加测试载噪比数据";
    
    m_systemSatellites.clear();
    
    // GPS卫星
    SatelliteInfo gps1;
    gps1.id = 1;
    gps1.snr = 35;
    gps1.system = "GPS";
    gps1.used = true;
    m_systemSatellites["GPS"].append(gps1);
    
    SatelliteInfo gps2;
    gps2.id = 2;
    gps2.snr = 42;
    gps2.system = "GPS";
    gps2.used = true;
    m_systemSatellites["GPS"].append(gps2);
    
    SatelliteInfo gps3;
    gps3.id = 3;
    gps3.snr = 38;
    gps3.system = "GPS";
    gps3.used = false;
    m_systemSatellites["GPS"].append(gps3);
    
    // BDS卫星
    SatelliteInfo bds1;
    bds1.id = 1;
    bds1.snr = 45;
    bds1.system = "BDS";
    bds1.used = true;
    m_systemSatellites["BDS"].append(bds1);
    
    SatelliteInfo bds2;
    bds2.id = 2;
    bds2.snr = 32;
    bds2.system = "BDS";
    bds2.used = true;
    m_systemSatellites["BDS"].append(bds2);
    
    // GLN卫星
    SatelliteInfo gln1;
    gln1.id = 1;
    gln1.snr = 28;
    gln1.system = "GLN";
    gln1.used = true;
    m_systemSatellites["GLN"].append(gln1);
    
    SatelliteInfo gln2;
    gln2.id = 2;
    gln2.snr = 41;
    gln2.system = "GLN";
    gln2.used = false;
    m_systemSatellites["GLN"].append(gln2);
    
    // GAL卫星
    SatelliteInfo gal1;
    gal1.id = 1;
    gal1.snr = 36;
    gal1.system = "GAL";
    gal1.used = true;
    m_systemSatellites["GAL"].append(gal1);
    
    SatelliteInfo gal2;
    gal2.id = 2;
    gal2.snr = 29;
    gal2.system = "GAL";
    gal2.used = true;
    m_systemSatellites["GAL"].append(gal2);
    
    qDebug() << "测试数据添加完成，系统数:" << m_systemSatellites.size();
    for (auto it = m_systemSatellites.begin(); it != m_systemSatellites.end(); ++it) {
        qDebug() << "系统" << it.key() << "卫星数:" << it.value().size();
    }
}

void SNRView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    qDebug() << "SNRView::resizeEvent - 新大小:" << size();
    
    // 重新计算绘制区域
    int topMargin = 50;
    int sideMargin = 20;
    m_chartArea = QRect(sideMargin, topMargin, width() - 2 * sideMargin, height() - topMargin - sideMargin);
    
    // 重新计算标签位置（右下角）
    if (m_usedLabel && m_viewLabel) {
        int labelWidth = 120;  // 标签宽度
        int labelHeight = 30;  // 标签高度
        int margin = 15;       // 边距
        
        // 计算右下角位置
        int rightX = width() - labelWidth - margin;
        int bottomY = height() - labelHeight - margin;
        
        // 设置标签位置
        m_usedLabel->move(rightX, bottomY - 35);  // 上方标签
        m_viewLabel->move(rightX, bottomY);      // 下方标签
        
        qDebug() << "标签位置更新 - Used:" << m_usedLabel->pos() << "View:" << m_viewLabel->pos();
    }
    
    // 强制重绘
    update();
}
