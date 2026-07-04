#include "picbutton.h"

PicButton::PicButton(QWidget *parent)
{

}

void PicButton::doSetIcons(const QString &normal, const QString &hover, const QString &pressed)
{
    m_normal = normal;
    m_hover = hover;
    m_pressed = pressed;

    QPixmap tmpPixmap;
    tmpPixmap.load(normal);
    this->resize(tmpPixmap.size());
    this->setIcon(tmpPixmap);
    this->setIconSize(tmpPixmap.size());

}

void PicButton::setNormalIcon()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(m_normal);
    this->setIcon(tmpPixmap);
}

void PicButton::setHoverIcon()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(m_hover);
    this->setIcon(tmpPixmap);
}

void PicButton::setPressedIcon()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(m_pressed);
    this->setIcon(tmpPixmap);
}

bool PicButton::event(QEvent *e)
{
    switch(e->type()){
    case QEvent::Enter:
        setHoverIcon();
        break;
    case QEvent::Leave:
        setNormalIcon();
        break;
    case QEvent::MouseButtonPress:
        setPressedIcon();
        break;
    case QEvent::MouseButtonRelease:
        setHoverIcon();
        break;
    default:
        break;
    }

    return QPushButton::event(e);
}
