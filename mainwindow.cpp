#include "mainwindow.h"
#include "nmeaview.h"
#include "basicview.h"
#include "messageview.h"
#include "satelliteview.h"
#include "snrview.h"
#include "nmeaparser.h"
#include "filemanager.h"
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>
#include <QMap>
#include <QFileInfo>
#include <QSettings>
#include <QCloseEvent>
#include <QSplitter>
#include <QDebug>
#include <QTabWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_isReplaying(false)
    , m_isIntegratedLayout(true)
{
    setWindowTitle("🛰️ 卫星应用软件 - GNSS数据可视化平台");
    setMinimumSize(1200, 800);
    resize(1400, 900);
    
    // 设置窗口属性
    setAttribute(Qt::WA_TranslucentBackground, false);
    
    // 设置窗口缩放策略
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    // 创建核心组件
    m_parser = new NMEAParser(this);
    m_fileManager = new FileManager(this);
    
    // 创建视图（集成到主界面）
    qDebug() << "创建NMEAView...";
    m_nmeaView = new NMEAView(this);
    qDebug() << "创建BasicView...";
    m_basicView = new BasicView(this);
    qDebug() << "创建MessageView...";
    m_messageView = new MessageView(this);
    qDebug() << "创建SatelliteView...";
    m_satelliteView = new SatelliteView(this);
    qDebug() << "创建SNRView...";
    m_snrView = new SNRView(this);
    qDebug() << "所有视图创建完成";
    
    // 设置视图为无边框，集成到主界面
    m_nmeaView->setWindowFlags(Qt::Widget);
    m_basicView->setWindowFlags(Qt::Widget);
    m_messageView->setWindowFlags(Qt::Widget);
    // SatelliteView和SNRView已经在构造函数中设置了正确的标志
    
    // 确保视图可见且可以接收绘制事件
    m_nmeaView->setVisible(true);
    m_basicView->setVisible(true);
    m_messageView->setVisible(true);
    m_satelliteView->setVisible(true);
    m_snrView->setVisible(true);
    
    // 设置绘制属性
    m_satelliteView->setAttribute(Qt::WA_OpaquePaintEvent, true);
    m_satelliteView->setAttribute(Qt::WA_PaintOnScreen, false);
    m_snrView->setAttribute(Qt::WA_OpaquePaintEvent, true);
    m_snrView->setAttribute(Qt::WA_PaintOnScreen, false);
    
    // 强制重绘
    m_satelliteView->update();
    m_snrView->update();
    
    // 视图已集成到主界面，不需要独立窗口
    
    // 设置UI
    setupUI();
    setupMainLayout();
    connectSignals();
    
    // 初始化定时器
    m_replayTimer = new QTimer(this);
    m_replayTimer->setInterval(1000); // 1秒间隔
    connect(m_replayTimer, &QTimer::timeout, m_fileManager, &FileManager::processNextLine);
    
    // 恢复窗口状态
    restoreWindowState();
    
    // 强制提升SatelliteView到最顶层
    m_satelliteView->raise();
    m_satelliteView->activateWindow();
    m_satelliteView->show();
    
    // 确保SatelliteView不被其他控件遮挡
    m_satelliteView->setAttribute(Qt::WA_TranslucentBackground, false);
    m_satelliteView->setWindowOpacity(1.0);
    
    qDebug() << "SatelliteView设置完成 - 可见性:" << m_satelliteView->isVisible()
             << "窗口标志:" << m_satelliteView->windowFlags()
             << "大小:" << m_satelliteView->size()
             << "位置:" << m_satelliteView->pos();
    
    qDebug() << "SNRView设置完成 - 可见性:" << m_snrView->isVisible()
             << "窗口标志:" << m_snrView->windowFlags()
             << "大小:" << m_snrView->size()
             << "位置:" << m_snrView->pos();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    setupMenuBar();
    setupToolBar();
    setupStatusBar();
}

void MainWindow::setupMenuBar()
{
    m_menuBar = menuBar();
    
    // 回放菜单
    m_replayMenu = m_menuBar->addMenu("📁 回放(&R)");
    
    m_startAction = new QAction("▶️ 开始回放(&S)", this);
    m_startAction->setShortcut(QKeySequence("Ctrl+S"));
    m_startAction->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    m_startAction->setToolTip("开始NMEA数据回放");
    m_replayMenu->addAction(m_startAction);
    
    m_stopAction = new QAction("⏹️ 停止回放(&T)", this);
    m_stopAction->setShortcut(QKeySequence("Ctrl+T"));
    m_stopAction->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    m_stopAction->setEnabled(false);
    m_stopAction->setToolTip("停止当前回放");
    m_replayMenu->addAction(m_stopAction);
    
    // 添加分隔符
    m_replayMenu->addSeparator();
    
    // 添加帮助菜单
    QMenu *helpMenu = m_menuBar->addMenu("❓ 帮助(&H)");
    QAction *aboutAction = new QAction("ℹ️ 关于", this);
    aboutAction->setShortcut(QKeySequence("F1"));
    helpMenu->addAction(aboutAction);
}

void MainWindow::setupToolBar()
{
    m_toolBar = addToolBar("主工具栏");
    m_toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_toolBar->setMovable(false);
    
    // 视图切换按钮
    m_nmeaViewAction = new QAction("📄 NMEA视图", this);
    m_nmeaViewAction->setToolTip("显示原始NMEA数据");
    m_toolBar->addAction(m_nmeaViewAction);
    
    m_basicViewAction = new QAction("📍 基础视图", this);
    m_basicViewAction->setToolTip("显示基本定位信息");
    m_toolBar->addAction(m_basicViewAction);
    
    m_messageViewAction = new QAction("📋 消息视图", this);
    m_messageViewAction->setToolTip("显示NMEA消息详情");
    m_toolBar->addAction(m_messageViewAction);
    
    m_satelliteViewAction = new QAction("🛰️ 星位视图", this);
    m_satelliteViewAction->setToolTip("显示卫星雷达图");
    m_toolBar->addAction(m_satelliteViewAction);
    
    m_snrViewAction = new QAction("📊 载噪比视图", this);
    m_snrViewAction->setToolTip("显示信噪比图表");
    m_toolBar->addAction(m_snrViewAction);
    
    // 添加分隔符
    m_toolBar->addSeparator();
    
    // 窗口管理按钮
    m_showAllAction = new QAction("👁️ 显示全部", this);
    m_showAllAction->setToolTip("显示所有视图窗口");
    m_toolBar->addAction(m_showAllAction);
    
    m_hideAllAction = new QAction("🙈 隐藏全部", this);
    m_hideAllAction->setToolTip("隐藏所有视图窗口");
    m_toolBar->addAction(m_hideAllAction);
    
    // 添加分隔符
    m_toolBar->addSeparator();
    
    // 布局切换按钮
    m_toggleLayoutAction = new QAction("🔄 切换布局", this);
    m_toggleLayoutAction->setToolTip("在集成布局和独立窗口间切换");
    m_toolBar->addAction(m_toggleLayoutAction);
}

void MainWindow::setupStatusBar()
{
    m_statusBar = statusBar();
    
    m_statusLabel = new QLabel("🟢 就绪");
    m_statusLabel->setStyleSheet("QLabel { color: #27ae60; font-weight: bold; }");
    
    m_progressBar = new QProgressBar();
    m_progressBar->setVisible(false);
    m_progressBar->setMinimumWidth(200);
    m_progressBar->setTextVisible(true);
    
    m_timeLabel = new QLabel(QDateTime::currentDateTime().toString("🕐 yyyy-MM-dd hh:mm:ss"));
    m_timeLabel->setStyleSheet("QLabel { color: #7f8c8d; font-weight: bold; }");
    
    m_statusBar->addWidget(m_statusLabel);
    m_statusBar->addPermanentWidget(m_progressBar);
    m_statusBar->addPermanentWidget(m_timeLabel);
    
    // 更新时间显示
    QTimer *timeTimer = new QTimer(this);
    connect(timeTimer, &QTimer::timeout, [this]() {
        m_timeLabel->setText(QDateTime::currentDateTime().toString("🕐 yyyy-MM-dd hh:mm:ss"));
    });
    timeTimer->start(1000);
}

void MainWindow::connectSignals()
{
    // 菜单动作
    connect(m_startAction, &QAction::triggered, this, &MainWindow::onStartReplay);
    connect(m_stopAction, &QAction::triggered, this, &MainWindow::onStopReplay);
    
    // 工具栏动作
    connect(m_nmeaViewAction, &QAction::triggered, this, &MainWindow::onShowNMEAView);
    connect(m_basicViewAction, &QAction::triggered, this, &MainWindow::onShowBasicView);
    connect(m_messageViewAction, &QAction::triggered, this, &MainWindow::onShowMessageView);
    connect(m_satelliteViewAction, &QAction::triggered, this, &MainWindow::onShowSatelliteView);
    connect(m_snrViewAction, &QAction::triggered, this, &MainWindow::onShowSNRView);
    connect(m_showAllAction, &QAction::triggered, this, &MainWindow::onShowAllViews);
    connect(m_hideAllAction, &QAction::triggered, this, &MainWindow::onHideAllViews);
    connect(m_toggleLayoutAction, &QAction::triggered, this, &MainWindow::onToggleLayout);
    
    // 数据更新信号
    connect(m_parser, &NMEAParser::dataParsed, this, &MainWindow::onDataUpdated);
    connect(m_fileManager, &FileManager::dataParsed, this, &MainWindow::onDataUpdated);
    
    // 连接文件管理器信号
    connect(m_fileManager, &FileManager::replayFinished, this, &MainWindow::onStopReplay);
    connect(m_fileManager, &FileManager::nmeaDataReceived, m_nmeaView, &NMEAView::addNMEALine);
}

void MainWindow::onStartReplay()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "📁 选择NMEA数据文件",
        "",
        "NMEA文件 (*.nmea *.txt);;文本文件 (*.txt);;所有文件 (*.*)"
    );
    
    if (!fileName.isEmpty()) {
        if (m_fileManager->loadFile(fileName)) {
            m_isReplaying = true;
            m_startAction->setEnabled(false);
            m_stopAction->setEnabled(true);
            m_replayTimer->start();
            m_progressBar->setVisible(true);
            m_progressBar->setValue(0);
            m_statusLabel->setText(QString("▶️ 正在回放: %1").arg(QFileInfo(fileName).fileName()));
    } else {
            QMessageBox::critical(this, "❌ 错误", QString("无法打开文件: %1").arg(fileName));
        }
    }
}

void MainWindow::onStopReplay()
{
    m_isReplaying = false;
    m_replayTimer->stop();
    m_startAction->setEnabled(true);
    m_stopAction->setEnabled(false);
    m_progressBar->setVisible(false);
    m_statusLabel->setText("⏹️ 回放已停止");
}

void MainWindow::onDataUpdated(const SatelliteData &data)
{
    // 调试信息
    qDebug() << "MainWindow::onDataUpdated - 卫星数:" << data.satelliteCount 
             << "纬度:" << data.latitude << "经度:" << data.longitude
             << "卫星列表大小:" << data.satellites.size();
    
    // 更新所有视图
    m_nmeaView->updateData(data);
    m_basicView->updateData(data);
    m_messageView->updateData(data);
    m_satelliteView->updateData(data);
    m_snrView->updateData(data);
    
    // 更新状态栏
    QString statusText = QString("🛰️ 数据更新 - 卫星数: %1 | 定位: %2").arg(data.satelliteCount).arg(data.fixType);
    m_statusLabel->setText(statusText);
    
    // 更新进度条
    if (m_isReplaying) {
        int progress = m_fileManager->getCurrentLine() * 100 / m_fileManager->getTotalLines();
        m_progressBar->setValue(progress);
    }
}

void MainWindow::onShowNMEAView()
{
    if (m_nmeaView->isVisible()) {
        m_nmeaView->hide();
        m_nmeaViewAction->setText("📄 NMEA视图");
        m_nmeaViewAction->setToolTip("显示原始NMEA数据");
    } else {
        m_nmeaView->show();
        m_nmeaViewAction->setText("📄 NMEA视图 ✓");
        m_nmeaViewAction->setToolTip("隐藏NMEA视图");
    }
}

void MainWindow::onShowBasicView()
{
    if (m_basicView->isVisible()) {
        m_basicView->hide();
        m_basicViewAction->setText("📍 基础视图");
        m_basicViewAction->setToolTip("显示基本定位信息");
    } else {
        m_basicView->show();
        m_basicViewAction->setText("📍 基础视图 ✓");
        m_basicViewAction->setToolTip("隐藏基础视图");
    }
}

void MainWindow::onShowMessageView()
{
    if (m_messageView->isVisible()) {
        m_messageView->hide();
        m_messageViewAction->setText("📋 消息视图");
        m_messageViewAction->setToolTip("显示NMEA消息详情");
    } else {
        m_messageView->show();
        m_messageViewAction->setText("📋 消息视图 ✓");
        m_messageViewAction->setToolTip("隐藏消息视图");
    }
}

void MainWindow::onShowSatelliteView()
{
    if (m_satelliteView->isVisible()) {
        m_satelliteView->hide();
        m_satelliteViewAction->setText("🛰️ 星位视图");
        m_satelliteViewAction->setToolTip("显示卫星雷达图");
    } else {
        m_satelliteView->show();
        m_satelliteViewAction->setText("🛰️ 星位视图 ✓");
        m_satelliteViewAction->setToolTip("隐藏星位视图");
    }
}

void MainWindow::onShowSNRView()
{
    if (m_snrView->isVisible()) {
        m_snrView->hide();
        m_snrViewAction->setText("📊 载噪比视图");
        m_snrViewAction->setToolTip("显示信噪比图表");
    } else {
        m_snrView->show();
        m_snrViewAction->setText("📊 载噪比视图 ✓");
        m_snrViewAction->setToolTip("隐藏载噪比视图");
    }
}

void MainWindow::onShowAllViews()
{
    m_nmeaView->show();
    m_basicView->show();
    m_messageView->show();
    m_satelliteView->show();
    m_snrView->show();
    
    // 更新按钮状态
    m_nmeaViewAction->setText("📄 NMEA视图 ✓");
    m_basicViewAction->setText("📍 基础视图 ✓");
    m_messageViewAction->setText("📋 消息视图 ✓");
    m_satelliteViewAction->setText("🛰️ 星位视图 ✓");
    m_snrViewAction->setText("📊 载噪比视图 ✓");
    
    m_statusLabel->setText("👁️ 所有视图已显示");
}

void MainWindow::onHideAllViews()
{
    m_nmeaView->hide();
    m_basicView->hide();
    m_messageView->hide();
    m_satelliteView->hide();
    m_snrView->hide();
    
    // 更新按钮状态
    m_nmeaViewAction->setText("📄 NMEA视图");
    m_basicViewAction->setText("📍 基础视图");
    m_messageViewAction->setText("📋 消息视图");
    m_satelliteViewAction->setText("🛰️ 星位视图");
    m_snrViewAction->setText("📊 载噪比视图");
    
    m_statusLabel->setText("🙈 所有视图已隐藏");
}

void MainWindow::onToggleLayout()
{
    if (m_isIntegratedLayout) {
        // 切换到独立窗口模式
        m_nmeaView->setWindowFlags(Qt::Window);
        m_basicView->setWindowFlags(Qt::Window);
        m_messageView->setWindowFlags(Qt::Window);
        m_satelliteView->setWindowFlags(Qt::Window);
        m_snrView->setWindowFlags(Qt::Window);
        
        // 隐藏集成布局
        if (m_mainSplitter) {
            m_mainSplitter->hide();
        }
        
        // 显示所有窗口
        m_nmeaView->show();
        m_basicView->show();
        m_messageView->show();
        m_satelliteView->show();
        m_snrView->show();
        
        m_toggleLayoutAction->setText("🔄 集成布局");
        m_toggleLayoutAction->setToolTip("切换到集成布局模式");
        m_statusLabel->setText("🪟 已切换到独立窗口模式");
        
        m_isIntegratedLayout = false;
    } else {
        // 切换到集成布局模式
        m_nmeaView->setWindowFlags(Qt::Widget);
        m_basicView->setWindowFlags(Qt::Widget);
        m_messageView->setWindowFlags(Qt::Widget);
        m_satelliteView->setWindowFlags(Qt::Widget);
        m_snrView->setWindowFlags(Qt::Widget);
        
        // 重新设置布局
        setupMainLayout();
        
        m_toggleLayoutAction->setText("🔄 独立窗口");
        m_toggleLayoutAction->setToolTip("切换到独立窗口模式");
        m_statusLabel->setText("📱 已切换到集成布局模式");
        
        m_isIntegratedLayout = true;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // 保存窗口状态
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    
    // 保存分割器状态
    if (m_mainSplitter) {
        settings.setValue("mainSplitterSizes", m_mainSplitter->saveState());
    }
    if (m_leftSplitter) {
        settings.setValue("leftSplitterSizes", m_leftSplitter->saveState());
    }
    if (m_rightSplitter) {
        settings.setValue("rightSplitterSizes", m_rightSplitter->saveState());
    }
    
    QMainWindow::closeEvent(event);
}

void MainWindow::setupMainLayout()
{
    // 创建主分割器（水平分割）
    m_mainSplitter = new QSplitter(Qt::Horizontal, this);
    setCentralWidget(m_mainSplitter);
    
    // 创建左侧分割器（垂直分割）
    m_leftSplitter = new QSplitter(Qt::Vertical, this);
    m_leftSplitter->addWidget(m_basicView);
    m_leftSplitter->addWidget(m_nmeaView);
    m_leftSplitter->setSizes({300, 400});
    
    // 创建右侧分割器（垂直分割）
    m_rightSplitter = new QSplitter(Qt::Vertical, this);
    m_rightSplitter->addWidget(m_snrView);  // 先添加SNRView
    m_rightSplitter->addWidget(m_satelliteView);  // 后添加SatelliteView，确保在最上层
    m_rightSplitter->setSizes({300, 400});  // 调整大小，给SatelliteView更多空间
    
    // 创建消息视图标签页
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->addTab(m_messageView, "📋 NMEA消息详情");
    
    // 添加到主分割器
    m_mainSplitter->addWidget(m_leftSplitter);
    m_mainSplitter->addWidget(m_rightSplitter);
    m_mainSplitter->addWidget(m_tabWidget);
    
    // 设置分割器比例
    m_mainSplitter->setSizes({400, 400, 300});
    
    // 调试：检查分割器中的控件
    qDebug() << "主分割器控件数量:" << m_mainSplitter->count();
    qDebug() << "右侧分割器控件数量:" << m_rightSplitter->count();
    for (int i = 0; i < m_rightSplitter->count(); ++i) {
        QWidget* widget = m_rightSplitter->widget(i);
        qDebug() << "右侧分割器控件" << i << ":" << widget->objectName() 
                 << "可见性:" << widget->isVisible() << "大小:" << widget->size();
    }
    
    // 设置分割器样式
    m_mainSplitter->setStyleSheet(
        "QSplitter::handle {"
        "    background-color: #4A90E2;"
        "    border: 1px solid #357ABD;"
        "    border-radius: 2px;"
        "}"
        "QSplitter::handle:hover {"
        "    background-color: #5BA0F2;"
        "}"
    );
    
    m_leftSplitter->setStyleSheet(
        "QSplitter::handle {"
        "    background-color: #4A90E2;"
        "    border: 1px solid #357ABD;"
        "    border-radius: 2px;"
        "}"
    );
    
    m_rightSplitter->setStyleSheet(
        "QSplitter::handle {"
        "    background-color: #4A90E2;"
        "    border: 1px solid #357ABD;"
        "    border-radius: 2px;"
        "}"
    );
}

void MainWindow::restoreWindowState()
{
    QSettings settings;
    
    // 恢复主窗口几何形状
    QByteArray geometry = settings.value("geometry").toByteArray();
    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    }
    
    // 恢复窗口状态
    QByteArray windowState = settings.value("windowState").toByteArray();
    if (!windowState.isEmpty()) {
        restoreState(windowState);
    }
    
    // 恢复分割器状态
    QByteArray mainSplitterState = settings.value("mainSplitterSizes").toByteArray();
    if (!mainSplitterState.isEmpty() && m_mainSplitter) {
        m_mainSplitter->restoreState(mainSplitterState);
    }
    
    QByteArray leftSplitterState = settings.value("leftSplitterSizes").toByteArray();
    if (!leftSplitterState.isEmpty() && m_leftSplitter) {
        m_leftSplitter->restoreState(leftSplitterState);
    }
    
    QByteArray rightSplitterState = settings.value("rightSplitterSizes").toByteArray();
    if (!rightSplitterState.isEmpty() && m_rightSplitter) {
        m_rightSplitter->restoreState(rightSplitterState);
    }
}
