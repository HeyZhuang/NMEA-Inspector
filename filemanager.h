#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QStringList>
#include <QTimer>
#include <QMap>
#include "satellitedata.h"

class NMEAParser;

class FileManager : public QObject
{
    Q_OBJECT

public:
    explicit FileManager(QObject *parent = nullptr);
    
    // 加载NMEA文件
    bool loadFile(const QString &fileName);
    
    // 处理下一行数据
    void processNextLine();
    
    // 获取文件信息
    QString getFileName() const { return m_fileName; }
    int getTotalLines() const { return m_nmeaLines.size(); }
    int getCurrentLine() const { return m_currentLine; }

signals:
    void dataParsed(const SatelliteData &data);
    void fileLoaded(const QString &fileName);
    void replayFinished();
    void nmeaDataReceived(const QString &line);

private:
    QString m_fileName;
    QStringList m_nmeaLines;
    int m_currentLine;
    NMEAParser *m_parser;
};

#endif // FILEMANAGER_H
