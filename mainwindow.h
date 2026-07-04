#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    virtual void resizeEvent(QResizeEvent *event);
private:
    Ui::MainWindow *ui;
    QWidget* _protree;
    QWidget* _picShow;

private slots:
    void do_act_creat_pro_triggered(bool checked = false);
    void do_act_open_pro_triggered(bool checked = false);

signals:
    void sigOpenPro(const QString& path);
};
#endif // MAINWINDOW_H
