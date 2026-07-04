#ifndef SLIDESHOWDIALOG_H
#define SLIDESHOWDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui {
class SlideShowDialog;
}

class SlideShowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SlideShowDialog(QWidget *parent = nullptr,
                             QTreeWidgetItem* firstItem = nullptr,
                             QTreeWidgetItem* lastItem = nullptr);
    ~SlideShowDialog();

private:
    Ui::SlideShowDialog *ui;
    QTreeWidgetItem* m_firstItem;
    QTreeWidgetItem* m_lastItem;

public slots:
    void do_btn_slideNext_clicked();
    void do_btn_slidePre_clicked();
};

#endif // SLIDESHOWDIALOG_H
