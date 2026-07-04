#include "opentreethread.h"
#include <QDir>
#include <protreeitem.h>
#include "const.h"
OpenTreeThread::OpenTreeThread(const QString& src_path, int file_count, QTreeWidget* self,
                               QObject* parent)
    : QThread{parent},mb_stop(false), m_src_path(src_path), m_file_count(file_count),
    m_self(self),m_root(nullptr)
{

}

void OpenTreeThread::openProTree(const QString &src_path, int &file_count, QTreeWidget *self)
{
    QDir src_dir(src_path);
    QString name = src_dir.dirName();

    ProTreeItem* item = new ProTreeItem(self, name, src_path, TreeItemPro);
    item->setData(0, Qt::DisplayRole, name);
    item->setData(0, Qt::DecorationRole, QIcon(":/icon/dir.png"));
    item->setData(0, Qt::ToolTipRole, src_path);
    m_root = item;
    recursiveProTree(src_path, file_count, self, m_root, item, nullptr);
}

void OpenTreeThread::run()
{
    openProTree(m_src_path, m_file_count, m_self);
    if(mb_stop){
        QString path = dynamic_cast<ProTreeItem*>(m_root)->GetPath();
        int index = m_self->indexOfTopLevelItem(m_root);
        delete m_self->takeTopLevelItem(index);
        QDir dir(path);
        dir.removeRecursively();
        return;
    }

    emit sigProgressFinished(m_file_count);
}

void OpenTreeThread::recursiveProTree(const QString &src_path, int &file_count,
                                      QTreeWidget *self, QTreeWidgetItem *root,
                                      QTreeWidgetItem *parent, QTreeWidgetItem *pre_item)
{
    QDir src_dir(src_path);
    src_dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    src_dir.setSorting(QDir::Name);

    QFileInfoList list = src_dir.entryInfoList();
    for(const auto elem : list){
        if(mb_stop){
            return;
        }
        QFileInfo file_info = elem;
        bool isDir = file_info.isDir();
        if(isDir){
            if(mb_stop){
                return;
            }
            file_count++;
            emit sigProgressUpdated(file_count);
            ProTreeItem* item = new ProTreeItem(parent, file_info.fileName(),
                                                file_info.absoluteFilePath(),
                                                m_root, TreeItemDir);
            item->setData(0, Qt::DisplayRole, file_info.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/icon/dir.png"));
            item->setData(0, Qt::ToolTipRole, file_info.absoluteFilePath());
            recursiveProTree(file_info.absoluteFilePath(), file_count,
                             self, root, item, pre_item);
        }
        else{
            if(mb_stop){
                return;
            }

            const QString& suffix = file_info.completeSuffix();
            if(suffix != "png" and suffix != "jpg" and suffix != "jpeg"){
                continue;
            }

            file_count++;
            emit sigProgressUpdated(file_count);

            ProTreeItem* item = new ProTreeItem(parent, file_info.fileName(),
                                                file_info.absoluteFilePath(),
                                                m_root, TreeItemPic);
            item->setData(0, Qt::DisplayRole, file_info.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/icon/pic.png"));
            item->setData(0, Qt::ToolTipRole, file_info.absoluteFilePath());
            if(pre_item){
                ProTreeItem* pre_pro_item = dynamic_cast<ProTreeItem*>(pre_item);
                pre_pro_item->SetNextItem(item);
            }
            item->SetPreItem(pre_item);
            pre_item = item;
        }
    }
    emit sigProgressFinished(file_count);
}

void OpenTreeThread::slotCancelProgress()
{
    this->mb_stop = true;
}
