#ifndef PROTREETHREAD_H
#define PROTREETHREAD_H

#include <QThread>
#include <QTreeWidget>
#include <QTreeWidgetItem>
class ProTreeThread : public QThread
{
    Q_OBJECT
public:
    ProTreeThread(const QString& src_path, const QString& dest_path, QTreeWidgetItem* parent_item,
            int file_count, QTreeWidget* self, QTreeWidgetItem* root, QObject *parent = nullptr);
    ~ProTreeThread();
protected:
    virtual void run();

private:
    void CreateProTree(const QString& src_path, const QString& dest_path, QTreeWidgetItem* parent_item,
                       int& file_count, QTreeWidget* self, QTreeWidgetItem* root,
                       QTreeWidgetItem* pre_item = nullptr);


private:
    QString _src_path;
    QString _dest_path;
    int _file_count;
    QTreeWidget* _self;
    QTreeWidgetItem* _root;
    QTreeWidgetItem* _parent_item;
    bool _bstop;
public slots:
    void slotCancelProgress();
signals:
    void sigProgressUpdated(int);
    void sigProgressFinished(int);
};

#endif // PROTREETHREAD_H
