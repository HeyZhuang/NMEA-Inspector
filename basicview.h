#ifndef BASICVIEW_H
#define BASICVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QMap>
#include "satellitedata.h"

class BasicView : public QWidget
{
    Q_OBJECT

public:
    explicit BasicView(QWidget *parent = nullptr);
    
    void updateData(const SatelliteData &data);

private:
    void setupUI();
    
    // UI组件
    QGroupBox *m_mainGroup;
    QVBoxLayout *m_mainLayout;
    
    // 数据字段
    QLineEdit *m_latitudeEdit;
    QLineEdit *m_longitudeEdit;
    QLineEdit *m_altitudeEdit;
    QLineEdit *m_timeEdit;
    QLineEdit *m_dateEdit;
    QLineEdit *m_beijingTimeEdit;
};

#endif // BASICVIEW_H
