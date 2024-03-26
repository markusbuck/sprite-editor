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

    void displayCurrentFrame();

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
    void onNewProject(int width, int height, QString name);
signals:

    void displayFrame(QImage* frame);

};

#endif // SPRITEEDITOR_H
