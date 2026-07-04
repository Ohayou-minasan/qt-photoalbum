#include "prelistwidget.h"
#include "protreeitem.h"
#include "const.h"
#include <QPainter>
#include "prelistitem.h"
#include <QGuiApplication>
PreListWidget::PreListWidget(QWidget* parent)
    :QListWidget(parent), m_globalCnt(0), m_lastIndex(17)
{
    this->setViewMode(QListView::IconMode);
    this->setIconSize(QSize(PREICON_SIZE, PREICON_SIZE));
    this->setSpacing(5);

    connect(this, &PreListWidget::itemPressed, this, &PreListWidget::slotItemPressed);
}

PreListWidget::~PreListWidget()
{

}

void PreListWidget::addListItem(const QString &path)
{
    QPixmap srcPixmap(path);
    srcPixmap = srcPixmap.scaled(PREICON_SIZE, PREICON_SIZE, Qt::KeepAspectRatio);

    QPixmap dstPixmap(QSize(PREICON_SIZE, PREICON_SIZE));
    dstPixmap.fill(QColor(220, 220, 220, 50));

    QPainter painter(&dstPixmap);
    int srcWidth = srcPixmap.width();
    int srcHeight = srcPixmap.height();
    int dstWidth = dstPixmap.width();
    int dstHeight = dstPixmap.height();

    //居中
    int x = (dstWidth - srcWidth) / 2;
    int y = (dstHeight - srcHeight) / 2;

    painter.drawPixmap(x, y, srcPixmap);
    m_globalCnt++;

    PreListItem* pItem = new PreListItem(QIcon(dstPixmap), path, m_globalCnt, this);
    pItem->setSizeHint(QSize(PREITEM_SIZE, PREITEM_SIZE));
    this->addItem(pItem);
    m_mapItems[path] = pItem;
    if(m_globalCnt == 1){
        m_posOrigin = this->pos();

    }
}

void PreListWidget::slotUpdatePreList(QTreeWidgetItem *item)
{
    if(!item){
        qDebug() << "tree item is empty().";
        return;
    }

    ProTreeItem* proItem = dynamic_cast<ProTreeItem*>(item);
    QString path = proItem->GetPath();
    QMap<QString, QListWidgetItem*>::iterator iter = m_mapItems.find(path);
    if(iter != m_mapItems.end()){
        qDebug() << " path " << path << "exists";
        return;
    }
    addListItem(path);
}

void PreListWidget::slotSelectPreList(QTreeWidgetItem *item)
{
    if(!item){
        return;
    }

    ProTreeItem* proItem = dynamic_cast<ProTreeItem*>(item);
    QString path = proItem->GetPath();
    QMap<QString, QListWidgetItem*>::iterator iter = m_mapItems.find(path);
    if(iter == m_mapItems.end()){
        return;
    }

    PreListItem* listItem = dynamic_cast<PreListItem*>(iter.value());
    int index = listItem->getIndex();
    if(index > 17){
        QPoint curPos = this->pos();
        this->move(curPos.x() - (index - m_lastIndex)*100, curPos.y());
        m_lastIndex = index;
    }
    else{
        this->move(m_posOrigin);
        m_lastIndex = 17;
    }

    this->setCurrentItem(iter.value());
}

void PreListWidget::slotItemPressed(QListWidgetItem *item)
{
    if(QGuiApplication::mouseButtons() != Qt::LeftButton){
        return;
    }

    PreListItem* listItem = dynamic_cast<PreListItem*>(item);
    int curIndex = listItem->getIndex();
    QString path = listItem->getPath();
    this->setCurrentItem(item);
    emit sigSelectItemUpdated(path);
}
