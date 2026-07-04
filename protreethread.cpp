#include "protreethread.h"
#include <QDir>
#include "protreeitem.h"
#include "const.h"

ProTreeThread::ProTreeThread(const QString &src_path, const QString &dest_path,
                             QTreeWidgetItem *parent_item, int file_count,
                             QTreeWidget *self, QTreeWidgetItem *root, QObject *parent)
    :QThread(parent), _src_path(src_path), _dest_path(dest_path),
    _parent_item(parent_item),_file_count(file_count),
    _root(root),_self(self), _bstop(false)
{

}

ProTreeThread::~ProTreeThread()
{
    delete _parent_item;
    delete _root;
    delete _self;
    _parent_item = nullptr;
    _root = nullptr;
    _self = nullptr;
    _bstop = false;
    _file_count = 0;
}

void ProTreeThread::run()
{
    CreateProTree(_src_path, _dest_path, _parent_item, _file_count, _self, _root);
    if(_bstop){
        QString path = dynamic_cast<ProTreeItem*>(_root)->GetPath();
        int index = _self->indexOfTopLevelItem(_root);
        delete _self->takeTopLevelItem(index);
        QDir dir(path);
        dir.removeRecursively();
        return;
    }

    emit sigProgressFinished(_file_count);
}



void ProTreeThread::CreateProTree(const QString &src_path, const QString &dest_path,
                                  QTreeWidgetItem *parent_item, int &file_count,
                                  QTreeWidget *self, QTreeWidgetItem *root,
                                  QTreeWidgetItem *pre_item)
{
    if(_bstop){
        return;
    }
    bool need_copy = true;
    if(src_path == dest_path){
        need_copy = false;
    }

    QDir import_dir(src_path);
    qDebug() << "src_path is " << src_path << "dis_path is " << dest_path << '\n';
    //过滤器
    QStringList nameFilters;
    import_dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    import_dir.setSorting(QDir::Name);
    QFileInfoList list = import_dir.entryInfoList();
    qDebug() << "list.size " << list.size() << '\n';
    for(int i = 0; i < list.size(); i++){
        if(_bstop){
            return;
        }
        QFileInfo fileInfo = list.at(i);
        bool is_dir = fileInfo.isDir();
        if(is_dir){
            if(_bstop)
                return;


            file_count++;
            emit sigProgressUpdated(file_count);

            QDir dest_dir(_dest_path);
            QString sub_dest_path = dest_dir.absoluteFilePath(fileInfo.fileName());
            QDir sub_dest_dir(sub_dest_path);
            if(!sub_dest_dir.exists()){
                bool ok = sub_dest_dir.mkpath(sub_dest_path);
                if(!ok){
                    continue;
                }
            }
            ProTreeItem* item = new ProTreeItem(parent_item, fileInfo.fileName(), sub_dest_path,
                                                root, TreeItemDir);
            item->setData(0, Qt::DisplayRole, fileInfo.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/icon/dir.png"));
            item->setData(0, Qt::ToolTipRole, sub_dest_path);
            CreateProTree(fileInfo.absoluteFilePath(), sub_dest_path,
                          item,file_count, self, root, pre_item);
        }
        else{
            if(_bstop)
                return;

            const QString& suffix = fileInfo.completeSuffix();
            if(suffix != "png" && suffix != "jpg" && suffix != "jpeg"){
                continue;
            }
            file_count++;
            //文件数量变了，通知更新
            emit sigProgressUpdated(file_count);
            if(not need_copy){
                continue;
            }


            QDir dest_dir(dest_path);
            QString dest_file_path = dest_dir.absoluteFilePath(fileInfo.fileName());
            if(!QFile::copy(fileInfo.absoluteFilePath(), dest_file_path)){
                continue;
            }

            ProTreeItem* item = new ProTreeItem(parent_item, fileInfo.fileName(),dest_file_path,
                                                root, TreeItemPic);
            item->setData(0, Qt::DisplayRole, fileInfo.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/icon/pic.png"));
            item->setData(0, Qt::ToolTipRole, dest_file_path);

            //串联
            if(pre_item){
                auto* pre_pro_item = dynamic_cast<ProTreeItem*>(pre_item);
                pre_pro_item->SetNextItem(item);
            }
            item->SetPreItem(pre_item);
            pre_item = item;
        }
    }
}

void ProTreeThread::slotCancelProgress()
{
    this->_bstop = true;
}
