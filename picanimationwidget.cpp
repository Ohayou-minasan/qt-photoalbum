#include "picanimationwidget.h"
#include "protreeitem.h"
#include <QPainter>

PicAnimationWidget::PicAnimationWidget(QWidget *parent)
    : QWidget{parent}, m_factor(0.0), m_curItem(nullptr), m_start(false)
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &PicAnimationWidget::doTimeOut);
}

PicAnimationWidget::~PicAnimationWidget()
{

}

//显示图片
void PicAnimationWidget::setPixmap(QTreeWidgetItem *item)
{
    if(!item){
        return;
    }

    ProTreeItem* treeItem = dynamic_cast<ProTreeItem*> (item);
    QString path = treeItem->GetPath();

    //双缓冲第一次加载
    m_pixmap1.load(path);
    m_curItem = treeItem;

    if(m_mapItems.find(path) == m_mapItems.end()){
        m_mapItems[path] = treeItem;
        //发送更新预览列表逻辑
        emit sigPreListUpdated(item);
    }


    emit sigPreListSelected(item);

    //双缓冲第二次加载
    ProTreeItem* nextTreeItem = treeItem->GetNextItem();
    if(!nextTreeItem){
        return;
    }

    QString nextItemPath = nextTreeItem->GetPath();
    m_pixmap2.load(nextItemPath);
    if(m_mapItems.find(nextItemPath) == m_mapItems.end()){
        m_mapItems[nextItemPath] = nextTreeItem;
        //发送更新预览列表逻辑
        emit sigPreListUpdated(nextTreeItem);
    }



}

void PicAnimationWidget::start()
{
    //发送轮播图片信号
    emit sigStart();

    //开始播放音乐
    emit sigStartMusic();
    m_factor = 0.0;
    m_timer->start(25);
    m_start = true;

}

void PicAnimationWidget::stop()
{
    //发送暂停轮播图片信号
    emit sigStop();
    //暂停播放音乐
    emit sigStopMusic();
    m_timer->stop();
    m_factor = 0.0;
    m_start = false;
}

void PicAnimationWidget::slideNext()
{
    stop();
    if(!m_curItem){
        return;
    }

    ProTreeItem* curProItem = dynamic_cast<ProTreeItem*>(m_curItem);
    ProTreeItem* nextProItem = curProItem->GetNextItem();
    if(!nextProItem){
        return;
    }

    this->setPixmap(nextProItem);
    update();
}

void PicAnimationWidget::slidePre()
{
    stop();
    if(!m_curItem){
        return;
    }

    ProTreeItem* curProItem = dynamic_cast<ProTreeItem*>(m_curItem);
    ProTreeItem* PreProItem = curProItem->GetPreItem();

    if(!PreProItem){
        return;
    }

    this->setPixmap(PreProItem);
    //手动刷新,触发paintEvent()
    update();
}

void PicAnimationWidget::paintEvent(QPaintEvent *event)
{
    if(m_pixmap1.isNull()){
        return;
    }
    //在区域上绘图
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QRect rect = this->geometry();
    int width = rect.width();
    int height = rect.height();

    m_pixmap1 = m_pixmap1.scaled(width, height, Qt::KeepAspectRatio);
    //alpha通道值
    int alpha = 255 * (1.0f - m_factor);

    QPixmap alphaPixmap(m_pixmap1.size());
    alphaPixmap.fill(Qt::transparent);

    //p1在Pixmap上绘图
    QPainter p1(&alphaPixmap);
    p1.setCompositionMode(QPainter::CompositionMode_Source);
    p1.drawPixmap(0, 0, m_pixmap1);
    p1.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p1.fillRect(alphaPixmap.rect(), QColor(0, 0, 0, alpha));
    p1.end();

    int x = (width - m_pixmap1.width()) / 2;
    int y = (height - m_pixmap1.height()) / 2;
    painter.drawPixmap(x, y, alphaPixmap);


    if(m_pixmap2.isNull()){
        return;
    }

    m_pixmap2 = m_pixmap2.scaled(width, height, Qt::KeepAspectRatio);
    alpha = 255 * m_factor;
    QPixmap alphaPixmap2(m_pixmap2.size());
    alphaPixmap2.fill(Qt::transparent);
    QPainter p2(&alphaPixmap2);
    p2.setCompositionMode(QPainter::CompositionMode_Source);
    p2.drawPixmap(0, 0, m_pixmap2);
    p2.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p2.fillRect(alphaPixmap2.rect(), QColor(0, 0, 0, alpha));
    p2.end();
    x = (width - m_pixmap2.width()) / 2;
    y = (height - m_pixmap2.height()) / 2;
    painter.drawPixmap(x, y, alphaPixmap2);
}


//选中预览区的item之后,更新播放顺序
void PicAnimationWidget::UpdateSelectItemPixmap(QTreeWidgetItem *item)
{
    if(!item){
        return;
    }

    ProTreeItem* treeItem =  dynamic_cast<ProTreeItem*>(item);
    QString path = treeItem->GetPath();
    m_pixmap1.load(path);
    m_curItem = treeItem;


    if(m_mapItems.find(path) == m_mapItems.end()){
        m_mapItems[path] = treeItem;
    }

    ProTreeItem* nextItem =  dynamic_cast<ProTreeItem*>(item);

    if(!nextItem){
        return;
    }

    QString nextItemPath = nextItem->GetPath();
    m_pixmap2.load(nextItemPath);
    if(m_mapItems.find(nextItemPath) == m_mapItems.end()){
        m_mapItems[nextItemPath] = nextItem;
    }

}

void PicAnimationWidget::doTimeOut()
{
    if(!m_curItem){
        stop();
        update();
        return;
    }

    m_factor += 0.01;
    if(m_factor >= 1){
        m_factor = 0.0;
        ProTreeItem* curProItem = dynamic_cast<ProTreeItem*>(m_curItem);
        ProTreeItem* nextProItem = curProItem->GetNextItem();
        if(!nextProItem){
            stop();
            update();
            return;
        }

        setPixmap(nextProItem);
        update();
        return;
    }

    update();

}


//显示选中的item的图片
void PicAnimationWidget::slotUpdateSelectItem(QString path)
{
    QMap<QString, QTreeWidgetItem*>::iterator iter = m_mapItems.find(path);
    if(iter == m_mapItems.end()){
        return;
    }

    UpdateSelectItemPixmap(iter.value());
    update();
}

void PicAnimationWidget::slotStartOrPause()
{
    if(!m_start){
        m_factor = 0;
        m_timer->start(25);
        m_start = true;
        //开始播放音乐
        emit sigStartMusic();
    }
    else{
        m_timer->stop();
        m_factor = 0;
        update();
        m_start = false;

        //停止播放音乐
        emit sigStopMusic();
    }
}
