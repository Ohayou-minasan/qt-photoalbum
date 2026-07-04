#include "protreeitem.h"
#include "const.h"
ProTreeItem::ProTreeItem(QTreeWidget* view, const QString& name, const QString& path,
                         int type)
    :QTreeWidgetItem(view,type),_name(name), _path(path),_root(this), _pre_item(nullptr), _next_item(nullptr)
{

}

ProTreeItem::ProTreeItem(QTreeWidgetItem *parent, const QString &name, const QString &path,
                        QTreeWidgetItem *root, int type)
    :QTreeWidgetItem(parent, type),_name(name), _path(path), _root(root), _pre_item(nullptr), _next_item(nullptr)
{

}

const QString &ProTreeItem::GetPath()
{
    return _path;
}

QTreeWidgetItem *ProTreeItem::GetRoot()
{
    return _root;
}

void ProTreeItem::SetPreItem(QTreeWidgetItem *item)
{
    _pre_item = item;
}

void ProTreeItem::SetNextItem(QTreeWidgetItem *item)
{
    _next_item = item;
}

ProTreeItem *ProTreeItem::GetNextItem()
{
    return dynamic_cast<ProTreeItem*>(_next_item);
}

ProTreeItem *ProTreeItem::GetPreItem()
{
    return dynamic_cast<ProTreeItem*>(_pre_item);
}

ProTreeItem *ProTreeItem::GetLastPicChild()
{
    if(this->type() == TreeItemPic){
        return nullptr;
    }
    int childCount = this->childCount();
    if(0 == childCount){
        return nullptr;
    }
    for(int i = childCount - 1; i >= 0; i--){
        QTreeWidgetItem* lastChild = this->child(i);
        ProTreeItem* lastTreeItem = dynamic_cast<ProTreeItem*>(lastChild);
        if(lastTreeItem->type() == TreeItemPic){
            return lastTreeItem;
        }
        lastChild = lastTreeItem->GetLastPicChild();
        if(!lastChild){
            continue;
        }
        lastTreeItem = dynamic_cast<ProTreeItem*>(lastChild);
        return lastTreeItem;
    }

    return nullptr;
}

ProTreeItem *ProTreeItem::GetFirstPicChild()
{
    if(this->type() == TreeItemPic){
        return nullptr;
    }
    int childCount = this->childCount();
    if(0 == childCount){
        return nullptr;
    }

    for(int i = 0; i < childCount; i++){
        QTreeWidgetItem* firstChild = this->child(i);
        ProTreeItem* firstTreeChild = dynamic_cast<ProTreeItem*>(firstChild);
        if(firstTreeChild->type() == TreeItemPic){
            return firstTreeChild;
        }
        firstChild = firstTreeChild->GetFirstPicChild();
        if(!firstChild){
            continue;
        }

        firstTreeChild = dynamic_cast<ProTreeItem*>(firstChild);
        return firstTreeChild;
    }
    return nullptr;
}
