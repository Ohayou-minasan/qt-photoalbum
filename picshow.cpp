#include "picshow.h"
#include "ui_picshow.h"
#include "picbutton.h"


PicShow::PicShow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PicShow),mb_btnVisible(false)
{
    ui->setupUi(this);
    ui->btn_previous->doSetIcons(":/icon/previous.png", ":/icon/previous_hover.png",
                                 ":/icon/previous_press.png");
    ui->btn_next->doSetIcons(":/icon/next.png", ":/icon/next_hover.png",
                             ":/icon/next_press.png");

    QGraphicsOpacityEffect* opacityPre = new QGraphicsOpacityEffect(this);
    opacityPre->setOpacity(0);
    ui->btn_previous->setGraphicsEffect(opacityPre);

    QGraphicsOpacityEffect* opacityNext = new QGraphicsOpacityEffect(this);
    opacityNext->setOpacity(0);
    ui->btn_next->setGraphicsEffect(opacityNext);

    m_animation_show_next = new QPropertyAnimation(opacityNext, "opacity", this);
    //线性渐变
    m_animation_show_next->setEasingCurve(QEasingCurve::Linear);
    m_animation_show_next->setDuration(500);

    m_animation_show_pre = new QPropertyAnimation(opacityPre, "opacity", this);
    m_animation_show_pre->setEasingCurve(QEasingCurve::Linear);
    m_animation_show_pre->setDuration(500);


    //btn_previouos 和 btn_next的点击信号传送给mainwindow,mainwindow再通过protree传送给widget,接着显示图片
    connect(ui->btn_next, &PicButton::clicked, this, &PicShow::sigBtnNextClicked);
    connect(ui->btn_previous, &PicButton::clicked, this, &PicShow::sigBtnPreviousClicked);



}

PicShow::~PicShow()
{
    delete ui;
}

void PicShow::reloadPic()
{
    if(m_selectedItemPath != ""){
        const int& width = ui->gridLayout->geometry().width();
        const int& height = ui->gridLayout->geometry().height();

        m_pixmap.load(m_selectedItemPath);
        m_pixmap = m_pixmap.scaled(width, height, Qt::KeepAspectRatio);
        ui->label->setPixmap(m_pixmap);
    }
}

bool PicShow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Enter:
        showPreNextBtns(true);
        break;
    case QEvent::Leave:
        showPreNextBtns(false);
    default:
        break;
    }
    return QDialog::event(event);
}

void PicShow::showPreNextBtns(bool show)
{
    //需要隐藏按钮并且按钮可见
    if(!show && mb_btnVisible){
        m_animation_show_pre->stop();
        m_animation_show_pre->setStartValue(1);
        m_animation_show_pre->setEndValue(0);
        m_animation_show_pre->start();

        m_animation_show_next->stop();
        m_animation_show_next->setStartValue(1);
        m_animation_show_next->setEndValue(0);
        m_animation_show_next->start();

        mb_btnVisible = false;
        return;
    }
    //需要展示并且按钮不可见
    if(show && !mb_btnVisible){
        m_animation_show_pre->stop();
        m_animation_show_pre->setStartValue(0);
        m_animation_show_pre->setEndValue(1);
        m_animation_show_pre->start();

        m_animation_show_next->stop();
        m_animation_show_next->setStartValue(0);
        m_animation_show_next->setEndValue(1);
        m_animation_show_next->start();

        mb_btnVisible = true;
        return;
    }
}

void PicShow::slotSlectedItem(const QString &path)
{
    m_selectedItemPath = path;
    m_pixmap.load(path);
    int width = this->width()-20;
    int height = this->height()-20;
    m_pixmap = m_pixmap.scaled(width, height, Qt::KeepAspectRatio);

    ui->label->setPixmap(m_pixmap);
}

void PicShow::slotUpdatePic(const QString &path)
{
    m_selectedItemPath = path;
    if(m_selectedItemPath != ""){
        const int& width = ui->gridLayout->geometry().width();
        const int& height = ui->gridLayout->geometry().height();

        m_pixmap.load(m_selectedItemPath);
        m_pixmap = m_pixmap.scaled(width, height, Qt::KeepAspectRatio);
        ui->label->setPixmap(m_pixmap);
    }
}

void PicShow::slotDeleteItem()
{
    m_selectedItemPath = "";
    ui->label->setPixmap(QPixmap());
}
