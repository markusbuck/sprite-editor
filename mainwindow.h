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

    bool isModified() const { return modified; }

public slots:
    void onDisplayCurrentFrame(QImage* frame);
    void updateMaxFrames(int max);

signals:
    void mouseMove(int x, int y);
    void mousePress(int x, int y, bool pressed);
    void setCanvasPosition(int x, int y);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    // void paintEvent(QPaintEvent *event) override;

private:
    Ui::MainWindow *ui;
    QImage background;
    StartDialog startdialog;
    QColorDialog colorDialog;

    // drawing stuff

    // void drawLineTo(const QPoint &endPoint);
    // void resizeImage(QImage *image, const QSize &newSize);

    bool modified = false;
    bool scribbling = false;
    int myPenWidth = 1;
    QColor myPenColor = Qt::blue;
    QImage image;
    QPoint lastPoint;
};
#endif // MAINWINDOW_H
