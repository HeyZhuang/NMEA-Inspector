#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QTimer>
#include <QProgressBar>
#include <QLabel>
#include <QMap>
#include <QSplitter>
#include <QTabWidget>
#include <QCloseEvent>
#include "satellitedata.h"

class NMEAView;
class BasicView;
class MessageView;
class SatelliteView;
class SNRView;
class NMEAParser;
class FileManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartReplay();
    void onStopReplay();
    void onDataUpdated(const SatelliteData &data);
    void onShowNMEAView();
    void onShowBasicView();
    void onShowMessageView();
    void onShowSatelliteView();
    void onShowSNRView();
    void onShowAllViews();
    void onHideAllViews();
    void onToggleLayout();
    void closeEvent(QCloseEvent *event) override;

private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void setupMainLayout();
    void connectSignals();
    void restoreWindowState();
    
    // UI组件
    QMenuBar *m_menuBar;
    QToolBar *m_toolBar;
    QStatusBar *m_statusBar;
    
    // 菜单和动作
    QMenu *m_replayMenu;
    QAction *m_startAction;
    QAction *m_stopAction;
    
    // 工具栏动作
    QAction *m_nmeaViewAction;
    QAction *m_basicViewAction;
    QAction *m_messageViewAction;
    QAction *m_satelliteViewAction;
    QAction *m_snrViewAction;
    QAction *m_showAllAction;
    QAction *m_hideAllAction;
    QAction *m_toggleLayoutAction;
    
    // 状态栏组件
    QLabel *m_statusLabel;
    QProgressBar *m_progressBar;
    QLabel *m_timeLabel;
    
    // 视图窗口（集成到主界面）
    NMEAView *m_nmeaView;
    BasicView *m_basicView;
    MessageView *m_messageView;
    SatelliteView *m_satelliteView;
    SNRView *m_snrView;
    
    // 主界面布局组件
    QTabWidget *m_tabWidget;
    QSplitter *m_mainSplitter;
    QSplitter *m_leftSplitter;
    QSplitter *m_rightSplitter;
    bool m_isIntegratedLayout;
    
    // 核心组件
    NMEAParser *m_parser;
    FileManager *m_fileManager;
    
    // 定时器
    QTimer *m_replayTimer;
    bool m_isReplaying;
};

#endif // MAINWINDOW_H