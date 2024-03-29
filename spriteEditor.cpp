#include "spriteEditor.h"
#include "QDebug"

SpriteEditor::SpriteEditor(QWidget *parent)
{
    // width = 440;
    // height = 440;
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

void SpriteEditor::erasePixel(int x, int y){

}

void SpriteEditor::drawPixel(int x, int y){

    QImage* frame = &frames[currentFrame];

    frame->setPixelColor(x, y, qRgba(0, 0, 0, 0));

    displayCurrentFrame();
}

void SpriteEditor::setCurrentColor(int r, int g, int b, int a){

}

void SpriteEditor::displayCurrentFrame(){

    if(this->isOnionSkinOn){

        QImage onionFrame = this->generateOnionSkin();
        QImage copyCurrentFrame = this->frames.at(this->currentFrame).copy();
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
        QImage scaledFrame = this->onionSkin->scaled(440, 440);

        emit displayFrame(&scaledFrame);
    }

    else{

        QImage* frame = &frames[currentFrame];
        QImage scaledFrame = frame->scaled(440, 440);
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
    x -= canvasX;
    y -= canvasY;

    if (x < 0 || x >= 440 || y < 0 || y >= 440)
        return;

    x = x * width / 440;
    y = y * height / 440;

    drawPixel(x, y);
}

void SpriteEditor::onMousePressed(bool pressed){
    // drawPixel();


}

void SpriteEditor::currentCanvasPosition(int x, int y){
    canvasX = x;
    canvasY = y;

    qDebug() << x << " " << y;

}
