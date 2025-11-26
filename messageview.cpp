#include "messageview.h"
#include <QHeaderView>
#include <QTreeWidgetItem>
#include <QMap>
#include <QDebug>

MessageView::MessageView(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("📋 消息视图 - NMEA字段详情");
    setMinimumSize(900, 700);
    resize(1100, 800);
    // 使用Emoji作为窗口图标
    
    // 设置窗口属性
    setAttribute(Qt::WA_DeleteOnClose, false);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    setupUI();
}

void MessageView::setupUI()
{
    m_splitter = new QSplitter(Qt::Horizontal, this);
    m_splitter->setStyleSheet("QSplitter::handle { background-color: #3498db; }");
    
    // 左侧树形视图
    m_treeGroup = new QGroupBox("📋 NMEA消息类型");
    m_treeGroup->setStyleSheet("QGroupBox { font-weight: bold; font-size: 11pt; }");
    QVBoxLayout *treeLayout = new QVBoxLayout(m_treeGroup);
    treeLayout->setContentsMargins(8, 8, 8, 8);
    m_treeWidget = new QTreeWidget();
    m_treeWidget->setStyleSheet("QTreeWidget { font-size: 9pt; }");
    treeLayout->addWidget(m_treeWidget);
    
    // 右侧表格视图
    m_tableGroup = new QGroupBox("📊 字段详情");
    m_tableGroup->setStyleSheet("QGroupBox { font-weight: bold; font-size: 11pt; }");
    QVBoxLayout *tableLayout = new QVBoxLayout(m_tableGroup);
    tableLayout->setContentsMargins(8, 8, 8, 8);
    m_tableWidget = new QTableWidget();
    m_tableWidget->setStyleSheet("QTableWidget { font-size: 9pt; }");
    tableLayout->addWidget(m_tableWidget);
    
    m_splitter->addWidget(m_treeGroup);
    m_splitter->addWidget(m_tableGroup);
    m_splitter->setSizes({350, 750});
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->addWidget(m_splitter);
    
    setupTreeWidget();
    setupTableWidget();
    
    // 连接信号
    connect(m_treeWidget, &QTreeWidget::itemClicked, this, &MessageView::onTreeItemClicked);
}

void MessageView::setupTreeWidget()
{
    m_treeWidget->setHeaderLabel("📋 NMEA消息类型");
    
    // 创建根节点
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(m_treeWidget);
    rootItem->setText(0, "🛰️ NMEA");
    rootItem->setExpanded(true);
    
    // GGA消息
    QTreeWidgetItem *ggaItem = new QTreeWidgetItem(rootItem);
    ggaItem->setText(0, "📍 GGA - 定位数据");
    
    // GLL消息
    QTreeWidgetItem *gllItem = new QTreeWidgetItem(rootItem);
    gllItem->setText(0, "🌐 GLL - 地理位置");
    
    // GSA消息
    QTreeWidgetItem *gsaItem = new QTreeWidgetItem(rootItem);
    gsaItem->setText(0, "🎯 GSA - 精度因子");
    
    QTreeWidgetItem *gsaGpsItem = new QTreeWidgetItem(gsaItem);
    gsaGpsItem->setText(0, "🛰️ GPS");
    
    QTreeWidgetItem *gsaBdsItem = new QTreeWidgetItem(gsaItem);
    gsaBdsItem->setText(0, "🛰️ BDS");
    
    QTreeWidgetItem *gsaGlnItem = new QTreeWidgetItem(gsaItem);
    gsaGlnItem->setText(0, "🛰️ GLN");
    
    // GSV消息
    QTreeWidgetItem *gsvItem = new QTreeWidgetItem(rootItem);
    gsvItem->setText(0, "📡 GSV - 卫星信息");
    
    QTreeWidgetItem *gsvGpsItem = new QTreeWidgetItem(gsvItem);
    gsvGpsItem->setText(0, "🛰️ GPS");
    
    QTreeWidgetItem *gsvBdsItem = new QTreeWidgetItem(gsvItem);
    gsvBdsItem->setText(0, "🛰️ BDS");
    
    QTreeWidgetItem *gsvGlnItem = new QTreeWidgetItem(gsvItem);
    gsvGlnItem->setText(0, "🛰️ GLN");
    
    // RMC消息
    QTreeWidgetItem *rmcItem = new QTreeWidgetItem(rootItem);
    rmcItem->setText(0, "🚀 RMC - 推荐最小数据");
    
    // VTG消息
    QTreeWidgetItem *vtgItem = new QTreeWidgetItem(rootItem);
    vtgItem->setText(0, "🧭 VTG - 航向速度");
    
    // ZDA消息
    QTreeWidgetItem *zdaItem = new QTreeWidgetItem(rootItem);
    zdaItem->setText(0, "🕐 ZDA - 时间日期");
    
    m_treeWidget->expandAll();
}

void MessageView::setupTableWidget()
{
    m_tableWidget->setColumnCount(2);
    m_tableWidget->setHorizontalHeaderLabels({"📋 字段名称", "📊 字段值"});
    m_tableWidget->horizontalHeader()->setStretchLastSection(true);
    m_tableWidget->setAlternatingRowColors(true);
    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableWidget->setStyleSheet("QTableWidget { gridline-color: #bdc3c7; }");
}

void MessageView::updateData(const SatelliteData &data)
{
    m_currentData = data;
    
    // 调试信息
    qDebug() << "MessageView::updateData - 卫星数:" << data.satelliteCount 
             << "纬度:" << data.latitude << "经度:" << data.longitude;
    
    // 更新树形控件显示
    updateTreeData();
    
    // 显示全部信息到表格
    updateTableData("全部信息");
    
    // 强制刷新视图
    m_treeWidget->update();
    m_tableWidget->update();
}

void MessageView::updateTreeData()
{
    // 清空现有数据
    m_treeWidget->clear();
    
    // 创建根节点
    QTreeWidgetItem *basicInfo = new QTreeWidgetItem(m_treeWidget);
    basicInfo->setText(0, "📍 基本信息");
    
    QTreeWidgetItem *positionInfo = new QTreeWidgetItem(m_treeWidget);
    positionInfo->setText(0, "🗺️ 位置信息");
    
    QTreeWidgetItem *satelliteInfo = new QTreeWidgetItem(m_treeWidget);
    satelliteInfo->setText(0, "🛰️ 卫星信息");
    
    QTreeWidgetItem *qualityInfo = new QTreeWidgetItem(m_treeWidget);
    qualityInfo->setText(0, "📊 质量信息");
    
    // 添加基本信息
    addTreeItem(basicInfo, "时间", m_currentData.time);
    addTreeItem(basicInfo, "日期", m_currentData.date);
    addTreeItem(basicInfo, "定位类型", m_currentData.fixType);
    
    // 添加位置信息
    addTreeItem(positionInfo, "纬度", QString::number(m_currentData.latitude, 'f', 6) + "°");
    addTreeItem(positionInfo, "经度", QString::number(m_currentData.longitude, 'f', 6) + "°");
    addTreeItem(positionInfo, "海拔", QString::number(m_currentData.altitude, 'f', 2) + " m");
    addTreeItem(positionInfo, "速度", QString::number(m_currentData.speed, 'f', 2) + " m/s");
    addTreeItem(positionInfo, "航向", QString::number(m_currentData.course, 'f', 1) + "°");
    
    // 添加卫星信息
    addTreeItem(satelliteInfo, "可见卫星数", QString::number(m_currentData.satelliteCount));
    addTreeItem(satelliteInfo, "使用卫星数", QString::number(m_currentData.usedSatelliteCount));
    
    // 添加质量信息
    addTreeItem(qualityInfo, "PDOP", QString::number(m_currentData.pdop, 'f', 2));
    addTreeItem(qualityInfo, "HDOP", QString::number(m_currentData.hdop, 'f', 2));
    addTreeItem(qualityInfo, "VDOP", QString::number(m_currentData.vdop, 'f', 2));
    
    // 展开所有节点
    m_treeWidget->expandAll();
}

void MessageView::addTreeItem(QTreeWidgetItem *parent, const QString &name, const QString &value)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, name);
    item->setText(1, value);
    item->setTextAlignment(1, Qt::AlignRight);
}

void MessageView::onTreeItemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    
    if (!item) return;
    
    QString itemText = item->text(0);
    QTreeWidgetItem *parent = item->parent();
    
    if (parent) {
        QString parentText = parent->text(0);
        updateTableData(parentText, itemText);
    } else {
        updateTableData(itemText);
    }
}

void MessageView::updateTableData(const QString &messageType, const QString &system)
{
    m_tableWidget->clearContents();
    m_tableWidget->setRowCount(0);
    
    QList<NMEAField> fields;
    
    if (messageType == "全部信息") {
        // 基本信息
        fields.append(NMEAField("=== 基本信息 ===", ""));
        fields.append(NMEAField("时间", m_currentData.time));
        fields.append(NMEAField("日期", m_currentData.date));
        fields.append(NMEAField("定位类型", m_currentData.fixType));
        fields.append(NMEAField("", "")); // 空行分隔
        
        // 位置信息
        fields.append(NMEAField("=== 位置信息 ===", ""));
        fields.append(NMEAField("纬度", QString::number(m_currentData.latitude, 'f', 6) + "°"));
        fields.append(NMEAField("经度", QString::number(m_currentData.longitude, 'f', 6) + "°"));
        fields.append(NMEAField("海拔", QString::number(m_currentData.altitude, 'f', 2) + " m"));
        fields.append(NMEAField("速度", QString::number(m_currentData.speed, 'f', 2) + " m/s"));
        fields.append(NMEAField("航向", QString::number(m_currentData.course, 'f', 1) + "°"));
        fields.append(NMEAField("", "")); // 空行分隔
        
        // 卫星信息
        fields.append(NMEAField("=== 卫星信息 ===", ""));
        fields.append(NMEAField("可见卫星数", QString::number(m_currentData.satelliteCount)));
        fields.append(NMEAField("使用卫星数", QString::number(m_currentData.usedSatelliteCount)));
        
        // 按系统分组显示卫星 - 按优先级排序
        QMap<QString, int> systemCount;
        QMap<QString, int> systemUsedCount;
        for (const SatelliteInfo &sat : m_currentData.satellites) {
            systemCount[sat.system]++;
            if (sat.used) {
                systemUsedCount[sat.system]++;
            }
        }
        
        // 按系统优先级排序显示
        QStringList systemOrder = {"GPS", "BDS", "GLN", "GAL", "QZSS", "SBAS"};
        for (const QString &systemName : systemOrder) {
            if (systemCount.contains(systemName)) {
                int totalCount = systemCount[systemName];
                int usedCount = systemUsedCount.value(systemName, 0);
                fields.append(NMEAField(systemName + "卫星总数", QString::number(totalCount)));
                fields.append(NMEAField(systemName + "使用卫星", QString::number(usedCount)));
            }
        }
        
        // 显示其他系统（如果有）
        for (auto it = systemCount.begin(); it != systemCount.end(); ++it) {
            QString systemName = it.key();
            if (!systemOrder.contains(systemName)) {
                int totalCount = it.value();
                int usedCount = systemUsedCount.value(systemName, 0);
                fields.append(NMEAField(systemName + "卫星总数", QString::number(totalCount)));
                fields.append(NMEAField(systemName + "使用卫星", QString::number(usedCount)));
            }
        }
        fields.append(NMEAField("", "")); // 空行分隔
        
        // 质量信息
        fields.append(NMEAField("=== 质量信息 ===", ""));
        fields.append(NMEAField("PDOP", QString::number(m_currentData.pdop, 'f', 2)));
        fields.append(NMEAField("HDOP", QString::number(m_currentData.hdop, 'f', 2)));
        fields.append(NMEAField("VDOP", QString::number(m_currentData.vdop, 'f', 2)));
        fields.append(NMEAField("", "")); // 空行分隔
        
        // 详细卫星信息
        if (!m_currentData.satellites.isEmpty()) {
            fields.append(NMEAField("=== 详细卫星信息 ===", ""));
            
            // 按系统分组显示详细卫星信息
            QMap<QString, QList<SatelliteInfo>> systemSatellites;
            for (const SatelliteInfo &sat : m_currentData.satellites) {
                systemSatellites[sat.system].append(sat);
            }
            
            for (auto it = systemSatellites.begin(); it != systemSatellites.end(); ++it) {
                QString systemName = it.key();
                const QList<SatelliteInfo> &sats = it.value();
                
                fields.append(NMEAField("--- " + systemName + "系统 ---", ""));
                
                for (const SatelliteInfo &sat : sats) {
                    QString satInfo = QString("ID:%1 仰角:%2° 方位角:%3° 信噪比:%4dB %5")
                                    .arg(sat.id)
                                    .arg(sat.elevation)
                                    .arg(sat.azimuth)
                                    .arg(sat.snr)
                                    .arg(sat.used ? "(使用中)" : "(未使用)");
                    fields.append(NMEAField(QString("卫星%1").arg(sat.id), satInfo));
                }
            }
        }
    }
    else if (messageType == "📍 基本信息") {
        fields.append(NMEAField("时间", m_currentData.time));
        fields.append(NMEAField("日期", m_currentData.date));
        fields.append(NMEAField("定位类型", m_currentData.fixType));
    }
    else if (messageType == "🗺️ 位置信息") {
        fields.append(NMEAField("纬度", QString::number(m_currentData.latitude, 'f', 6) + "°"));
        fields.append(NMEAField("经度", QString::number(m_currentData.longitude, 'f', 6) + "°"));
        fields.append(NMEAField("海拔", QString::number(m_currentData.altitude, 'f', 2) + " m"));
        fields.append(NMEAField("速度", QString::number(m_currentData.speed, 'f', 2) + " m/s"));
        fields.append(NMEAField("航向", QString::number(m_currentData.course, 'f', 1) + "°"));
    }
    else if (messageType == "🛰️ 卫星信息") {
        fields.append(NMEAField("可见卫星数", QString::number(m_currentData.satelliteCount)));
        fields.append(NMEAField("使用卫星数", QString::number(m_currentData.usedSatelliteCount)));
        
        // 按系统分组显示卫星
        QMap<QString, int> systemCount;
        for (const SatelliteInfo &sat : m_currentData.satellites) {
            systemCount[sat.system]++;
        }
        
        for (auto it = systemCount.begin(); it != systemCount.end(); ++it) {
            fields.append(NMEAField(it.key() + "卫星数", QString::number(it.value())));
        }
    }
    else if (messageType == "📊 质量信息") {
        fields.append(NMEAField("PDOP", QString::number(m_currentData.pdop, 'f', 2)));
        fields.append(NMEAField("HDOP", QString::number(m_currentData.hdop, 'f', 2)));
        fields.append(NMEAField("VDOP", QString::number(m_currentData.vdop, 'f', 2)));
    }
    else if (messageType == "GGA") {
        fields.append(NMEAField("时间", m_currentData.time));
        fields.append(NMEAField("纬度", QString::number(m_currentData.latitude, 'f', 6) + "°"));
        fields.append(NMEAField("经度", QString::number(m_currentData.longitude, 'f', 6) + "°"));
        fields.append(NMEAField("定位质量", m_currentData.fixType));
        fields.append(NMEAField("卫星数", QString::number(m_currentData.satelliteCount)));
        fields.append(NMEAField("HDOP", QString::number(m_currentData.hdop, 'f', 2)));
        fields.append(NMEAField("海拔", QString::number(m_currentData.altitude, 'f', 2) + " m"));
    }
    else if (messageType == "GSA") {
        fields.append(NMEAField("模式", "自动"));
        fields.append(NMEAField("定位类型", m_currentData.fixType));
        fields.append(NMEAField("使用卫星数", QString::number(m_currentData.usedSatelliteCount)));
        fields.append(NMEAField("PDOP", QString::number(m_currentData.pdop, 'f', 2)));
        fields.append(NMEAField("HDOP", QString::number(m_currentData.hdop, 'f', 2)));
        fields.append(NMEAField("VDOP", QString::number(m_currentData.vdop, 'f', 2)));
        
        // 添加使用的卫星ID
        for (int i = 0; i < m_currentData.satellites.size() && i < 12; ++i) {
            const SatelliteInfo &sat = m_currentData.satellites[i];
            if (sat.used) {
                fields.append(NMEAField(QString("SVID%1").arg(i+1), QString::number(sat.id)));
            }
        }
    }
    else if (messageType == "GSV") {
        fields.append(NMEAField("可见卫星数", QString::number(m_currentData.satelliteCount)));
        
        // 按系统分组显示卫星信息
        QMap<QString, QList<SatelliteInfo>> systemSatellites;
        for (const SatelliteInfo &sat : m_currentData.satellites) {
            systemSatellites[sat.system].append(sat);
        }
        
        for (auto it = systemSatellites.begin(); it != systemSatellites.end(); ++it) {
            QString systemName = it.key();
            if (system.isEmpty() || system == systemName) {
                fields.append(NMEAField(systemName + "卫星数", QString::number(it.value().size())));
                
                for (const SatelliteInfo &sat : it.value()) {
                    QString satInfo = QString("ID:%1 仰角:%2° 方位角:%3° 信噪比:%4dB")
                                    .arg(sat.id).arg(sat.elevation).arg(sat.azimuth).arg(sat.snr);
                    fields.append(NMEAField(QString("卫星%1").arg(sat.id), satInfo));
                }
            }
        }
    }
    else if (messageType == "RMC") {
        fields.append(NMEAField("时间", m_currentData.time));
        fields.append(NMEAField("日期", m_currentData.date));
        fields.append(NMEAField("状态", m_currentData.fixType));
        fields.append(NMEAField("纬度", QString::number(m_currentData.latitude, 'f', 6) + "°"));
        fields.append(NMEAField("经度", QString::number(m_currentData.longitude, 'f', 6) + "°"));
        fields.append(NMEAField("速度", QString::number(m_currentData.speed, 'f', 2) + " m/s"));
        fields.append(NMEAField("航向", QString::number(m_currentData.course, 'f', 1) + "°"));
    }
    else if (messageType == "VTG") {
        fields.append(NMEAField("航向", QString::number(m_currentData.course, 'f', 1) + "°"));
        fields.append(NMEAField("速度", QString::number(m_currentData.speed, 'f', 2) + " m/s"));
    }
    else if (messageType == "ZDA") {
        fields.append(NMEAField("时间", m_currentData.time));
        fields.append(NMEAField("日期", m_currentData.date));
    }
    
    // 更新表格
    m_tableWidget->setRowCount(fields.size());
    for (int i = 0; i < fields.size(); ++i) {
        const NMEAField &field = fields[i];
        
        QTableWidgetItem *idItem = new QTableWidgetItem(field.name);
        QTableWidgetItem *valueItem = new QTableWidgetItem(field.value);
        
        // 设置不同类型行的样式
        if (field.name.startsWith("===")) {
            // 分类标题行 - 浅蓝色背景，深色文字
            idItem->setBackground(QColor(173, 216, 230));
            idItem->setForeground(QColor(25, 25, 112));
            idItem->setFont(QFont("Arial", 10, QFont::Bold));
            valueItem->setBackground(QColor(173, 216, 230));
            valueItem->setForeground(QColor(25, 25, 112));
            valueItem->setFont(QFont("Arial", 10, QFont::Bold));
        } else if (field.name.startsWith("---")) {
            // 子系统标题行 - 浅蓝色背景，深色文字
            idItem->setBackground(QColor(173, 216, 230));
            idItem->setForeground(QColor(25, 25, 112));
            idItem->setFont(QFont("Arial", 9, QFont::Bold));
            valueItem->setBackground(QColor(173, 216, 230));
            valueItem->setForeground(QColor(25, 25, 112));
            valueItem->setFont(QFont("Arial", 9, QFont::Bold));
        } else if (field.name.isEmpty()) {
            // 空行分隔 - 浅灰色背景，深色文字
            idItem->setBackground(QColor(236, 240, 241));
            idItem->setForeground(QColor(44, 62, 80));
            valueItem->setBackground(QColor(236, 240, 241));
            valueItem->setForeground(QColor(44, 62, 80));
        } else {
            // 普通数据行 - 白色背景，深色文字
            idItem->setBackground(QColor(255, 255, 255));
            idItem->setForeground(QColor(44, 62, 80));
            idItem->setFont(QFont("Arial", 9));
            valueItem->setBackground(QColor(255, 255, 255));
            valueItem->setForeground(QColor(44, 62, 80));
            valueItem->setFont(QFont("Arial", 9));
            
            // 为重要数据添加特殊颜色
            if (field.name.contains("卫星") || field.name.contains("PDOP") || 
                field.name.contains("HDOP") || field.name.contains("VDOP")) {
                valueItem->setForeground(QColor(39, 174, 96));
                valueItem->setFont(QFont("Arial", 9, QFont::Bold));
            }
        }
        
        m_tableWidget->setItem(i, 0, idItem);
        m_tableWidget->setItem(i, 1, valueItem);
    }
    
    m_tableWidget->resizeColumnsToContents();
    
    // 设置表格样式
    m_tableWidget->setStyleSheet(
        "QTableWidget { "
        "    gridline-color: #bdc3c7; "
        "    background-color: #f8f9fa; "
        "    alternate-background-color: #ecf0f1; "
        "    selection-background-color: #3498db; "
        "    selection-color: #ecf0f1; "
        "} "
        "QTableWidget::item { "
        "    padding: 5px; "
        "    border: none; "
        "} "
        "QTableWidget::item:selected { "
        "    background-color: #3498db; "
        "    color: #ecf0f1; "
        "} "
        "QHeaderView::section { "
        "    background-color: #2c3e50; "
        "    color: #ecf0f1; "
        "    padding: 8px; "
        "    font-weight: bold; "
        "    border: 1px solid #34495e; "
        "}"
    );
}
