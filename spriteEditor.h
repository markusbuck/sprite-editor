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
    QColor currentColor;

    void displayCurrentFrame();

    void translateAndDraw(int x, int y);

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
    QImage generateOnionSkin(int frame);
    void erasePixel(int x, int y);
    void drawPixel(int x, int y);
    void setCurrentColor(const QColor &newColor);
    void addFrame();
    void deleteFrame();
    void onNewProject(int width, int height, QString name);
    void adjustFrame(int value);

    // mouse

    void onMousePressed(int x, int y, bool pressed);
    void onMouseMoved(int x, int y);

    void currentCanvasPosition(int x, int y);

signals:
    void updateMaxFrames(int max);
    void updateFrameBox(int value);
    void displayFrame(QImage *frame);
};

#endif // SPRITEEDITOR_H
