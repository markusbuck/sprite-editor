#include "spriteEditor.h"
#include "QDebug"
#include <QPainter>

SpriteEditor::SpriteEditor(QWidget *parent)
{
    drawing = true;
    erasing = false;
    mousePressed = false;
    lastMousePosition = QPoint(-1, -1);
}

void SpriteEditor::toggleOnionSkin(int state) {
    if(state == 0) {
        this->isOnionSkinOn = false;
        displayCurrentFrame();
    }

    else if(state == 2) {
        this->isOnionSkinOn = true;
        displayCurrentFrame();
    }
}

QImage SpriteEditor::generateOnionSkin(){

    if(this->currentFrame - 1 < 0) {
        QImage currentFrameCopy = this->frames.at(0).copy();
        return currentFrameCopy;
    }

    QImage previousFrame = this->frames.at(this->currentFrame - 1);
    QImage copyPreviousFrame = this->frames.at(this->currentFrame - 1).copy();

    for(int y = 0; y < this->height; y++){
        for(int x = 0; x < this->width; x++){

            QColor color  = previousFrame.pixelColor(x, y);

            if(color.alpha() == 0){
                continue;
            }

            QColor onionCol(color.red(), color.blue(), color.blue(), 125);
            copyPreviousFrame.setPixelColor(x, y, onionCol);
        }
    }

    return copyPreviousFrame;
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

void SpriteEditor::displayCurrentFrame(){

    const QImage *curFrame = &frames.at(currentFrame);

    // alpha channel
    QImage alphaPattern = QImage(curFrame->size(), QImage::Format_ARGB32);
    int patternRatio = (int)qMax(ratio / 16, 4.0);

    for (int y = 0; y < alphaPattern.height(); y++) {
        for (int x = 0; x < alphaPattern.width(); x++) {
            QRgb color = ((x / patternRatio) % 2 == (y / patternRatio) % 2) ? qRgb(192, 192, 192) : qRgb(220, 220, 220);
            alphaPattern.setPixel(x, y, color);
        }
    }

    for (int y = 0; y < curFrame->height(); ++y) {
        for (int x = 0; x < curFrame->width(); ++x) {
            QRgb pixel = curFrame->pixel(x, y);
            if (qAlpha(pixel) > 0)
                alphaPattern.setPixel(x, y, pixel);
        }
    }

    //

    const QImage *frame = &alphaPattern;

    if(this->isOnionSkinOn){

        QImage onionFrame = this->generateOnionSkin();
        QImage copyCurrentFrame = frame->copy();
        QImage mergedFrame = QImage(width, height, QImage::Format_ARGB32);
        mergedFrame.fill(qRgba(0, 0, 0, 0));

        for(int y = 0; y < this->height; y++){
            for(int x = 0; x < this->width; x++){

                QColor onionColor  = onionFrame.pixelColor(x, y);

                QColor currentFrameColor = copyCurrentFrame.pixelColor(x, y);

                if(onionColor.alpha() == 0 && currentFrameColor.alpha() == 0){
                    continue;
                }

                else if(onionColor.alpha() == 0) {

                    mergedFrame.setPixelColor(x, y, QColor(currentFrameColor.red(), currentFrameColor.green(), currentFrameColor.blue(), currentFrameColor.alpha()));
                }

                else if (currentFrameColor.alpha() == 0) {
                    mergedFrame.setPixelColor(x, y, QColor(onionColor.red(), onionColor.green(), onionColor.blue(), onionColor.alpha()));
                }

                else {

                    mergedFrame.setPixelColor(x, y, QColor(onionColor.red(), onionColor.green(), onionColor.blue(), onionColor.alpha()));
                    mergedFrame.setPixelColor(x, y, QColor(currentFrameColor.red(), currentFrameColor.green(), currentFrameColor.blue(), currentFrameColor.alpha()));
                }
            }
        }


        this->onionSkin = &mergedFrame;
        QImage scaledFrame = this->onionSkin->scaled(width * ratio, height * ratio);

        emit displayFrame(&scaledFrame);
    } else {
        QImage scaledFrame = frame->scaled(width * ratio, height * ratio);
        emit displayFrame(&scaledFrame);
    }
}

void SpriteEditor::addFrame(){
    QImage newFrame = QImage(width, height, QImage::Format_ARGB32);
    newFrame.fill(qRgba(0, 0, 0, 0));
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

bool SpriteEditor::translateAndDraw(int x, int y, bool draw) {
    x -= canvasPosition.x();
    y -= canvasPosition.y();

    if (x < 0 || x >= width * ratio || y < 0 || y >= height * ratio)
        return false;

    x = x / ratio;
    y = y / ratio;

    // only draw while the mouse is pressed, otherwise show brush preview for not drawing and erasing
    if (!mousePressed) {
        showCursorPreview(x, y);
        return true;
    } else if (!draw) {
        erasePixel(x, y);
        showCursorPreview(x, y);
    } else {
        drawPixel(x, y, currentColor);
        lastMousePosition = QPoint(-1, -1);
    }

    return true;
}

// mouse events

void SpriteEditor::onMouseMoved(int x, int y) {
    if(drawing)
        translateAndDraw(x, y, true);
    if(erasing)
        translateAndDraw(x, y, false);
}

void SpriteEditor::onMousePressed(int x, int y, bool pressed) {
    if (pressed && (drawing || erasing)) {
        mousePressed = true; // mouse is originally pressed, but was it in bounds of canvas?
        mousePressed = translateAndDraw(x, y, drawing || !erasing);
    } else
        mousePressed = false;
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
