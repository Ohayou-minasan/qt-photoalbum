#ifndef PROTREEWIDGET_H
#define PROTREEWIDGET_H

#include <QTreeWidget>
#include <QSet>
#include <QAction>
#include <QProgressDialog>
#include <memory>
#include "protreethread.h"
#include "opentreethread.h"
#include <QMediaPlayer>
#include <QUrl>
#include <QVector>
#include <QAudioOutput>

class SlideShowDialog;
class ProTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    ProTreeWidget(QWidget *parent = nullptr);
    void AddProToTree(const QString& name, const QString& path);
private:
    QSet<QString> _set_path;

    //保存当前鼠标右键按下的位置上的item
    QTreeWidgetItem* _right_btn_item;

    //保存当前启动的item
    QTreeWidgetItem* _active_item;

    //记录当前选中的item
    QTreeWidgetItem* _selected_item;
    QAction* _act_import;
    QAction* _act_setstart;
    QAction* _act_closepro;
    QAction* _act_slideshow;

    //create进度条对话框
    QProgressDialog* _dialog_progress;
    //open对话框
    QProgressDialog* _open_progress_dialog;

    //线程创建和打开项目
    std::shared_ptr<ProTreeThread> _thread_create_pro;
    std::shared_ptr<OpenTreeThread> _thread_open_pro;

    //轮播对话框
    std::shared_ptr<SlideShowDialog> m_slideShowDlg;

    //播放音乐
    QMediaPlayer* m_player;
    QVector<QUrl> m_playList;
    QAudioOutput* m_audioOutput;
    //音乐列表的索引
    int m_currentIndex;
private slots:
    //TreeWidget中item相关信号的槽函数
    void SlotItemPressed(QTreeWidgetItem *item, int column);
    void slotDoubleClicked(QTreeWidgetItem *doubleItem, int column);

    void SlotActImport(bool checked = false);
    void slotSetActive(bool checked = false);
    void slotClosePro(bool checked = false);

    //create相关槽函数
    void slotUpdateProgress(int count);
    void slotFinishProgress();
    void slotCancelProgress();

    //open相关槽函数
    void slotUpdateOpenProgress(int count);
    void slotFinishOpenProgress();
    void slotCancelOpenProgress();

    //播放幻灯片
    void slotSlideShow(bool checked = false);
public slots:
    void slotOpenPro(const QString& path);

    //PicShow中Btn的槽函数
    void slotNextShow();
    void slotPreShow();

    //设置音乐
    void slotSetMusic(bool checked = false);

    //播放音乐
    void slotStartMusic();
    //暂停音乐
    void slotStopMusic();
signals:
    void sigCancelProgress();
    void sigCancelOpenProgress();

    //显示图片信号
    void sigUpdateSelected(const QString&);

    //TreeWidget通知PicShow显示图片
    void sigPicUpdated(const QString&);

    //通知清除节点的信号
    void sigClearSelected();
};

#endif // PROTREEWIDGET_H
