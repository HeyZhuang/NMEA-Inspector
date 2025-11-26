#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QStringList>

class SerialManager : public QObject
{
    Q_OBJECT

public:
    explicit SerialManager(QObject *parent = nullptr);
    ~SerialManager();

    // 串口操作
    bool openPort();
    void closePort();
    bool isPortOpen() const;
    
    // 串口配置
    void setPortName(const QString &portName);
    void setBaudRate(int baudRate);
    void setDataBits(int dataBits);
    void setStopBits(int stopBits);
    void setParity(int parity);
    
    // 获取可用串口列表
    QStringList getAvailablePorts();
    
    // 获取当前配置
    QString getCurrentPortName() const;
    int getCurrentBaudRate() const;
    
    // 数据发送
    void sendData(const QByteArray &data);

signals:
    // 数据接收信号
    void dataReceived(const QString &data);
    // 串口状态变化信号
    void portStatusChanged(bool isOpen);
    // 错误信号
    void errorOccurred(const QString &error);

private slots:
    void onDataReceived();
    void onErrorOccurred(QSerialPort::SerialPortError error);

private:
    QSerialPort *m_serialPort;
    QString m_portName;
    int m_baudRate;
    int m_dataBits;
    int m_stopBits;
    int m_parity;
    
    void setupSerialPort();
};

#endif // SERIALMANAGER_H
