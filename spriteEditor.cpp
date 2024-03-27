#include "spriteEditor.h"
#include "QDebug"

SpriteEditor::SpriteEditor(QWidget *parent)
{
    qDebug() << "Constructor";
    width = 0;
    height = 0;
}

QImage SpriteEditor::generateOnionSkin(int frame){

}

void SpriteEditor::erasePixel(int x, int y){

}

void SpriteEditor::drawPixel(int x, int y){

}

void SpriteEditor::setCurrentColor(int r, int g, int b, int a){

}

void SpriteEditor::displayCurrentFrame() {
    emit displayFrame(&frames[currentFrame]);
}

void SpriteEditor::addFrame(){
    QImage newFrame = QImage(width, height, QImage::Format_ARGB32);
    newFrame.fill(qRgba(255, 255, 255, 255));
    frames.push_back(newFrame);
    currentFrame = frames.length() - 1;
    displayCurrentFrame();

    qDebug() << "add frame";
}

void SpriteEditor::deleteFrame(){

}

void SpriteEditor::onNewProject(int width, int height, QString name){
    this->height = height;
    this->width = width;
    this->name = name;
    this->frames = QVector<QImage>();

    addFrame();
}

void SpriteEditor::onMouseMoved(int x, int y){

    x -= canvasX;
    y -= canvasY + 20;

    if(x < 0 || x > width || y < 0 || y > height)
        return;

    qDebug() << x << " " << y;
}

void SpriteEditor::onMousePressed(bool pressed){
    // qDebug() << pressed;
}

void SpriteEditor::currentCanvasPosition(int x, int y){
    canvasX = x;
    canvasY = y;

    qDebug() << x << " " << y;

}
