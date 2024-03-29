#include "spriteEditor.h"
#include "QDebug"

SpriteEditor::SpriteEditor(QWidget *parent)
{
    drawing = true;
    erasing = false;
    mousePressed = false;
    lastMousePosition = QPoint(-1, -1);
}

QImage SpriteEditor::generateOnionSkin(int frame) {
    // TODO
}

void SpriteEditor::erasePixel(int x, int y) {
    QImage* frame = &frames[currentFrame];
    frame->setPixel(x, y, qRgba(255, 255, 255, 0));
    displayCurrentFrame();
}

void SpriteEditor::drawPixel(int x, int y, const QColor &color) {
    QImage* frame = &frames[currentFrame];
    frame->setPixelColor(x, y, color);
    displayCurrentFrame();
}

void SpriteEditor::setCurrentColor(const QColor &newColor) {
    currentColor = newColor;
    previewColor = newColor.darker(120);
}

void SpriteEditor::displayCurrentFrame() {
    QImage* frame = &frames[currentFrame];
    QImage scaledFrame = frame->scaled(width * ratio, height * ratio);
    emit displayFrame(&scaledFrame);
}

void SpriteEditor::addFrame(){
    QImage newFrame = QImage(width, height, QImage::Format_ARGB32);
    newFrame.fill(qRgba(255, 255, 255, 0));
    frames.push_back(newFrame);
    currentFrame = frames.length() - 1;
    displayCurrentFrame();
    emit updateFrameBox(currentFrame);
}

void SpriteEditor::deleteFrame(){
    if(currentFrame != 0 && frames.length()-1 != 0){
        frames.removeAt(currentFrame);
        currentFrame -= 1;
        emit deleteModelFrame(currentFrame);
        displayCurrentFrame();
    } else if(currentFrame == 0 && frames.length()-1 > 0){
        frames.removeAt(0);
        emit deleteModelFrame(currentFrame);
        displayCurrentFrame();
    }
}

void SpriteEditor::adjustFrame(int val){
    currentFrame = val;
    displayCurrentFrame();
}

void SpriteEditor::onNewProject(int width, int height, QString name) {
    this->height = height;
    this->width = width;
    this->name = name;
    frames = QVector<QImage>();
    setCurrentColor(qRgba(255, 0, 0, 255));

    ratio = qMin(maxImageSize.x() / width, maxImageSize.y() / height);
    emit updateCanvasSize(width * ratio, height * ratio);

    addFrame();
}

void SpriteEditor::onMouseMoved(int x, int y) {
    if(drawing)
        translateAndDraw(x, y, true);

    if(erasing)
        translateAndDraw(x, y, false);
}

void SpriteEditor::onMousePressed(int x, int y, bool pressed) {
    mousePressed = pressed;
    if(pressed && drawing)
        translateAndDraw(x, y, true);
    else if(pressed && erasing)
        translateAndDraw(x, y, false);
}

void SpriteEditor::currentCanvasPosition(int x, int y) {
    canvasPosition = QPoint(x, y);
}

void SpriteEditor::showCursorPreview(int x, int y) {
    if (lastMousePosition.x() >= 0) // revert color
        drawPixel(lastMousePosition.x(), lastMousePosition.y(), lastPreviewColor);

    lastMousePosition = QPoint(x, y);
    lastPreviewColor = frames[currentFrame].pixelColor(x, y);
    drawPixel(x, y, previewColor);
}

void SpriteEditor::translateAndDraw(int x, int y, bool draw) {
    x -= canvasPosition.x();
    y -= canvasPosition.y();

    if (x < 0 || x >= width * ratio || y < 0 || y >= height * ratio)
        return;

    x = x / ratio;
    y = y / ratio;

    // only draw when mouse is pressed, otherwise, show possible preview instead
    if (!mousePressed) {
        showCursorPreview(x, y);
        return;
    }
    lastMousePosition = QPoint(-1, -1);

    if(draw)
        drawPixel(x, y, currentColor);
    else
        erasePixel(x, y);
}

// toolbar

void SpriteEditor::onDrawPressed(bool pressed){
    if(erasing)
        erasing = false;

    drawing = true;
}

void SpriteEditor::onErasePressed(bool pressed){
    if(drawing)
        drawing = false;

    erasing = true;
}
