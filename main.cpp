#include "mainwindow.h"

#include <QApplication>

int Main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SpriteEditor editor;
    MainWindow w(editor);
    w.show();
    return a.exec();
}
