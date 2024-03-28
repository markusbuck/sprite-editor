#include "spriteEditor.h"
#include "QDebug"

SpriteEditor::SpriteEditor(QWidget *parent)
{
    // width = 440;
    // height = 440;
}

QImage SpriteEditor::generateOnionSkin(int frame){

}

void SpriteEditor::erasePixel(int x, int y){

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
    QImage scaledFrame = frame->scaled(440, 440);
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

}

void SpriteEditor::adjustFrame(int val){
    currentFrame = val;
    displayCurrentFrame();
}

void SpriteEditor::onNewProject(int width, int height, QString name){

    this->height = height;
    this->width = width;
    this->ratioX = width / 440;
    this->ratioY = height / 440;
    this->name = name;
    this->frames = QVector<QImage>();

    addFrame();
}

void SpriteEditor::onMouseMoved(int x, int y){

    translateAndDraw(x, y);
}

void SpriteEditor::onMousePressed(int x, int y, bool pressed){
    // drawPixel();

    if(pressed)
        translateAndDraw(x, y);
}

void SpriteEditor::currentCanvasPosition(int x, int y){
    canvasX = x;
    canvasY = y;

    qDebug() << x << " " << y;

}

void SpriteEditor::translateAndDraw(int x, int y){
    x -= canvasX;
    y -= canvasY;

    if (x < 0 || x >= 440 || y < 0 || y >= 440)
        return;

    x = x * width / 440;
    y = y * height / 440;

    drawPixel(x, y);
}
