#include "prosetpage.h"
#include "ui_prosetpage.h"
#include <QLineEdit>
#include <QDir>
#include <QFileDialog>

ProSetPage::ProSetPage(QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::ProSetPage)
{
    ui->setupUi(this);
    registerField("proPath", ui->lineEdit_Path);
    registerField("proName*", ui->lineEdit_Name);

    connect(ui->lineEdit_Name, &QLineEdit::textEdited, this, &ProSetPage::completeChanged);
    connect(ui->lineEdit_Path, &QLineEdit::textEdited, this, &ProSetPage::completeChanged);

    QString curPath = QDir::currentPath();
    ui->lineEdit_Path->setText(curPath);
    ui->lineEdit_Path->setCursorPosition(ui->lineEdit_Path->text().size());
    ui->lineEdit_Path->setClearButtonEnabled(true);

    ui->lineEdit_Name->setClearButtonEnabled(true);

}

ProSetPage::~ProSetPage()
{
    delete ui;
}

void ProSetPage::GetProSettings(QString &name, QString &path)
{
    name = ui->lineEdit_Name->text();
    path = ui->lineEdit_Path->text();
}

bool ProSetPage::isComplete() const
{
    if(ui->lineEdit_Path->text() == "" || ui->lineEdit_Path->text() == ""){
        return false;
    }

    QDir dir(ui->lineEdit_Path->text());
    if(not dir.exists()){
        ui->lab_Tips->setText("project path is not exists");
        return false;

    }

    QString absFilePath = dir.absoluteFilePath(ui->lineEdit_Name->text());
    QDir dist_dir(absFilePath);
    if(dist_dir.exists()){
        ui->lab_Tips->setText("project has exists, change path or name");
        return false;
    }

    ui->lab_Tips->setText("");
    return QWizardPage::isComplete();

}

void ProSetPage::on_btn_Broswer_clicked()
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setWindowTitle("选择导入的文件夹");
    QString path = QDir::currentPath();
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
    qDebug() << "import_path is " << import_path << '\n';
    ui->lineEdit_Path->setText(import_path);

}

