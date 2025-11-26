#ifndef MESSAGEVIEW_H
#define MESSAGEVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QTableWidget>
#include <QSplitter>
#include <QGroupBox>
#include <QMap>
#include "satellitedata.h"

class MessageView : public QWidget
{
    Q_OBJECT

public:
    explicit MessageView(QWidget *parent = nullptr);
    
    void updateData(const SatelliteData &data);

private slots:
    void onTreeItemClicked(QTreeWidgetItem *item, int column);

private:
    void setupUI();
    void setupTreeWidget();
    void setupTableWidget();
    void updateTableData(const QString &messageType, const QString &system = "");
    void updateTreeData();
    void addTreeItem(QTreeWidgetItem *parent, const QString &name, const QString &value);
    
    // UI组件
    QSplitter *m_splitter;
    QGroupBox *m_treeGroup;
    QGroupBox *m_tableGroup;
    QTreeWidget *m_treeWidget;
    QTableWidget *m_tableWidget;
    
    // 数据存储
    SatelliteData m_currentData;
};

#endif // MESSAGEVIEW_H
