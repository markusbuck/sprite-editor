#include "mainwindow.h"

#include <QApplication>

/*!
 * \brief main
 * \param argc
 * \param argv
 * \return
 *
 * Main file that executes the spriteEditor.h
 */
int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    SpriteEditor editor;
    MainWindow w(editor);
    w.show();
    return a.exec();
}
