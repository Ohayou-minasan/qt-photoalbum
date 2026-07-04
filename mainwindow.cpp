#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QKeySequence>
#include <QDebug>
#include "wizard.h"
#include <QWizardPage>
#include <QStyleFactory>
#include "protree.h"
#include <QFileDialog>
#include "protreewidget.h"
#include "picshow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //创建菜单栏
    this->setMinimumSize(1920, 860);
    QMenu* menu_file = this->menuBar()->addMenu(tr("文件(&F)"));

    //"创建项目"动作
    QAction* act_creat_pro = new QAction(QIcon(":/icon/createpro.png"),tr("创建项目"), this);

    act_creat_pro->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    menu_file->addAction(act_creat_pro);

    //"打开项目" 动作
    QAction* act_open_pro = new QAction(QIcon(":/icon/openpro.png"), tr("打开项目"), this);
    act_open_pro->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    menu_file->addAction(act_open_pro);

    //创建"设置"菜单
    QMenu* menu_set = this->menuBar()->addMenu(tr("设置(&S)"));

    //设置背景音乐
    QAction* act_music = new QAction(QIcon(":/icon/music.png"), tr("背景音乐"), this);
    act_music->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
    menu_set->addAction(act_music);


    //连接信号与槽函数
    connect(act_creat_pro, &QAction::triggered, this, &MainWindow::do_act_creat_pro_triggered);
    connect(act_open_pro, &QAction::triggered, this, &MainWindow::do_act_open_pro_triggered);


    //打开项目操作的信号与槽函数连接
    _protree = new ProTree();
    ui->proLayot->addWidget(_protree, 0);
    QTreeWidget* tree_widget = dynamic_cast<ProTree*>(_protree)->getTreeWidget();
    ProTreeWidget* pro_tree_widget = dynamic_cast<ProTreeWidget*>(tree_widget);
    connect(this, &MainWindow::sigOpenPro, pro_tree_widget, &ProTreeWidget::slotOpenPro);

    _picShow = new PicShow();
    PicShow* proPicShow = dynamic_cast<PicShow*>(_picShow);
    ui->picLayout->addWidget(_picShow, 0);


    //TreeWidget中Item双击操作，PicShow中显示
    connect(pro_tree_widget, &ProTreeWidget::sigUpdateSelected,
            proPicShow, &PicShow::slotSlectedItem);

    //连接PicShow的Next和Pre信号,显示图片
    connect(proPicShow, &PicShow::sigBtnNextClicked,
            pro_tree_widget, &ProTreeWidget::slotNextShow);
    connect(proPicShow, &PicShow::sigBtnPreviousClicked,
            pro_tree_widget, &ProTreeWidget::slotPreShow);


    connect(pro_tree_widget, &ProTreeWidget::sigPicUpdated, proPicShow, &PicShow::slotUpdatePic);

    connect(pro_tree_widget, &ProTreeWidget::sigClearSelected, proPicShow, &PicShow::slotDeleteItem);


    //播放音乐
    connect(act_music, &QAction::triggered, pro_tree_widget, &ProTreeWidget::slotSetMusic);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    PicShow* proPicShow = dynamic_cast<PicShow*>(_picShow);
    proPicShow->reloadPic();
    QMainWindow::resizeEvent(event);
}

void MainWindow::do_act_creat_pro_triggered(bool checked)
{
    qDebug() << "slot act_creat_pro triggered.";
    Wizard wizard(this);
    wizard.setWindowTitle(tr("创建项目"));
    QWizardPage* page1 = wizard.page(0);
    page1->setTitle(tr("设置项目配置"));

    //默认的wizardStyle风格显示不全next和cancel按钮，切换一个显示风格就行了
    wizard.setWizardStyle(QWizard::ModernStyle);

    //连接信号和槽函数,把项目配置传回来todo
    connect(&wizard, &Wizard::SigProSettings,
            dynamic_cast<ProTree*>(_protree), &ProTree::AddProToTree);

    wizard.show();
    wizard.exec();

    //断开所有信号
    disconnect(&wizard);
}

void MainWindow::do_act_open_pro_triggered(bool checked)
{

    // qDebug() << "slot act_open_pro triggered.";
    Q_UNUSED(checked);
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setWindowTitle(tr("选择导入的文件夹"));
    file_dialog.setDirectory(QDir::currentPath());
    file_dialog.setViewMode(QFileDialog::Detail);

    QStringList file_names;
    if(file_dialog.exec()){
        file_names = file_dialog.selectedFiles();
    }

    if(file_names.length() <= 0){
        return;
    }

    QString import_path = file_names.at(0);
    emit sigOpenPro(import_path);
}
