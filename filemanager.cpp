#include "filemanager.h"
#include "nmeaparser.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMap>

FileManager::FileManager(QObject *parent)
    : QObject(parent)
    , m_currentLine(0)
    , m_parser(new NMEAParser(this))
{
    connect(m_parser, &NMEAParser::dataParsed, this, &FileManager::dataParsed);
}

bool FileManager::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件:" << fileName;
        return false;
    }
    
    m_nmeaLines.clear();
    m_currentLine = 0;
    m_fileName = fileName;
    
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    
    QString line;
    while (stream.readLineInto(&line)) {
        // 只保留符合NMEA格式的行
        if (line.startsWith('$') && line.contains('*')) {
            m_nmeaLines.append(line.trimmed());
        }
    }
    
    file.close();
    
    qDebug() << "文件加载完成:" << fileName << "总行数:" << m_nmeaLines.size();
    emit fileLoaded(fileName);
    
    return !m_nmeaLines.isEmpty();
}

void FileManager::processNextLine()
{
    if (m_currentLine >= m_nmeaLines.size()) {
        emit replayFinished();
        return;
    }
    
    QString line = m_nmeaLines[m_currentLine];
    
    // 发送原始NMEA数据到NMEA视图
    emit nmeaDataReceived(line);
    
    // 解析NMEA数据
    m_parser->parseNMEASentence(line);
    
    m_currentLine++;
    
    qDebug() << "处理第" << m_currentLine << "行:" << line;
}
