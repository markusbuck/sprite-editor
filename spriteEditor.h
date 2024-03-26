#ifndef SPRITEEDITOR_H
#define SPRITEEDITOR_H

#include <QObject>
#include <QImage>
#include <QColor>
#include <QPen>
#include <QVector>
#include <QJsonObject>

class SpriteEditor : public QObject {

    Q_OBJECT

private:

    QVector<QImage> frames;
    int currentFrame;
    QPen pen;

public:
    explicit SpriteEditor(QObject *parent = nullptr);

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
