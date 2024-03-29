#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "startdialog.h"
#include "spriteEditor.h"
#include "QColorDialog"

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
    void onUpdateCanvasSize(int x, int y);
    void deleteViewFrame(int index);


signals:
    void mouseMove(int x, int y);
    void mousePress(int x, int y, bool pressed);
    void setCanvasPosition(int x, int y);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    Ui::MainWindow *ui;
    QImage background;
    StartDialog startdialog;
    QColorDialog colorDialog;
};
#endif // MAINWINDOW_H
