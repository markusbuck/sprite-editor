#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "startdialog.h"
#include "spriteEditor.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(SpriteEditor& editor, QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onDisplayCurrentFrame(QImage* frame);
    void updateMaxFrames(int max);

private:
    Ui::MainWindow *ui;
    QImage background;
    StartDialog startdialog;
};
#endif // MAINWINDOW_H
