#include "spriteEditor.h"
#include "QDebug"

SpriteEditor::SpriteEditor(QWidget *parent)
{
    drawing = true;
    erasing = false;
}

QImage SpriteEditor::generateOnionSkin(int frame){

}

void SpriteEditor::erasePixel(int x, int y){

    QImage* frame = &frames[currentFrame];

    frame->setPixel(x, y, qRgba(255, 255, 255, 0));

    displayCurrentFrame();
}

void SpriteEditor::drawPixel(int x, int y){

    QImage* frame = &frames[currentFrame];

    frame->setPixelColor(x, y, currentColor);

    displayCurrentFrame();
}

void SpriteEditor::setCurrentColor(const QColor &newColor){
    currentColor = newColor;
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
    }
    else if(currentFrame == 0 && frames.length()-1 > 0){
        frames.removeAt(0);
        emit deleteModelFrame(currentFrame);
        displayCurrentFrame();
    }
}

void SpriteEditor::adjustFrame(int val){
    currentFrame = val;
    displayCurrentFrame();
}

void SpriteEditor::onNewProject(int width, int height, QString name){

    this->height = height;
    this->width = width;
    this->name = name;
    frames = QVector<QImage>();
    currentColor = qRgba(255, 0, 0, 255);

    ratio = qMin(maxImageX / width, maxImageY / height);
    emit updateCanvasSize(width * ratio, height * ratio);

    addFrame();
}

void SpriteEditor::onMouseMoved(int x, int y){
    if(drawing)
        translateAndDraw(x, y, true);

    if(erasing)
        translateAndDraw(x, y, false);
}

void SpriteEditor::onMousePressed(int x, int y, bool pressed){
    if(pressed && drawing)
        translateAndDraw(x, y, true);

    if(pressed && erasing)
        translateAndDraw(x, y, false);
}

void SpriteEditor::currentCanvasPosition(int x, int y){
    canvasX = x;
    canvasY = y;

    qDebug() << x << " " << y;

}

void SpriteEditor::translateAndDraw(int x, int y, bool draw){
    x -= canvasX;
    y -= canvasY;

    if (x < 0 || x >= width * ratio || y < 0 || y >= height * ratio)
        return;

    x = x / ratio;
    y = y / ratio;

    if(draw)
        drawPixel(x, y);
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
