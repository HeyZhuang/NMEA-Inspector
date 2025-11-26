#include "nmeaparser.h"
#include <QDebug>
#include <QRegularExpression>
#include <QMap>

NMEAParser::NMEAParser(QObject *parent)
    : QObject(parent)
    , m_gsvMessageCount(0)
    , m_gsvCurrentMessage(0)
{
}

bool NMEAParser::parseNMEASentence(const QString &sentence)
{
    // 检查NMEA语句格式
    if (!sentence.startsWith('$') || !sentence.contains('*')) {
        return false;
    }
    
    // 分割字段
    QStringList fields = sentence.split(',');
    if (fields.size() < 3) {
        return false;
    }
    
    // 提取语句类型
    QString sentenceType = fields[0].mid(1); // 去掉$符号
    
    qDebug() << "解析NMEA语句:" << sentenceType;
    
    // 根据语句类型进行解析 - 支持多卫星系统
    bool parseResult = false;
    
    // GGA语句 - 支持GPS, BDS, GLN, GAL
    if (sentenceType.endsWith("GGA")) {
        parseResult = parseGGA(fields);
    }
    // RMC语句 - 支持GPS, BDS, GLN, GAL
    else if (sentenceType.endsWith("RMC")) {
        parseResult = parseRMC(fields);
    }
    // GSV语句 - 支持GPS, BDS, GLN, GAL
    else if (sentenceType.endsWith("GSV")) {
        parseResult = parseGSV(fields);
    }
    // GSA语句 - 支持GPS, BDS, GLN, GAL
    else if (sentenceType.endsWith("GSA")) {
        parseResult = parseGSA(fields);
    }
    // GLL语句 - 支持GPS, BDS, GLN, GAL
    else if (sentenceType.endsWith("GLL")) {
        parseResult = parseGLL(fields);
    }
    // VTG语句 - 支持GPS, BDS, GLN, GAL
    else if (sentenceType.endsWith("VTG")) {
        parseResult = parseVTG(fields);
    }
    // ZDA语句 - 支持GPS, BDS, GLN, GAL
    else if (sentenceType.endsWith("ZDA")) {
        parseResult = parseZDA(fields);
    }
    
    if (parseResult) {
        qDebug() << "NMEAParser::parseNMEASentence - 解析成功:" << sentenceType
                 << "卫星数:" << m_currentData.satelliteCount
                 << "纬度:" << m_currentData.latitude
                 << "经度:" << m_currentData.longitude;
        emit dataParsed(m_currentData);
    }
    
    return parseResult;
}

QString NMEAParser::getSatelliteSystem(const QString &sentenceType)
{
    // 根据NMEA语句前缀识别卫星系统
    if (sentenceType.startsWith("GP")) {
        return "GPS";
    } else if (sentenceType.startsWith("BD")) {
        return "BDS";
    } else if (sentenceType.startsWith("GL")) {
        return "GLN";
    } else if (sentenceType.startsWith("GA")) {
        return "GAL";
    } else if (sentenceType.startsWith("GB")) {
        return "BDS"; // 有些设备使用GB前缀表示BDS
    } else if (sentenceType.startsWith("GN")) {
        return "MULTI"; // GN表示多系统
    }
    return "UNKNOWN";
}

QString NMEAParser::getSatelliteSystemByID(int satelliteID)
{
    // 根据卫星ID范围判断卫星系统
    if (satelliteID >= 1 && satelliteID <= 32) {
        return "GPS";
    } else if (satelliteID >= 33 && satelliteID <= 64) {
        return "GLN"; // GLONASS
    } else if (satelliteID >= 65 && satelliteID <= 96) {
        return "GAL"; // Galileo
    } else if (satelliteID >= 97 && satelliteID <= 158) {
        return "BDS"; // BeiDou (包括扩展范围)
    } else if (satelliteID >= 159 && satelliteID <= 192) {
        return "QZSS"; // QZSS
    } else if (satelliteID >= 193 && satelliteID <= 256) {
        return "SBAS"; // SBAS
    }
    return "UNKNOWN";
}

bool NMEAParser::parseGGA(const QStringList &fields)
{
    // 调用原有的GPGGA解析函数
    return parseGPGGA(fields);
}

bool NMEAParser::parseRMC(const QStringList &fields)
{
    // 调用原有的GPRMC解析函数
    return parseGPRMC(fields);
}

bool NMEAParser::parseGSV(const QStringList &fields)
{
    // 获取当前语句类型以确定卫星系统
    QString sentenceType = fields[0].mid(1); // 去掉$符号
    QString system = getSatelliteSystem(sentenceType);
    
    // 调用原有的GPGSV解析函数，但需要设置系统信息
    bool result = parseGPGSV(fields);
    
    // 如果解析成功，更新卫星系统信息
    if (result) {
        for (auto &satellite : m_tempSatellites) {
            if (satellite.system == "UNKNOWN") {
                satellite.system = system;
                satellite.color = SatelliteSystemColors::getSystemColor(satellite.system);
            }
        }
    }
    
    return result;
}

bool NMEAParser::parseGSA(const QStringList &fields)
{
    // 调用原有的GPGSA解析函数
    return parseGPGSA(fields);
}

bool NMEAParser::parseGLL(const QStringList &fields)
{
    // 调用原有的GPGLL解析函数
    return parseGPGLL(fields);
}

bool NMEAParser::parseVTG(const QStringList &fields)
{
    // 调用原有的GPVTG解析函数
    return parseGPVTG(fields);
}

bool NMEAParser::parseZDA(const QStringList &fields)
{
    // 调用原有的GPZDA解析函数
    return parseGPZDA(fields);
}

bool NMEAParser::parseGPGGA(const QStringList &fields)
{
    // $GPGGA,时间,纬度,纬度半球,经度,经度半球,定位质量,卫星数,HDOP,海拔,海拔单位,差分时间,差分站ID*校验和
    if (fields.size() < 15) {
        return false;
    }
    
    try {
        // 解析时间
        QString timeStr = fields[1];
        if (timeStr.length() >= 6) {
            m_currentData.time = timeStr.mid(0, 2) + ":" + 
                                timeStr.mid(2, 2) + ":" + 
                                timeStr.mid(4, 2);
        }
        
        // 解析纬度
        m_currentData.latitude = parseCoordinate(fields[2], fields[3]);
        
        // 解析经度
        m_currentData.longitude = parseCoordinate(fields[4], fields[5]);
        
        // 解析定位质量
        int fixQuality = fields[6].toInt();
        m_currentData.fixType = getFixTypeString(fixQuality);
        
        // 解析卫星数
        m_currentData.satelliteCount = fields[7].toInt();
        
        // 解析HDOP
        m_currentData.hdop = fields[8].toDouble();
        
        // 解析海拔
        m_currentData.altitude = fields[9].toDouble();
        
        // 设置时间戳
        m_currentData.timestamp = QDateTime::currentDateTime();
        
        // 计算北京时间
        QDateTime utcTime = m_currentData.timestamp;
        m_currentData.beijingTime = utcTime.addSecs(8 * 3600).toString("hh:mm:ss");
        
        return true;
    } catch (...) {
        qDebug() << "GPGGA解析失败";
        return false;
    }
}

bool NMEAParser::parseGPRMC(const QStringList &fields)
{
    // $GPRMC,时间,状态,纬度,纬度半球,经度,经度半球,速度,航向,日期,磁偏角,磁偏角方向*校验和
    if (fields.size() < 12) {
        return false;
    }
    
    try {
        // 解析时间
        QString timeStr = fields[1];
        QString dateStr = fields[9];
        m_currentData.timestamp = parseDateTime(timeStr, dateStr);
        
        if (timeStr.length() >= 6) {
            m_currentData.time = timeStr.mid(0, 2) + ":" + 
                                timeStr.mid(2, 2) + ":" + 
                                timeStr.mid(4, 2);
        }
        
        if (dateStr.length() >= 6) {
            m_currentData.date = "20" + dateStr.mid(4, 2) + ":" + 
                               dateStr.mid(2, 2) + ":" + 
                               dateStr.mid(0, 2);
        }
        
        // 解析状态
        QString status = fields[2];
        if (status == "A") {
            m_currentData.fixType = "有效定位";
        } else {
            m_currentData.fixType = "无效定位";
        }
        
        // 解析纬度
        m_currentData.latitude = parseCoordinate(fields[3], fields[4]);
        
        // 解析经度
        m_currentData.longitude = parseCoordinate(fields[5], fields[6]);
        
        // 解析速度 (节转换为米/秒)
        m_currentData.speed = fields[7].toDouble() * 0.514444;
        
        // 解析航向
        m_currentData.course = fields[8].toDouble();
        
        return true;
    } catch (...) {
        qDebug() << "GPRMC解析失败";
        return false;
    }
}

bool NMEAParser::parseGPGSV(const QStringList &fields)
{
    // $GPGSV,总消息数,当前消息号,可见卫星数,卫星1PRN,卫星1仰角,卫星1方位角,卫星1信噪比,...*校验和
    if (fields.size() < 4) {
        return false;
    }
    
    try {
        m_gsvMessageCount = fields[1].toInt();
        m_gsvCurrentMessage = fields[2].toInt();
        
        // 如果是第一条GSV消息，清空之前的卫星列表
        if (m_gsvCurrentMessage == 1) {
            m_tempSatellites.clear();
        }
        
        // 解析卫星信息 (每4个字段为一组卫星信息)
        int satelliteCount = (fields.size() - 4) / 4;
        for (int i = 0; i < satelliteCount; i++) {
            int baseIndex = 4 + i * 4;
            if (baseIndex + 3 < fields.size()) {
                SatelliteInfo satellite;
                satellite.id = fields[baseIndex].toInt();
                satellite.elevation = fields[baseIndex + 1].toInt();
                satellite.azimuth = fields[baseIndex + 2].toInt();
                satellite.snr = fields[baseIndex + 3].toInt();
                
                // 根据卫星ID范围判断卫星系统
                satellite.system = getSatelliteSystemByID(satellite.id);
                satellite.color = SatelliteSystemColors::getSystemColor(satellite.system);
                
                qDebug() << "解析卫星 ID:" << satellite.id 
                         << "系统:" << satellite.system 
                         << "仰角:" << satellite.elevation 
                         << "方位角:" << satellite.azimuth 
                         << "信噪比:" << satellite.snr;
                
                if (satellite.id > 0) {
                    m_tempSatellites.append(satellite);
                }
            }
        }
        
        // 如果是最后一条GSV消息，更新当前数据
        if (m_gsvCurrentMessage == m_gsvMessageCount) {
            m_currentData.satellites = m_tempSatellites;
            m_currentData.satelliteCount = m_tempSatellites.size();
            
            // 统计各系统卫星数量
            QMap<QString, int> systemCount;
            for (const SatelliteInfo &sat : m_tempSatellites) {
                systemCount[sat.system]++;
            }
            
            qDebug() << "GPGSV解析完成 - 总卫星数:" << m_tempSatellites.size();
            for (auto it = systemCount.begin(); it != systemCount.end(); ++it) {
                qDebug() << "  " << it.key() << "系统:" << it.value() << "颗卫星";
            }
            
            // 立即发送数据更新信号
            emit dataParsed(m_currentData);
        }
        
        return true;
    } catch (...) {
        qDebug() << "GPGSV解析失败";
        return false;
    }
}

bool NMEAParser::parseGPGSA(const QStringList &fields)
{
    // $GPGSA,模式,定位类型,卫星1,卫星2,...,卫星12,PDOP,HDOP,VDOP*校验和
    if (fields.size() < 18) {
        return false;
    }
    
    try {
        // 解析定位类型
        int fixType = fields[2].toInt();
        m_currentData.fixType = getFixTypeString(fixType);
        
        // 解析精度因子
        m_currentData.pdop = fields[15].toDouble();
        m_currentData.hdop = fields[16].toDouble();
        m_currentData.vdop = fields[17].toDouble();
        
        // 统计用于定位的卫星数并标记使用的卫星
        m_currentData.usedSatelliteCount = 0;
        for (int i = 3; i <= 14; i++) {
            if (!fields[i].isEmpty() && fields[i].toInt() > 0) {
                m_currentData.usedSatelliteCount++;
                
                // 标记当前卫星列表中的卫星为使用状态
                int satelliteID = fields[i].toInt();
                for (auto &satellite : m_currentData.satellites) {
                    if (satellite.id == satelliteID) {
                        satellite.used = true;
                        break;
                    }
                }
            }
        }
        
        return true;
    } catch (...) {
        qDebug() << "GPGSA解析失败";
        return false;
    }
}

bool NMEAParser::parseGPGLL(const QStringList &fields)
{
    // $GPGLL,纬度,纬度半球,经度,经度半球,时间,状态*校验和
    if (fields.size() < 7) {
        return false;
    }
    
    try {
        // 解析纬度
        m_currentData.latitude = parseCoordinate(fields[1], fields[2]);
        
        // 解析经度
        m_currentData.longitude = parseCoordinate(fields[3], fields[4]);
        
        // 解析时间
        QString timeStr = fields[5];
        if (timeStr.length() >= 6) {
            m_currentData.time = timeStr.mid(0, 2) + ":" + 
                                timeStr.mid(2, 2) + ":" + 
                                timeStr.mid(4, 2);
        }
        
        return true;
    } catch (...) {
        qDebug() << "GPGLL解析失败";
        return false;
    }
}

bool NMEAParser::parseGPVTG(const QStringList &fields)
{
    // $GPVTG,航向1,T,航向2,M,速度1,N,速度2,K*校验和
    if (fields.size() < 9) {
        return false;
    }
    
    try {
        // 解析航向
        m_currentData.course = fields[1].toDouble();
        
        // 解析速度 (节转换为米/秒)
        m_currentData.speed = fields[7].toDouble() * 0.277778; // km/h to m/s
        
        return true;
    } catch (...) {
        qDebug() << "GPVTG解析失败";
        return false;
    }
}

bool NMEAParser::parseGPZDA(const QStringList &fields)
{
    // $GPZDA,时间,日,月,年,时区1,时区2*校验和
    if (fields.size() < 6) {
        return false;
    }
    
    try {
        // 解析时间
        QString timeStr = fields[1];
        if (timeStr.length() >= 6) {
            m_currentData.time = timeStr.mid(0, 2) + ":" + 
                                timeStr.mid(2, 2) + ":" + 
                                timeStr.mid(4, 2);
        }
        
        // 解析日期
        QString day = fields[2];
        QString month = fields[3];
        QString year = fields[4];
        m_currentData.date = year + ":" + month + ":" + day;
        
        return true;
    } catch (...) {
        qDebug() << "GPZDA解析失败";
        return false;
    }
}

double NMEAParser::parseCoordinate(const QString &coord, const QString &hemisphere)
{
    if (coord.isEmpty()) {
        return 0.0;
    }
    
    // 将DDMM.MMMM格式转换为DD.DDDDDD格式
    double degrees = coord.left(coord.indexOf('.') - 2).toDouble();
    double minutes = coord.mid(coord.indexOf('.') - 2).toDouble();
    
    double result = degrees + minutes / 60.0;
    
    // 根据半球调整符号
    if (hemisphere == "S" || hemisphere == "W") {
        result = -result;
    }
    
    return result;
}

QString NMEAParser::getFixTypeString(int fixType)
{
    switch (fixType) {
    case 0: return "无定位";
    case 1: return "单点定位";
    case 2: return "差分定位";
    case 3: return "PPS定位";
    case 4: return "RTK固定解";
    case 5: return "RTK浮点解";
    default: return "未知";
    }
}


QDateTime NMEAParser::parseDateTime(const QString &timeStr, const QString &dateStr)
{
    if (timeStr.length() >= 6 && dateStr.length() >= 6) {
        int hour = timeStr.mid(0, 2).toInt();
        int minute = timeStr.mid(2, 2).toInt();
        int second = timeStr.mid(4, 2).toInt();
        
        int day = dateStr.mid(0, 2).toInt();
        int month = dateStr.mid(2, 2).toInt();
        int year = 2000 + dateStr.mid(4, 2).toInt();
        
        return QDateTime(QDate(year, month, day), QTime(hour, minute, second));
    }
    return QDateTime::currentDateTime();
}