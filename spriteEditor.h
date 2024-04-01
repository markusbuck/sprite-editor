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
#include<QJsonObject>
#include<QJsonArray>
#include <QVariant>
#include <QBuffer>
#include <QTimer>

/*!
 * Authors: Kevin Soto, Zak Holt, Walker Hall, Brenden Suess, and Markus Buckwalter
 * CS 3505 Assignment 8: Sprite Editor Implementation
 * Date: 4/1/24
 *
 * This class contains the implementation of a Sprite Editor that a user can use to draw pixels on a canvas.
 */
class SpriteEditor : public QWidget
{

	Q_OBJECT

private:
	QVector<QImage> frames;
    QTimer *timer;
	int currentFrame;

    int index = 0;

	QColor currentColor;
    QColor previewColor;

    // max scaled values
    const QPoint maxImageSize = QPoint(448, 448);
    const QPoint maxPreviewSize = QPoint(164,164);

    // the ratio of how much to scale upwards
    float previewRatio;
    float ratio;

    // the current canvas position to for mouse translation
    QPoint canvasPosition;

    // for canvas brush preview
    QPoint lastMousePosition;
    QColor lastPreviewColor;

    // toolbar
	bool drawing;
	bool erasing;
    bool mousePressed;

    // Playback Modes
    bool reverse;
    bool forward;
    bool boomerang;
    bool boomerangDirection;
    bool isScaledPreview;

    // helpers

    /*!
     * \brief The displayCurrentFrame method will display the current frame the use is on.
     */
	void displayCurrentFrame();

    /*!
     * \brief Given a coordinate, and passing true if the mouse is in bounds of the frame, the
     * translateAndDraw method will draw a pxiel at the given coordinate, otherwise it will not draw on the frame.
     * \param x The X-Coordinate
     * \param y The Y-Coordinate
     * \param draw Determine to draw on the frame
     * \return True if the pixels were drawn, false otherwise
     */
    bool translateAndDraw(int x, int y, bool draw); // true if mouse was in bounds

    /*!
     * \brief The showCursorPreview method will show a pixel on the cursor to
     * give the user a visual of where a pixel is on the mouse icon.
     * \param x The X-Coordinate
     * \param y The Y-Coordinate
     */
    void showCursorPreview(int x, int y);

    /*!
     * \brief The forwardPlayback helper method will play the animation
     * frames in sequential order by incrementing a counter.
     */
    void forwardPlayback();

    /*!
     * \brief The reversePlayback helper method will play the animation
     * frames in reverse order by decrementing a counter.
     */
    void reversePlayback();

    /*!
     * \brief The boomerangPlayback helper method will play the animation frames
     * sequentially, then going in reverse order once it reaches the last frame.
     */
    void boomerangPlayback();

    // onion skin
    bool isOnionSkinOn = false;
    QImage* onionSkin;

    /*!
     * \brief The generateOnionSkin method will create a copy of the previous frames
     * QImage, but changing the alpha of the pixels to 125.
     * \return A QImage of the previous frame but the pixels are more transparent.
     */
    QImage generateOnionSkin();

public:
	explicit SpriteEditor(QWidget *parent = nullptr);

    // actual project info
	int width;
	int height;
	QString name;
public slots:
    /*!
     * \brief The erasePixel slot will erase a pixel on the given coordinate.
     * \param x The X-Coordinate
     * \param y The Y-Coordinate
     */
	void erasePixel(int x, int y);

    /*!
     * \brief The drawPixel slot will draw a pixel on the given coordinate, and color.
     * \param x The X-Coordinate
     * \param y The Y-Coordinate
     * \param color The color to draw a pixel
     */
    void drawPixel(int x, int y, const QColor &color);

    /*!
     * \brief The setCurrentColor slot will change the color to the new given color.
     * \param newColor The new color to be added
     */
	void setCurrentColor(const QColor &newColor);

    /*!
     * \brief The addFrame slot will append a QImage to a vector of QImages.
     */
	void addFrame();

    /*!
     * \brief The deleteFrame slot will remove the last QImage in a vector of QImages.
     */
	void deleteFrame();

    /*!
     * \brief The onNewProject slot will reset the vector of QImages, and
     * will create a new frame with the given width and height.
     * The name of the project will be changed to the provided string.
     * \param width The Width of a frame
     * \param height The Height of a frame
     * \param name The name of the project for saving
     */
	void onNewProject(int width, int height, QString name);

    /*!
     * \brief The adjustFrame slot will change the currentFrame
     * value to the number provided, then it will display the frame.
     * \param value The number to determine which frame to show.
     */
    void adjustFrame(int value);

    /*!
     * \brief The toggleOnionSkin slot will determine when to display the onion skin on a frame by the number provided.
     * If the number is 0, the onion skin will not show.
     * If the number is 2, the onion skin will show.
     * \param state The number to determine when to show the onion skin on a frame
     */
    void toggleOnionSkin(int state);

    /*!
     * \brief The framePreview slot will determine which preview to will be shown.
     */
    void framePreview();

    /*!
     * \brief The adjustFPS slot will change the FPS to the provided number.
     */
    void adjustFPS(int);

    /*!
     * \brief The toggleForward slot will set the forward, and boomerangDirection bool's to true.
     */
    void toggleForward();

    /*!
     * \brief The toggleReverse slot will set the reverse bool to true.
     */
    void toggleReverse();

    /*!
     * \brief The toggleBoomerang slot will set the boomerang bool to true.
     */
    void toggleBoomerang();

    /*!
     * \brief The toggleActualSize slot will determine when to display the actual size of the frames by the given state.
     * If the given number is 0, isScaledPreview will be set to true
     * If the given number is 2, isScaledPreview will be set to false
     * \param state The number to determine what to set isScaledPreview to
     */
    void toggleActualSize(int state);

    // save/load

    /*!
     * \brief The toJson slot will turn the SpriteEditor object into a JSON file.
     */
    void toJson();

    /*!
     * \brief The toQImage slot will open the file, get the information, and get the width, height, and project name.
     * It will also get the frames from the file.
     * \param filePath The path of the file to load
     */
    void toQImage(QString filePath);

	// mouse

    /*!
     * \brief The onMousePressed slot will draw a pixel to the frame on the given
     * coordnate if the mouse is pressed, otherwise a pixel will not be drawn.
     * \param x The X-Coordinate
     * \param y The Y-Coordinate
     * \param pressed A bool to determine if the mouse button is being pressed
     */
	void onMousePressed(int x, int y, bool pressed);

    /*!
     * \brief The onMouseMoved slot will call TranslateAndDraw and pass the coordinate
     * to tranlate the coordinate, and draw the pixel.
     * \param x The X-Coordinate
     * \param y The Y-Coordinate
     */
	void onMouseMoved(int x, int y);

    /*!
     * \brief The currentCanvasPosition slot will create a QPoint of
     * the passed in coordinate, and will set the QPoint to the canvasPosition.
     * \param x The X-Coordinate
     * \param y The Y-Coordinate
     */
	void currentCanvasPosition(int x, int y);

	// toolbar

    /*!
     * \brief The onDrawPressed slot will change the drawing bool to true, and erasing to false.
     * \param pressed The bool to determine if the OnDraw button has been pressed
     */
	void onDrawPressed(bool pressed);

    /*!
     * \brief The onErasePressed slot will change the drawing bool to false, and erasing to true.
     * \param pressed The bool to determine if the OnErase button has been pressed
     */
	void onErasePressed(bool pressed);

signals:
    /*!
     * \brief The updateMaxFrames signal is used to get the max number of frames.
     * \param max The max amount of frames
     */
	void updateMaxFrames(int max);

    /*!
     * \brief The updateFrameBox signal is used to change the frame box's number to the provided value.
     * \param value The value to change the frame box's number
     */
	void updateFrameBox(int value);

    /*!
     * \brief The displayFrame signal is used to display the provided frame on the screen.
     * \param frame The frame to display on the screen
     */
	void displayFrame(QImage *frame);

    /*!
     * \brief The displayPreview signal is used to display the provided frame to the preview section.
     * \param frame The frame to display on the preview
     */
    void displayPreview(QImage *frame);

    /*!
     * \brief The updateCanvasSize isgnal is used to update the size of the canvas.
     * \param x The new width of the canvas
     * \param y The new height of the canvas
     */
    void updateCanvasSize(int x, int y);

    /*!
     * \brief The deleteModelFrame signal is used to delete a frame by the provided index.
     * \param frameIndex The index of the freame to delete
     */
    void deleteModelFrame(int frameIndex);

    /*!
     * \brief The jsonObject signal is used to convert the SpriteEditor object into a JSON to be saved as a file.
     * \param json The JSON containing the information to save as a file
     */
    void jsonObject(QJsonObject json);

    /*!
     * \brief The displayLoadedFrames signal is used to display the frames from a file.
     * \param newFrames A vector of frames
     */
    void displayLoadedFrames(QVector<QImage> newFrames);

};

#endif // SPRITEEDITOR_H
