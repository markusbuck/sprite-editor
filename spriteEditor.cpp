#include "spriteEditor.h"
#include "QDebug"

SpriteEditor::SpriteEditor(QObject *parent) {}

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
    newFrame.fill(qRgba(currentFrame * 50, currentFrame * 50, 0, 255));
    frames.push_back(newFrame);
    currentFrame = frames.length() - 1;
    displayCurrentFrame();
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
