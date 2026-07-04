#ifndef PRELISTWIDGET_H
#define PRELISTWIDGET_H

#include <QListWidget>
#include <QTreeWidgetItem>
#include <QMap>
class PreListWidget : public QListWidget
{
    Q_OBJECT
public:
    PreListWidget(QWidget* parent = nullptr);
    ~PreListWidget();

private:
    //path与对应的item组成Key-Value
    QMap<QString, QListWidgetItem*> m_mapItems;
    int m_globalCnt;
    QPoint m_posOrigin;

    //记录上一次item的位置
    //预览的位置总共17项,超过17项实现预览区域的滚动
    int m_lastIndex;
private:
    void addListItem(const QString& path);
public slots:
    void slotUpdatePreList(QTreeWidgetItem* item);
    void slotSelectPreList(QTreeWidgetItem* item);
    void slotItemPressed(QListWidgetItem *item);
signals:
    void sigSelectItemUpdated(QString path);
};

#endif // PRELISTWIDGET_H
