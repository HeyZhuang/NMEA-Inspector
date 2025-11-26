#ifndef NMEAVIEW_H
#define NMEAVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QScrollBar>
#include <QMap>
#include "satellitedata.h"

class NMEAView : public QWidget
{
    Q_OBJECT

public:
    explicit NMEAView(QWidget *parent = nullptr);
    
    void updateData(const SatelliteData &data);
    void addNMEALine(const QString &line);

private slots:
    void onClearData();
    void onSaveData();

private:
    void setupUI();
    
    // UI组件
    QGroupBox *m_mainGroup;
    QVBoxLayout *m_mainLayout;
    QTextEdit *m_textEdit;
    QHBoxLayout *m_buttonLayout;
    QPushButton *m_clearButton;
    QPushButton *m_saveButton;
    
    // 数据存储
    QStringList m_nmeaLines;
};

#endif // NMEAVIEW_H
