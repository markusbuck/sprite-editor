#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "startdialog.h"
#include "spriteEditor.h"
#include "QColorDialog"
#include <QSaveFile>
#include <QFileDialog>
#include <QJsonDocument>

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
    void onDisplayPreview(QImage* frame);
    void onDisplayCurrentFrame(QImage* frame);
    void updateMaxFrames(int max);
    void onUpdateCanvasSize(int x, int y);
    void deleteViewFrame(int index);
    void saveAs(QJsonObject json);
    void onFrameRateSlide(int value);
    void onLoad();

signals:
    void mouseMove(int x, int y);
    void mousePress(int x, int y, bool pressed);
    void setCanvasPosition(int x, int y);
    void loadFile(QString filepath);

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
