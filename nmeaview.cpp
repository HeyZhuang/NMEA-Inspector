#include "nmeaview.h"
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDateTime>
#include <QStandardPaths>
#include <QMap>
#include <QTextCursor>

NMEAView::NMEAView(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("📄 NMEA数据 - 原始数据流");
    setMinimumSize(600, 450);
    resize(800, 600);
    // 使用Emoji作为窗口图标
    
    // 设置窗口属性
    setAttribute(Qt::WA_DeleteOnClose, false);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    setupUI();
}

void NMEAView::setupUI()
{
    m_mainGroup = new QGroupBox("📄 NMEA原始数据流", this);
    m_mainGroup->setStyleSheet("QGroupBox { font-weight: bold; font-size: 11pt; }");
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(10, 10, 10, 10);
    m_mainLayout->addWidget(m_mainGroup);
    
    QVBoxLayout *groupLayout = new QVBoxLayout(m_mainGroup);
    groupLayout->setSpacing(8);
    
    // 文本显示区域
    m_textEdit = new QTextEdit();
    m_textEdit->setReadOnly(true);
    m_textEdit->setFont(QFont("Consolas", 9));
    m_textEdit->setPlaceholderText("NMEA数据将在此显示...\n格式: [时间戳] $GPGGA,时间,纬度,经度,定位质量,卫星数,HDOP,海拔...");
    m_textEdit->setStyleSheet("QTextEdit { background-color: #2c3e50; color: #ecf0f1; border: 2px solid #34495e; border-radius: 6px; padding: 8px; }");
    groupLayout->addWidget(m_textEdit);
    
    // 按钮布局
    m_buttonLayout = new QHBoxLayout();
    m_buttonLayout->setSpacing(8);
    
    m_clearButton = new QPushButton("🗑️ 清空数据");
    m_clearButton->setToolTip("清空所有NMEA数据");
    m_buttonLayout->addWidget(m_clearButton);
    
    m_saveButton = new QPushButton("💾 保存数据");
    m_saveButton->setToolTip("保存NMEA数据到文件");
    m_buttonLayout->addWidget(m_saveButton);
    
    m_buttonLayout->addStretch();
    groupLayout->addLayout(m_buttonLayout);
    
    // 连接信号
    connect(m_clearButton, &QPushButton::clicked, this, &NMEAView::onClearData);
    connect(m_saveButton, &QPushButton::clicked, this, &NMEAView::onSaveData);
}

void NMEAView::updateData(const SatelliteData &data)
{
    // 显示解析后的数据摘要
    QString summary = QString("📍 位置: %1, %2 | 🛰️ 卫星: %3 | ⏰ 时间: %4")
                      .arg(data.latitude, 0, 'f', 6)
                      .arg(data.longitude, 0, 'f', 6)
                      .arg(data.satelliteCount)
                      .arg(data.time);
    
    QString displayLine = QString("[%1] %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss"), summary);
    m_textEdit->append(displayLine);
    
    // 限制显示行数，避免内存占用过大
    if (m_textEdit->document()->blockCount() > 1000) {
        QTextCursor cursor = m_textEdit->textCursor();
        cursor.movePosition(QTextCursor::Start);
        cursor.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor, 100);
        cursor.removeSelectedText();
    }
}

void NMEAView::addNMEALine(const QString &line)
{
    if (line.startsWith('$') && line.contains('*')) {
        m_nmeaLines.append(line);
        
        // 添加时间戳
        QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
        QString displayLine = QString("[%1] %2").arg(timestamp, line);
        
        m_textEdit->append(displayLine);
        
        // 限制显示行数，避免内存占用过多
        if (m_textEdit->document()->blockCount() > 1000) {
            QTextCursor cursor = m_textEdit->textCursor();
            cursor.movePosition(QTextCursor::Start);
            cursor.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor, 500);
            cursor.removeSelectedText();
        }
        
        // 自动滚动到底部
        QScrollBar *scrollBar = m_textEdit->verticalScrollBar();
        scrollBar->setValue(scrollBar->maximum());
    }
}

void NMEAView::onClearData()
{
    m_textEdit->clear();
    m_nmeaLines.clear();
}

void NMEAView::onSaveData()
{
    if (m_nmeaLines.isEmpty()) {
        QMessageBox::information(this, "提示", "没有数据可保存");
        return;
    }
    
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "保存NMEA数据",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/nmea_data.txt",
        "文本文件 (*.txt);;所有文件 (*.*)"
    );
    
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            
            for (const QString &line : m_nmeaLines) {
                stream << line << "\n";
            }
            
            file.close();
            QMessageBox::information(this, "成功", "数据已保存到: " + fileName);
        } else {
            QMessageBox::critical(this, "错误", "无法创建文件: " + fileName);
        }
    }
}
