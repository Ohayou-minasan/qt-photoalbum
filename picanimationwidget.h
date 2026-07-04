#ifndef PICANIMATIONWIDGET_H
#define PICANIMATIONWIDGET_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QTimer>
#include <QPixmap>
#include <QMap>
class PicAnimationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PicAnimationWidget(QWidget *parent = nullptr);

    ~PicAnimationWidget();
    void setPixmap(QTreeWidgetItem* item);
    void start();
    void stop();
    void slideNext();
    void slidePre();
protected:
    virtual void paintEvent(QPaintEvent *event);
private:
    float m_factor;
    QTimer* m_timer;

    //双缓冲图片
    QPixmap m_pixmap1;
    QPixmap m_pixmap2;

    //记录播放到哪张图片
    QTreeWidgetItem* m_curItem;

    //通过Key-Value方式 管理播放的图片,避免重复添加
    QMap<QString, QTreeWidgetItem*> m_mapItems;

    //播放/暂停
    bool m_start;
private:
    void UpdateSelectItemPixmap(QTreeWidgetItem* item);
signals:
    //此信号通知PreListWidget更新预览
    void sigPreListUpdated(QTreeWidgetItem*);

    //更新PreListWidget的选中状态
    void sigPreListSelected(QTreeWidgetItem*);

    //开始播放图片信号
    void sigStart();
    //停止播放图片信号
    void sigStop();

    //是否播放音乐
    void sigStartMusic();
    void sigStopMusic();
private slots:
    void doTimeOut();
public slots:
    void slotUpdateSelectItem(QString path);

    //开始和停止播放图片的槽函数
    void slotStartOrPause();
};

#endif // PICANIMATIONWIDGET_H
