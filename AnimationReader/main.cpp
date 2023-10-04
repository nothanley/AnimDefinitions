#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
//    QApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
//    if (qgetenv("QT_FONT_DPI").isEmpty()) {qputenv("QT_FONT_DPI", "96");}

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QStringList args = QCoreApplication::arguments();
    for (auto &arg : args){
        if (arg.endsWith(".adefs") ){
            w.OpenFile(arg);
        }
    }

    return a.exec();
}
