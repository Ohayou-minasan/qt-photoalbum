#include "picstatebtn.h"
#include "const.h"
#include <QPixmap>
#include <QEvent>
PicStateBtn::PicStateBtn(QWidget *parent)
    :QPushButton(parent)
{

}

void PicStateBtn::doSetIcons(const QString &normal, const QString &hover, const QString &pressed, const QString &normal2, const QString &hover2, const QString &pressed2)
{
    m_normal = normal;
    m_hover = hover;
    m_pressed = pressed;

    m_normal2 = normal2;
    m_hover2 = hover2;
    m_pressed2 = pressed2;

    QPixmap tmpPixmap;
    tmpPixmap.load(normal);
    this->resize(tmpPixmap.size());
    this->setIcon(tmpPixmap);
    this->setIconSize(tmpPixmap.size());

    m_curState = PicBtnStateNormal;
}

PicStateBtn::~PicStateBtn()
{

}

bool PicStateBtn::event(QEvent *e)
{
    switch (e->type()) {
    case QEvent::Enter:
        if(m_curState < PicBtnState2Normal){
            this->setHoverIcon();
        }
        else{
            this->setHover2Icon();
        }
        break;
    case QEvent::Leave:
        if(m_curState < PicBtnState2Normal){
            this->setNormalIcon();
        }
        else{
            this->setNormal2Icon();
        }
        break;
    case QEvent::MouseButtonPress:
        if(m_curState < PicBtnState2Normal){
            this->setPressedIcon();
        }
        else{
            this->setPressed2Icon();
        }
        break;
    case QEvent::MouseButtonRelease:
        if(m_curState < PicBtnState2Normal){
            this->setHover2Icon();
        }
        else{
            this->setHoverIcon();
        }
        break;
    default:
        break;
    }
    return QPushButton::event(e);
}

void PicStateBtn::setNormalIcon()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(m_normal);
    this->setIcon(tmpPixmap);

    m_curState = PicBtnStateNormal;
}

void PicStateBtn::setHoverIcon()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(m_hover);
    this->setIcon(tmpPixmap);

    m_curState = PicBtnStateHover;
}

void PicStateBtn::setPressedIcon()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(m_pressed);
    this->setIcon(tmpPixmap);

    m_curState = PicBtnStatePressed;
}

void PicStateBtn::setNormal2Icon()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(m_normal2);
    this->setIcon(tmpPixmap);

    m_curState = PicBtnState2Normal;
}

void PicStateBtn::setHover2Icon()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(m_hover2);
    this->setIcon(tmpPixmap);

    m_curState = PicBtnState2Hover;
}

void PicStateBtn::setPressed2Icon()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(m_pressed2);
    this->setIcon(tmpPixmap);

    m_curState = PicBtnState2Pressed;
}

void PicStateBtn::slotStart()
{
    this->setNormal2Icon();
}

void PicStateBtn::slotStop()
{
this->setNormalIcon();
}
