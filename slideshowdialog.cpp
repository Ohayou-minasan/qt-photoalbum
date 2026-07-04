#include "slideshowdialog.h"
#include "ui_slideshowdialog.h"
#include "prelistwidget.h"
#include "protreewidget.h"
SlideShowDialog::SlideShowDialog(QWidget *parent, QTreeWidgetItem *firstItem,
                                 QTreeWidgetItem *lastItem)
    :QDialog(parent), m_firstItem(firstItem),m_lastItem(lastItem),ui(new Ui::SlideShowDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    ui->btn_slidePre->doSetIcons(":/icon/previous.png",
                                 ":/icon/previous_hover.png",
                                 ":/icon/previous_press.png");

    ui->btn_slideNext->doSetIcons(":/icon/next.png",
                                 ":/icon/next_hover.png",
                                 ":/icon/next_press.png");

    ui->btn_close->doSetIcons(":/icon/closeshow.png",
                              ":/icon/closeshow_hover.png",
                              ":/icon/closeshow_press.png");

    ui->btn_play->doSetIcons(":/icon/play.png",":/icon/play_hover.png", ":/icon/play_press.png",
                             ":/icon/pause.png", ":/icon/pause_hover.png", ":/icon/pause_press.png");

    //关闭按钮
    connect(ui->btn_close, &PicButton::clicked, this, &SlideShowDialog::close);


    //向后查看按钮
    connect(ui->btn_slideNext, &PicButton::clicked, this, &SlideShowDialog::do_btn_slideNext_clicked);

    //向前查看按钮
    connect(ui->btn_slidePre, &PicButton::clicked, this, &SlideShowDialog::do_btn_slidePre_clicked);
    //通过Dialog连接PreListWidget和PicAnimation
    PreListWidget* preListWidget = dynamic_cast<PreListWidget*>(ui->preListWidget);
    connect(ui->picAnimation, &PicAnimationWidget::sigPreListUpdated,
            preListWidget, &PreListWidget::slotUpdatePreList);

    connect(ui->picAnimation, &PicAnimationWidget::sigPreListSelected,
            preListWidget, &PreListWidget::slotSelectPreList);

    //播放/暂停
    connect(ui->btn_play, &PicStateBtn::clicked,
            ui->picAnimation, &PicAnimationWidget::slotStartOrPause);

    //初次启动时,更新播放/暂停按钮
    connect(ui->picAnimation, &PicAnimationWidget::sigStart, ui->btn_play, &PicStateBtn::slotStart);
    connect(ui->picAnimation, &PicAnimationWidget::sigStop, ui->btn_play, &PicStateBtn::slotStop);

    //点击预览区实现图片切换
    connect(ui->preListWidget, &PreListWidget::sigSelectItemUpdated,
            ui->picAnimation, &PicAnimationWidget::slotUpdateSelectItem);


    //动画界面决定是否播放音乐
    ProTreeWidget* proTreeWidget = dynamic_cast<ProTreeWidget*>(parent);

    connect(ui->picAnimation, &PicAnimationWidget::sigStartMusic,
            proTreeWidget, &ProTreeWidget::slotStartMusic);

    connect(ui->picAnimation, &PicAnimationWidget::sigStopMusic,
            proTreeWidget, &ProTreeWidget::slotStopMusic);

    ui->picAnimation->setPixmap(m_firstItem);
    ui->picAnimation->start();
}

SlideShowDialog::~SlideShowDialog()
{
    delete ui;
}

void SlideShowDialog::do_btn_slideNext_clicked()
{
    ui->picAnimation->slideNext();
}

void SlideShowDialog::do_btn_slidePre_clicked()
{
    ui->picAnimation->slidePre();
}
