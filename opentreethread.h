#ifndef OPENTREETHREAD_H
#define OPENTREETHREAD_H

#include <QThread>
#include <QTreeWidget>
class OpenTreeThread : public QThread
{
    Q_OBJECT
public:
    explicit OpenTreeThread(const QString& src_path, int file_count, QTreeWidget* self,
                            QObject* parent = nullptr);
    void openProTree(const QString& src_path, int& file_count, QTreeWidget* self);
protected:
    virtual void run();
private:
    void recursiveProTree(const QString& src_path, int& file_count, QTreeWidget* self,
                          QTreeWidgetItem* root, QTreeWidgetItem* parent, QTreeWidgetItem* pre_item);

private:
    //源文件路径
    QString m_src_path;
    //文件数量
    int m_file_count;
    //所属Widget
    QTreeWidget* m_self;
    //线程停止控制
    bool mb_stop;
    //item的根节点
    QTreeWidgetItem* m_root;
signals:
    void sigProgressFinished(int);
    void sigProgressUpdated(int);
public slots:
    void slotCancelProgress();
};

#endif // OPENTREETHREAD_H
