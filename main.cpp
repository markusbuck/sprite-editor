#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SpriteEditor editor;
    MainWindow w(editor);
    w.show();
    return a.exec();
}
