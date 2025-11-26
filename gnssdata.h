#ifndef GNSSDATA_H
#define GNSSDATA_H

#include <QString>
#include <QDateTime>
#include <QList>

// GNSS卫星信息结构体
struct SatelliteInfo {
    int id;                    // 卫星ID
    int elevation;             // 高度角 (度)
    int azimuth;               // 方位角 (度)
    int snr;                   // 信噪比 (dB)
    QString system;            // 卫星系统 (GPS, BDS, GLONASS等)
    bool used;                 // 是否用于定位
    
    SatelliteInfo() : id(0), elevation(0), azimuth(0), snr(0), used(false) {}
};

// GNSS定位数据主结构体
struct GNSSData {
    QDateTime timestamp;       // 时间戳
    double latitude;           // 纬度 (度)
    double longitude;          // 经度 (度)
    double altitude;           // 海拔高度 (米)
    int satelliteCount;        // 可见卫星数
    int usedSatelliteCount;    // 用于定位的卫星数
    double hdop;              // 水平精度因子
    double pdop;              // 位置精度因子
    double vdop;              // 垂直精度因子
    QString fixType;          // 定位类型 (无定位, 单点定位, 差分定位等)
    double speed;             // 速度 (m/s)
    double course;            // 航向 (度)
    QList<SatelliteInfo> satellites; // 卫星信息列表
    
    GNSSData() : latitude(0.0), longitude(0.0), altitude(0.0),
                 satelliteCount(0), usedSatelliteCount(0),
                 hdop(0.0), pdop(0.0), vdop(0.0),
                 speed(0.0), course(0.0) {}
};

#endif // GNSSDATA_H
