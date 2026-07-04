#ifndef PICBUTTON_H
#define PICBUTTON_H

#include <QPushButton>
#include <QPixmap>
#include <QEvent>
class PicButton : public QPushButton
{
    Q_OBJECT
public:
    PicButton(QWidget *parent = nullptr);
    void doSetIcons(const QString& normal, const QString& hover, const QString& pressed);

private:
    //图片保存的路径
     QString m_normal;

     QString m_hover;
     QString m_pressed;
 private:
     void setNormalIcon();
     void setHoverIcon();
     void setPressedIcon();
 protected:
     bool event(QEvent *e) override;

};

#endif // PICBUTTON_H
