#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QLatin1StringView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //加载qss
    QFile qss;
    qss.setFileName(":/style/style.qss");
    if(qss.open(QIODevice::ReadOnly)){
        qDebug() << "open qss successful.";
        QString style = QLatin1StringView(qss.readAll());
        a.setStyleSheet(style);
    }
    else{
        qDebug() << "open qss failed.";
        return -1;
    }
    MainWindow w;
    w.setWindowTitle("PhotoAlbum");
    // w.show();
    w.showMaximized();
    return a.exec();
}
