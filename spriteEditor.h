#ifndef SPRITEEDITOR_H
#define SPRITEEDITOR_H

#include <QObject>
#include <QImage>
#include <QColor>
#include <QPen>
#include <QVector>
#include <QJsonObject>
#include <QWidget>
#include <QMouseEvent>

class SpriteEditor : public QWidget
{

    Q_OBJECT

private:
    QVector<QImage> frames;
    int currentFrame;
    QPen pen;

    void displayCurrentFrame();
    bool isOnionSkinOn = false;
    QImage* onionSkin;
    QImage generateOnionSkin();

public:
    explicit SpriteEditor(QWidget *parent = nullptr);

    int width;
    int height;
    QString name;

    int canvasX;
    int canvasY;
    int ratioX;
    int ratioY;

public slots:
    void erasePixel(int x, int y);
    void drawPixel(int x, int y);
    void setCurrentColor(int r, int g, int b, int a);
    void addFrame();
    void deleteFrame();
    void onNewProject(int width, int height, QString name);
    void adjustFrame(int value);
    void toggleOnionSkin(int state);

    // mouse

    void onMousePressed(bool pressed);
    void onMouseMoved(int x, int y);

    void currentCanvasPosition(int x, int y);

signals:
    void updateMaxFrames(int max);
    void updateFrameBox(int value);
    void displayFrame(QImage *frame);
};

#endif // SPRITEEDITOR_H
