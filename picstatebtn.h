#ifndef PICSTATEBTN_H
#define PICSTATEBTN_H

#include <QPushButton>

class PicStateBtn : public QPushButton
{
    Q_OBJECT
public:
    PicStateBtn(QWidget *parent = nullptr);

    //1设置的是播放的样式，2设置的是停止的样式
    void doSetIcons(const QString& normal, const QString& hover, const QString& pressed,
                    const QString& normal2, const QString& hover2, const QString& pressed2);

    ~PicStateBtn();

protected:
    bool event(QEvent *e) override;
private:
    void setNormalIcon();
    void setHoverIcon();
    void setPressedIcon();

    void setNormal2Icon();
    void setHover2Icon();
    void setPressed2Icon();
private:
    QString m_normal;
    QString m_hover;
    QString m_pressed;
    QString m_normal2;
    QString m_hover2;
    QString m_pressed2;
    int m_curState;

public slots:
    void slotStart();
    void slotStop();
};

#endif // PICSTATEBTN_H
