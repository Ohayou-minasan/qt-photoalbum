#include "prelistitem.h"

//不显示path,内部通过m_path记录path
PreListItem::PreListItem(const QIcon &icon, const QString &text, const int &index,
                         QListWidget *listview, int type)
    :QListWidgetItem(icon, "", listview, type), m_path(text), m_index(index)
{

}

QString PreListItem::getPath() const
{
    return m_path;
}

int PreListItem::getIndex() const
{
    return m_index;
}
