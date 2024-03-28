#include "spriteEditor.h"
#include "QDebug"

SpriteEditor::SpriteEditor(QWidget *parent)
{
    // width = 440;
    // height = 440;
    drawing = true;
    erasing = false;
}

QImage SpriteEditor::generateOnionSkin(int frame){

}

void SpriteEditor::erasePixel(int x, int y){

    QImage* frame = &frames[currentFrame];

    frame->setPixelColor(x, y, qRgba(255, 255, 255, 255));

    displayCurrentFrame();
}

void SpriteEditor::drawPixel(int x, int y){

    QImage* frame = &frames[currentFrame];

    frame->setPixelColor(x, y, qRgba(0, 0, 0, 0));

    displayCurrentFrame();
}

void SpriteEditor::setCurrentColor(int r, int g, int b, int a){

}

void SpriteEditor::displayCurrentFrame() {

    QImage* frame = &frames[currentFrame];
    QImage scaledFrame = frame->scaled(440, 440);
    emit displayFrame(&scaledFrame);
}

void SpriteEditor::addFrame(){
    QImage newFrame = QImage(width, height, QImage::Format_ARGB32);
    newFrame.fill(qRgba(255, 255, 255, 255));
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

    if(drawing)
        translateAndDraw(x, y, true);

    if(erasing)
        translateAndDraw(x, y, false);
}

void SpriteEditor::onMousePressed(int x, int y, bool pressed){
    // drawPixel();

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

    if (x < 0 || x >= 440 || y < 0 || y >= 440)
        return;

    x = x * width / 440;
    y = y * height / 440;

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
