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
	QPen pen;
	bool drawing;
	bool erasing;

	void displayCurrentFrame();

	void translateAndDraw(int x, int y, bool draw);

public:
	explicit SpriteEditor(QWidget *parent = nullptr);

    // actual project info
	int width;
	int height;
	QString name;

    // max scaled values
    const int maxImageX = 440;
    const int maxImageY = 440;

    // the ratio of how much to scale upwards
    float ratio;

    // canvas position
	int canvasX;
	int canvasY;

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

	// toolbar

	void onDrawPressed(bool pressed);
	void onErasePressed(bool pressed);

signals:
	void updateMaxFrames(int max);
	void updateFrameBox(int value);
	void displayFrame(QImage *frame);
   void updateCanvasSize(int x, int y);
   void deleteModelFrame(int frameIndex);

};

#endif // SPRITEEDITOR_H
