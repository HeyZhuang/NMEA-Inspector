#ifndef SATELLITEDATA_H
#define SATELLITEDATA_H

#include <QString>
#include <QDateTime>
#include <QList>
#include <QColor>

// 卫星信息结构体
struct SatelliteInfo {
    int id;                    // 卫星ID/PRN
    int elevation;             // 仰角 (度)
    int azimuth;               // 方位角 (度)
    int snr;                   // 信噪比 (dB)
    QString system;            // 卫星系统 (GPS, BDS, GLN, GAL)
    bool used;                 // 是否用于定位
    QColor color;              // 显示颜色
    
    SatelliteInfo() : id(0), elevation(0), azimuth(0), snr(0), used(false) {}
};

// NMEA语句字段信息
struct NMEAField {
    QString name;              // 字段名称
    QString value;            // 字段值
    QString description;       // 字段描述
    
    NMEAField() {}
    NMEAField(const QString &n, const QString &v, const QString &d = "")
        : name(n), value(v), description(d) {}
};

// 卫星定位数据
struct SatelliteData {
    QDateTime timestamp;       // 时间戳
    double latitude;           // 纬度 (度)
    double longitude;          // 经度 (度)
    double altitude;           // 海拔 (米)
    QString time;              // UTC时间 (hh:mm:ss)
    QString date;              // UTC日期 (yyyy:MM:dd)
    QString beijingTime;       // 北京时间
    
    // 定位质量信息
    int satelliteCount;        // 可见卫星数
    int usedSatelliteCount;    // 用于定位的卫星数
    double hdop;              // 水平精度因子
    double pdop;              // 位置精度因子
    double vdop;              // 垂直精度因子
    QString fixType;          // 定位类型
    
    // 运动信息
    double speed;             // 速度 (m/s)
    double course;            // 航向 (度)
    
    // 卫星信息
    QList<SatelliteInfo> satellites;
    
    // NMEA字段信息
    QList<NMEAField> nmeaFields;
    
    SatelliteData() : latitude(0.0), longitude(0.0), altitude(0.0),
                     satelliteCount(0), usedSatelliteCount(0),
                     hdop(0.0), pdop(0.0), vdop(0.0),
                     speed(0.0), course(0.0) {}
};

// 卫星系统颜色定义
class SatelliteSystemColors {
public:
    static QColor getSystemColor(const QString &system) {
        if (system == "GPS") return QColor(0, 100, 200);      // 蓝色
        else if (system == "BDS") return QColor(255, 140, 0);  // 橙色
        else if (system == "GLN") return QColor(0, 150, 0);   // 绿色
        else if (system == "GAL") return QColor(150, 0, 150); // 紫色
        else return QColor(128, 128, 128);                    // 灰色
    }
    
    static QString getSystemName(const QString &system) {
        if (system == "GPS") return "GPS";
        else if (system == "BDS") return "北斗";
        else if (system == "GLN") return "格洛纳斯";
        else if (system == "GAL") return "伽利略";
        else return "其他";
    }
};

#endif // SATELLITEDATA_H
