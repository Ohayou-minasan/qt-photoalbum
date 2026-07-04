#ifndef PICSHOW_H
#define PICSHOW_H

#include <QDialog>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
namespace Ui {
class PicShow;
}

class PicShow : public QDialog
{
    Q_OBJECT

public:
    explicit PicShow(QWidget *parent = nullptr);
    ~PicShow();
    void reloadPic();
protected:
    bool event(QEvent *event) override;
private:
    Ui::PicShow *ui;

    //控制按钮的动画
    QPropertyAnimation* m_animation_show_pre;
    QPropertyAnimation* m_animation_show_next;

    //控制按钮是否可见
    bool mb_btnVisible;
    //是否显示按钮
    void showPreNextBtns(bool show);
    //记录选中Item的需要显示的图片的路径
    QString m_selectedItemPath;

    QPixmap m_pixmap;
public slots:
    void slotSlectedItem(const QString& path);

    //TreeWidget通知更新显示图片
    void slotUpdatePic(const QString& path);


    //删除项目时,如果当前显示的图片属于这个项目,则要删除图片.
    void slotDeleteItem();
signals:
    void sigBtnNextClicked();
    void sigBtnPreviousClicked();
};

#endif // PICSHOW_H
