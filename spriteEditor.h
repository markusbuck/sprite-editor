#ifndef SPRITEEDITOR_H
#define SPRITEEDITOR_H

#include <QImage>
#include <QColor>
#include <QPen>
#include <QVector>
#include <QJsonObject>

class SpriteEditor{

    Q_OBJECT

private:

    QVector<QImage> frames;
    int currentFrame;
    QPen pen;

public:
    int width;
    int height;
    QString name;

public slots:

    QImage generateOnionSkin(int frame);
    void erasePixel(int x, int y);
    void drawPixel(int x, int y);
    void setCurrentColor(int r, int g, int b, int a);
    void addFrame();
    void deleteFrame();
    void onNewProject(int height, int width, QString name);

signals:

    void updateFrame(QImage* frame);

};

#endif // SPRITEEDITOR_H
