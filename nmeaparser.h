#ifndef NMEAPARSER_H
#define NMEAPARSER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QMap>
#include "satellitedata.h"

class NMEAParser : public QObject
{
    Q_OBJECT

public:
    explicit NMEAParser(QObject *parent = nullptr);
    
    // 解析NMEA语句
    bool parseNMEASentence(const QString &sentence);
    
    // 获取当前数据
    SatelliteData getCurrentData() const { return m_currentData; }

signals:
    void dataParsed(const SatelliteData &data);

private:
    // 解析不同类型的NMEA语句 - 支持多卫星系统
    bool parseGGA(const QStringList &fields);
    bool parseRMC(const QStringList &fields);
    bool parseGSV(const QStringList &fields);
    bool parseGSA(const QStringList &fields);
    bool parseGLL(const QStringList &fields);
    bool parseVTG(const QStringList &fields);
    bool parseZDA(const QStringList &fields);
    
    // 原有的GPS解析函数
    bool parseGPGGA(const QStringList &fields);
    bool parseGPRMC(const QStringList &fields);
    bool parseGPGSV(const QStringList &fields);
    bool parseGPGSA(const QStringList &fields);
    bool parseGPGLL(const QStringList &fields);
    bool parseGPVTG(const QStringList &fields);
    bool parseGPZDA(const QStringList &fields);
    
    // 工具函数
    double parseCoordinate(const QString &coord, const QString &hemisphere);
    QString getFixTypeString(int fixType);
    QString getSatelliteSystem(const QString &sentenceType);
    QString getSatelliteSystemByID(int satelliteID);
    QDateTime parseDateTime(const QString &timeStr, const QString &dateStr = "");
    
    // 数据存储
    SatelliteData m_currentData;
    
    // GSV消息处理
    int m_gsvMessageCount;
    int m_gsvCurrentMessage;
    QList<SatelliteInfo> m_tempSatellites;
};

#endif // NMEAPARSER_H