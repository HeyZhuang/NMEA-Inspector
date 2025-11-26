#include "basicview.h"
#include <QFormLayout>
#include <QMap>

BasicView::BasicView(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("📍 基础视图 - 定位信息");
    setMinimumSize(350, 280);
    resize(450, 320);
    // 使用Emoji作为窗口图标
    
    // 设置窗口属性
    setAttribute(Qt::WA_DeleteOnClose, false);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    setupUI();
}

void BasicView::setupUI()
{
    m_mainGroup = new QGroupBox("📍 基础定位信息", this);
    m_mainGroup->setStyleSheet("QGroupBox { font-weight: bold; font-size: 11pt; }");
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(10, 10, 10, 10);
    m_mainLayout->addWidget(m_mainGroup);
    
    QFormLayout *formLayout = new QFormLayout(m_mainGroup);
    formLayout->setSpacing(12);
    formLayout->setLabelAlignment(Qt::AlignRight);
    
    // 纬度
    m_latitudeEdit = new QLineEdit();
    m_latitudeEdit->setReadOnly(true);
    m_latitudeEdit->setPlaceholderText("0.000000°");
    m_latitudeEdit->setStyleSheet("QLineEdit { font-family: 'Consolas'; font-size: 10pt; }");
    formLayout->addRow("🌐 纬度:", m_latitudeEdit);
    
    // 经度
    m_longitudeEdit = new QLineEdit();
    m_longitudeEdit->setReadOnly(true);
    m_longitudeEdit->setPlaceholderText("0.000000°");
    m_longitudeEdit->setStyleSheet("QLineEdit { font-family: 'Consolas'; font-size: 10pt; }");
    formLayout->addRow("🌐 经度:", m_longitudeEdit);
    
    // 海拔
    m_altitudeEdit = new QLineEdit();
    m_altitudeEdit->setReadOnly(true);
    m_altitudeEdit->setPlaceholderText("0.00 m");
    m_altitudeEdit->setStyleSheet("QLineEdit { font-family: 'Consolas'; font-size: 10pt; }");
    formLayout->addRow("⛰️ 海拔:", m_altitudeEdit);
    
    // UTC时间
    m_timeEdit = new QLineEdit();
    m_timeEdit->setReadOnly(true);
    m_timeEdit->setPlaceholderText("00:00:00");
    m_timeEdit->setStyleSheet("QLineEdit { font-family: 'Consolas'; font-size: 10pt; }");
    formLayout->addRow("🕐 UTC时间:", m_timeEdit);
    
    // UTC日期
    m_dateEdit = new QLineEdit();
    m_dateEdit->setReadOnly(true);
    m_dateEdit->setPlaceholderText("2024:01:01");
    m_dateEdit->setStyleSheet("QLineEdit { font-family: 'Consolas'; font-size: 10pt; }");
    formLayout->addRow("📅 UTC日期:", m_dateEdit);
    
    // 北京时间
    m_beijingTimeEdit = new QLineEdit();
    m_beijingTimeEdit->setReadOnly(true);
    m_beijingTimeEdit->setPlaceholderText("00:00:00");
    m_beijingTimeEdit->setStyleSheet("QLineEdit { font-family: 'Consolas'; font-size: 10pt; }");
    formLayout->addRow("🇨🇳 北京时间:", m_beijingTimeEdit);
}

void BasicView::updateData(const SatelliteData &data)
{
    // 更新纬度 (保留6位小数)
    if (data.latitude != 0.0) {
        m_latitudeEdit->setText(QString::number(data.latitude, 'f', 6) + "°");
    }
    
    // 更新经度 (保留6位小数)
    if (data.longitude != 0.0) {
        m_longitudeEdit->setText(QString::number(data.longitude, 'f', 6) + "°");
    }
    
    // 更新海拔 (保留2位小数)
    if (data.altitude != 0.0) {
        m_altitudeEdit->setText(QString::number(data.altitude, 'f', 2) + " m");
    }
    
    // 更新时间
    if (!data.time.isEmpty()) {
        m_timeEdit->setText(data.time);
    }
    
    // 更新日期
    if (!data.date.isEmpty()) {
        m_dateEdit->setText(data.date);
    }
    
    // 更新北京时间
    if (!data.beijingTime.isEmpty()) {
        m_beijingTimeEdit->setText(data.beijingTime);
    }
}
