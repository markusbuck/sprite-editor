#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "startdialog.h"
#include "helpdialog.h"
#include "spriteEditor.h"
#include "QColorDialog"
#include <QSaveFile>
#include <QFileDialog>
#include <QJsonDocument>

/*!
 * Authors: Kevin Soto, Zak Holt, Walker Hall, Brenden Suess, and Markus Buckwalter
 * CS 3505 Assignment 8: Sprite Editor Implementation
 * Date: 4/1/24
 *
 * This file contains the components that control the view of the sprite editor.
 */
QT_BEGIN_NAMESPACE
namespace Ui{
    class MainWindow;
}
QT_END_NAMESPACE

/*!
 * \brief The MainWindow class
 */
class MainWindow : public QMainWindow{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    QImage background;
    StartDialog startdialog;
    QColorDialog colorDialog;
    HelpDialog helpDialog;
public:

    /*!
     * \brief MainWindow - Constuctor for the MainWindow.
     * \param editor - Sprite Editor object that represent the model.
     * \param parent - null pointer.
     */
    MainWindow(SpriteEditor& editor, QWidget *parent = nullptr);

    /*!
     * \brief ~MainWindow - Deconstuctor for the MainWindow.
     */
    ~MainWindow();

public slots:
    /*!
     * \brief onDisplayPreview - Used to display a frame in the preview section of the UI.
     * \param frame - the current frame to be displayed.
     */
    void onDisplayPreview(QImage* frame);

    /*!
     * \brief onDisplayCurrentFrame - Used to display a frame in the Editor section of the UI.
     * \param frame - the current frame to be displayed.
     */
    void onDisplayCurrentFrame(QImage* frame);

    /*!
     * \brief updateMaxFrames - updates the number of frames a user can acces in the spin box.
     * \param max - max number of frames.
     */
    void updateMaxFrames(int max);

    /*!
     * \brief onUpdateCanvasSize - sets the size of the Canvas to be used.
     * \param x - width.
     * \param y - height.
     */
    void onUpdateCanvasSize(int x, int y);

    /*!
     * \brief deleteViewFrame - sets the maximum of the frame spin box one less than the current max
     * when the user deletes a frame, and adjusts the spin box to show what frame the user is on.
     * \param index - the frame the user is currently on.
     */
    void deleteViewFrame(int index);

    /*!
     * \brief saveAs - Used to give save functionality by taking in a filename from the user and saving in JSON format.
     * \param json - JSON object to save to.
     */
    void saveAs(QJsonObject json);

    /*!
     * \brief onFrameRateSlide - adjusts the label representing the number of
     * frames per second when the slider is adjusted
     * \param value - number of frames per second.
     */
    void onFrameRateSlide(int value);

    /*!
     * \brief onLoad - Gets a filename of a file to be loaded into the editor.
     */
    void onLoad();

signals:

    /*!
     * \brief mouseMove - sends out the location of the mouse when moving.
     * \param x - x coordinate of mouse.
     * \param y - y coordinate of mouse.
     */
    void mouseMove(int x, int y);

    /*!
     * \brief mousePress - sends out the location of the mouse when pressed.
     * \param x - x coordinate of mouse.
     * \param y - y coordinate of mouse.
     * \param pressed - bool of if the mouse is pressed or not.
     */
    void mousePress(int x, int y, bool pressed);

    /*!
     * \brief setCanvasPosition - sends location of where Canvas is to be set.
     * \param x - x coordinate of Canvas.
     * \param y - y coordinate of Canvas.
     */
    void setCanvasPosition(int x, int y);

    /*!
     * \brief loadFile
     * \param filepath
     */
    void loadFile(QString filepath);

protected:

    /*!
     * \brief mousePressEvent - Used when a mouse is pressed.
     * \param event - event to be used to get coordinates from.
     */
    void mousePressEvent(QMouseEvent *event) override;

    /*!
     * \brief mouseMoveEvent - Used when a mouse is moved.
     * \param event - event to be use to get coordinates from.
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /*!
     * \brief mouseReleaseEvent - Used when a mouse is released.
     * \param event - event to be use to get coordinates from.
     */
    void mouseReleaseEvent(QMouseEvent *event) override;
};
#endif // MAINWINDOW_H
