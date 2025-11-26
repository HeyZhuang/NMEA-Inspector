#include "serialmanager.h"
#include <QDebug>

SerialManager::SerialManager(QObject *parent)
    : QObject(parent)
    , m_serialPort(new QSerialPort(this))
    , m_portName("COM1")
    , m_baudRate(9600)
    , m_dataBits(8)
    , m_stopBits(1)
    , m_parity(0)
{
    setupSerialPort();
}

SerialManager::~SerialManager()
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
    }
}

void SerialManager::setupSerialPort()
{
    // 连接信号槽
    connect(m_serialPort, &QSerialPort::readyRead, this, &SerialManager::onDataReceived);
    connect(m_serialPort, QOverload<QSerialPort::SerialPortError>::of(&QSerialPort::error),
            this, &SerialManager::onErrorOccurred);
}

bool SerialManager::openPort()
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
    }
    
    // 设置串口参数
    m_serialPort->setPortName(m_portName);
    m_serialPort->setBaudRate(m_baudRate);
    m_serialPort->setDataBits(static_cast<QSerialPort::DataBits>(m_dataBits));
    m_serialPort->setStopBits(static_cast<QSerialPort::StopBits>(m_stopBits));
    m_serialPort->setParity(static_cast<QSerialPort::Parity>(m_parity));
    
    // 打开串口
    if (m_serialPort->open(QIODevice::ReadWrite)) {
        qDebug() << "串口打开成功:" << m_portName << "波特率:" << m_baudRate;
        emit portStatusChanged(true);
        return true;
    } else {
        QString errorMsg = QString("串口打开失败: %1").arg(m_serialPort->errorString());
        qDebug() << errorMsg;
        emit errorOccurred(errorMsg);
        emit portStatusChanged(false);
        return false;
    }
}

void SerialManager::closePort()
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
        qDebug() << "串口已关闭:" << m_portName;
        emit portStatusChanged(false);
    }
}

bool SerialManager::isPortOpen() const
{
    return m_serialPort->isOpen();
}

void SerialManager::setPortName(const QString &portName)
{
    m_portName = portName;
}

void SerialManager::setBaudRate(int baudRate)
{
    m_baudRate = baudRate;
}

void SerialManager::setDataBits(int dataBits)
{
    m_dataBits = dataBits;
}

void SerialManager::setStopBits(int stopBits)
{
    m_stopBits = stopBits;
}

void SerialManager::setParity(int parity)
{
    m_parity = parity;
}

QStringList SerialManager::getAvailablePorts()
{
    QStringList portList;
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    
    for (const QSerialPortInfo &portInfo : serialPortInfos) {
        QString portName = portInfo.portName();
        QString description = portInfo.description();
        QString manufacturer = portInfo.manufacturer();
        
        QString displayText = QString("%1 - %2").arg(portName);
        if (!description.isEmpty()) {
            displayText += QString(" (%3)").arg(description);
        }
        
        portList << displayText;
    }
    
    return portList;
}

QString SerialManager::getCurrentPortName() const
{
    return m_portName;
}

int SerialManager::getCurrentBaudRate() const
{
    return m_baudRate;
}

void SerialManager::sendData(const QByteArray &data)
{
    if (m_serialPort->isOpen()) {
        qint64 bytesWritten = m_serialPort->write(data);
        if (bytesWritten == -1) {
            emit errorOccurred("数据发送失败");
        } else {
            qDebug() << "发送数据:" << data.toHex();
        }
    } else {
        emit errorOccurred("串口未打开，无法发送数据");
    }
}

void SerialManager::onDataReceived()
{
    QByteArray data = m_serialPort->readAll();
    QString dataString = QString::fromUtf8(data);
    
    // 发送接收到的数据
    emit dataReceived(dataString);
    
    qDebug() << "接收到数据:" << dataString.trimmed();
}

void SerialManager::onErrorOccurred(QSerialPort::SerialPortError error)
{
    if (error != QSerialPort::NoError) {
        QString errorString;
        switch (error) {
        case QSerialPort::DeviceNotFoundError:
            errorString = "设备未找到";
            break;
        case QSerialPort::PermissionError:
            errorString = "权限不足";
            break;
        case QSerialPort::OpenError:
            errorString = "打开设备失败";
            break;
        case QSerialPort::WriteError:
            errorString = "写入数据失败";
            break;
        case QSerialPort::ReadError:
            errorString = "读取数据失败";
            break;
        case QSerialPort::ResourceError:
            errorString = "资源错误";
            break;
        case QSerialPort::UnsupportedOperationError:
            errorString = "不支持的操作";
            break;
        case QSerialPort::TimeoutError:
            errorString = "操作超时";
            break;
        default:
            errorString = "未知错误";
            break;
        }
        
        emit errorOccurred(errorString);
        emit portStatusChanged(false);
    }
}
