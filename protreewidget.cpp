#include "protreewidget.h"
#include <QDir>
#include "protreeitem.h"
#include "const.h"
#include <QHeaderView>
#include <QApplication>
#include <QMenu>
#include <QFileDialog>
#include <QProgressDialog>
#include <QThread>
#include "removeprodialog.h"
#include "slideshowdialog.h"
ProTreeWidget::ProTreeWidget(QWidget *parent)
    :QTreeWidget(parent), _active_item(nullptr),_right_btn_item(nullptr),
    _dialog_progress(nullptr), _selected_item(nullptr),
    _thread_create_pro(nullptr),_thread_open_pro(nullptr),
    _open_progress_dialog(nullptr),
    m_player(new QMediaPlayer(this)), m_audioOutput(new QAudioOutput(this)),
    m_currentIndex(-1)
{
    this->header()->hide();
    //初始化媒体播放器
    m_player->setAudioOutput(m_audioOutput);
    //单曲循环
    m_player->setLoops(QMediaPlayer::Infinite);
    m_audioOutput->setVolume(0.5);
    // this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &ProTreeWidget::itemPressed, this, &ProTreeWidget::SlotItemPressed);
    _act_import = new QAction(QIcon(":/iocn/import.png"), tr("导入文件"), this);
    _act_setstart = new QAction(QIcon(":/icon/core.png"), tr("设置活动项目"), this);
    _act_closepro = new QAction(QIcon(":/icon/close.png"), tr("关闭项目"), this);
    _act_slideshow = new QAction(QIcon(":/icon/slideshow.png"), tr("轮播图片"), this);

    //导入功能
    connect(_act_import, &QAction::triggered, this, &ProTreeWidget::SlotActImport);

    //设置活动项目
    connect(_act_setstart, &QAction::triggered, this, &ProTreeWidget::slotSetActive);

    //关闭项目
    connect(_act_closepro, &QAction::triggered, this, &ProTreeWidget::slotClosePro);

    //幻灯片播放
    connect(_act_slideshow, &QAction::triggered, this, &ProTreeWidget::slotSlideShow);

    //实现点击item展示图片
    connect(this, &ProTreeWidget::itemDoubleClicked, this, &ProTreeWidget::slotDoubleClicked);

}

void ProTreeWidget::AddProToTree(const QString &name, const QString &path)
{
    QDir dir(path);
    QString file_path = dir.absoluteFilePath(name);

    if(_set_path.find(file_path) != _set_path.end()){
        return;
    }

    QDir pro_dir(file_path);
    if(not pro_dir.exists()){
        bool enable = pro_dir.mkpath(file_path);
        if(not enable){
            qDebug() << "pro_dir make path failed.\n";
            return;
        }
    }

    _set_path.insert(file_path);
    ProTreeItem* item = new ProTreeItem(this, name, file_path, TreeItemPro);
    item->setData(0, Qt::DisplayRole, name);
    item->setData(0, Qt::DecorationRole, QIcon(":/icon/dir.png"));
    item->setData(0, Qt::ToolTipRole, file_path);
    this->addTopLevelItem(item);
}

void ProTreeWidget::SlotItemPressed(QTreeWidgetItem *item, int column)
{
    // qDebug() << "enter rightButton menu.";
    if(QApplication::mouseButtons() == Qt::RightButton){
        // qDebug() << "enter first if.";
        QMenu menu(this);
        int itemType = item->type();
        if(itemType == TreeItemPro){
            // qDebug() << "enter second if.";

            //缓存当前item
            _right_btn_item = item;

            menu.addAction(_act_import);
            menu.addAction(_act_setstart);
            menu.addAction(_act_closepro);
            menu.addAction(_act_slideshow);
            menu.exec(QCursor::pos());
        }

    }
}

void ProTreeWidget::slotDoubleClicked(QTreeWidgetItem *doubleItem, int column)
{
    if(QGuiApplication::mouseButtons() == Qt::LeftButton){
        ProTreeItem* treeDoubleItem = dynamic_cast<ProTreeItem*>(doubleItem);
        if(!treeDoubleItem){
            return;
        }

        int itemType = treeDoubleItem->type();
        if(itemType == TreeItemPic){
            emit sigUpdateSelected(treeDoubleItem->GetPath());
            _selected_item = doubleItem;
        }
    }
}

void ProTreeWidget::SlotActImport(bool checked)
{
    Q_UNUSED(checked);
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setWindowTitle(tr("选择导入的文件夹"));
    QString path = "";
    if(not _right_btn_item){
        qDebug() << "_right_btn_item is empty.\n";
        return;
    }

    path = dynamic_cast<ProTreeItem*>(_right_btn_item)->GetPath();

    file_dialog.setDirectory(path);

    file_dialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if(file_dialog.exec()){
        fileNames = file_dialog.selectedFiles();
    }
    if(fileNames.length() <= 0){
        return;
    }

    QString import_path = fileNames.at(0);

    int file_count = 0;

    _dialog_progress = new QProgressDialog(this);

    _thread_create_pro = std::make_shared<ProTreeThread>(std::ref(import_path),
                                                         std::ref(path),_right_btn_item,
                                                         file_count, this, _right_btn_item, nullptr);

    qDebug() << "run subthread....";

    //文件数量更改,更新进度条
    connect(_thread_create_pro.get(), &ProTreeThread::sigProgressUpdated,
            this, &ProTreeWidget::slotUpdateProgress);

    connect(_thread_create_pro.get(), &ProTreeThread::sigProgressFinished,
            this, &ProTreeWidget::slotFinishProgress);

    connect(_dialog_progress, &QProgressDialog::canceled, this, &ProTreeWidget::slotCancelProgress);

    connect(this, &ProTreeWidget::sigCancelProgress,
            _thread_create_pro.get(), &ProTreeThread::slotCancelProgress);

    _thread_create_pro->start();

    _dialog_progress->setWindowTitle(tr("Please wait...."));
    _dialog_progress->setFixedWidth(PROGRESS_WIDTH);
    _dialog_progress->setRange(0, PROGRESS_WIDTH);
    _dialog_progress->exec();

}

void ProTreeWidget::slotSetActive(bool checked)
{
    if(!_right_btn_item){
        return;
    }

    QFont nullFont;
    nullFont.setBold(false);


    if(_active_item){
        _active_item->setFont(0, nullFont);
    }

    _active_item = _right_btn_item;
    nullFont.setBold(true);

    _active_item->setFont(0, nullFont);

}

void ProTreeWidget::slotClosePro(bool checked)
{
    RemoveProDialog remove_pro_dialog;
    int ret_val = remove_pro_dialog.exec();
    if(ret_val != QDialog::Accepted){
        return;
    }
    bool b_remove = remove_pro_dialog.isRemoved();
    int index_right_btn = this->indexOfTopLevelItem(_right_btn_item);
    ProTreeItem* pro_tree_item = dynamic_cast<ProTreeItem*>(_right_btn_item);
    ProTreeItem* selected_item = dynamic_cast<ProTreeItem*>(_selected_item);

    QString delete_path = pro_tree_item->GetPath();
    _set_path.remove(delete_path);
    if(b_remove){
        QDir delete_dir(delete_path);
        delete_dir.removeRecursively();
    }

    if(pro_tree_item == _active_item){
        _active_item = nullptr;
    }


    if(selected_item && pro_tree_item == selected_item->GetRoot()){
        selected_item = nullptr;
        _selected_item = nullptr;
        emit sigClearSelected();
    }
    delete this->takeTopLevelItem(index_right_btn);
    _right_btn_item = nullptr;
}

void ProTreeWidget::slotUpdateProgress(int count)
{
    if(!_dialog_progress){
        return;
    }

    if(count >= PROGRESS_MAX){
        _dialog_progress->setValue(count%PROGRESS_MAX);
    }
    else{
        _dialog_progress->setValue(count);
    }
}

void ProTreeWidget::slotFinishProgress()
{
    _dialog_progress->setValue(PROGRESS_MAX);
    _dialog_progress->deleteLater();
}

void ProTreeWidget::slotCancelProgress()
{
    emit sigCancelProgress();
    delete _dialog_progress;
    _dialog_progress = nullptr;
}

void ProTreeWidget::slotUpdateOpenProgress(int count)
{
    if(!_open_progress_dialog){
        return;
    }

    if(count >= PROGRESS_MAX){
        _open_progress_dialog->setValue(count%PROGRESS_MAX);
    }
    else{
        _open_progress_dialog->setValue(count);
    }
}

void ProTreeWidget::slotFinishOpenProgress()
{
    if(!_open_progress_dialog){
        return;
    }
    _open_progress_dialog->setValue(PROGRESS_MAX);
    delete _open_progress_dialog;
    _open_progress_dialog = nullptr;
}

void ProTreeWidget::slotCancelOpenProgress()
{
    emit sigCancelOpenProgress();
    delete _open_progress_dialog;
    _open_progress_dialog = nullptr;
}

void ProTreeWidget::slotSlideShow(bool checked)
{
    if(!_right_btn_item){
        return;
    }

    ProTreeItem* rightProItem = dynamic_cast<ProTreeItem*>(_right_btn_item);
    ProTreeItem* lastChildItem = rightProItem->GetLastPicChild();
    ProTreeItem* firstChildItem = rightProItem->GetFirstPicChild();

    if(!lastChildItem || !firstChildItem){
        return;
    }

    qDebug() << "first child item is" << firstChildItem->GetPath();
    qDebug() << "last child item is " << lastChildItem->GetPath();
    m_slideShowDlg = std::make_shared<SlideShowDialog>(this, firstChildItem, lastChildItem);

    m_slideShowDlg->setModal(true);
    m_slideShowDlg->showMaximized();
}

void ProTreeWidget::slotOpenPro(const QString &path)
{
    if(_set_path.find(path) != _set_path.end()){
        qDebug() << "file has loaded";
        return;
    }

    _set_path.insert(path);
    int file_count = 0;

    QDir pro_dir(path);
    QString pro_name = pro_dir.dirName();

    _thread_open_pro = std::make_shared<OpenTreeThread>(path, file_count, this, nullptr);

    _open_progress_dialog = new QProgressDialog(this);
    connect(_thread_open_pro.get(), &OpenTreeThread::sigProgressUpdated,
            this, &ProTreeWidget::slotUpdateOpenProgress);

    connect(_thread_open_pro.get(), &OpenTreeThread::sigProgressFinished,
            this, &ProTreeWidget::slotFinishOpenProgress);

    connect(_open_progress_dialog, &QProgressDialog::canceled, this, &ProTreeWidget::slotCancelOpenProgress);

    connect(this, &ProTreeWidget::sigCancelOpenProgress,
            _thread_open_pro.get(), &OpenTreeThread::slotCancelProgress);

    _thread_open_pro->start();

    _open_progress_dialog->setWindowTitle(tr("Please wait...."));
    _open_progress_dialog->setFixedWidth(PROGRESS_WIDTH);
    _open_progress_dialog->setRange(0, PROGRESS_WIDTH);
    _open_progress_dialog->exec();
}

void ProTreeWidget::slotNextShow()
{
    if(!_selected_item){
        return;
    }
    ProTreeItem* curItem = dynamic_cast<ProTreeItem*>(_selected_item)->GetNextItem();
    if(!curItem){
        return;
    }

    emit sigPicUpdated(curItem->GetPath());
    _selected_item = curItem;
    this->setCurrentItem(curItem);
}

void ProTreeWidget::slotPreShow()
{
    if(!_selected_item){
        return;
    }
    ProTreeItem* curItem = dynamic_cast<ProTreeItem*>(_selected_item)->GetPreItem();
    if(!curItem){
        return;
    }

    emit sigPicUpdated(curItem->GetPath());
    _selected_item = curItem;
    this->setCurrentItem(curItem);
}

void ProTreeWidget::slotSetMusic(bool checked)
{
    Q_UNUSED(checked);
    m_playList.clear();
    QFileDialog fileDlg;
    fileDlg.setFileMode(QFileDialog::ExistingFiles);
    fileDlg.setWindowTitle(tr("选择音频文件"));
    fileDlg.setDirectory(QDir::currentPath());
    fileDlg.setViewMode(QFileDialog::Detail);
    QStringList filters;
    filters << "MP3 Files (*.mp3)"
            << "All Files (*.*)";
    fileDlg.setNameFilters(filters);
    QStringList fileNames;

    if(fileDlg.exec()){
        fileNames = fileDlg.selectedFiles();
    }
    else{
        return;
    }

    if(fileNames.length() <= 0){
        return;
    }
    for(const auto& fileName : fileNames){
        m_playList.push_back(QUrl::fromLocalFile(fileName));
    }
    if(m_player->playbackState() != QMediaPlayer::PlayingState){
        m_currentIndex = 0;
        m_player->setSource(m_playList.at(m_currentIndex));
    }
}

void ProTreeWidget::slotStartMusic()
{
    m_player->play();
}

void ProTreeWidget::slotStopMusic()
{
    m_player->pause();
}
