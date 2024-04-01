#include "spriteEditor.h"
#include "QDebug"
#include <QPainter>

SpriteEditor::SpriteEditor(QWidget *parent)
{
    drawing = true;
    erasing = false;
    mousePressed = false;
    forward = true;
    reverse = false;
    boomerang = false;
    boomerangDirection = true;
    isScaledPreview = true;
    lastMousePosition = QPoint(-1, -1);

    this->timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SpriteEditor::framePreview);
    this->timer->start(1000);
}

void SpriteEditor::framePreview(){
    if(frames.length() > 0){
        if(index >= frames.length()){
            index = frames.length() - 1;
        }
        if(forward){
            forwardPlayback();
        }
        else if(boomerang){
            boomerangPlayback();
        }
        else if(reverse){
            reversePlayback();
        }
        QImage preview = frames.at(index);
        QImage previewFrame;
        if(isScaledPreview) {
            previewFrame = preview.scaled(width * previewRatio, height * previewRatio);
        }
        else{
            previewFrame = preview;
        }
        emit displayPreview(&previewFrame);
    }
}

void SpriteEditor::forwardPlayback(){
    if(index == frames.length() - 1){
        index = 0;
    }
    else if(index < frames.length() - 1){
        index++;
    }
}

void SpriteEditor::reversePlayback(){
    if(index == 0){
        index = frames.length() - 1;
    }
    else if(index > 0){
        index--;
    }
}

void SpriteEditor::boomerangPlayback(){
    if(boomerangDirection){
        if(index == frames.length() - 1){
            boomerangDirection = false;
        }
        else if(index < frames.length() - 1){
            index++;
        }
    }else{
        if(index == 0){
            boomerangDirection = true;
        }
        else if(index > 0){
            index--;
        }
    }
}

void SpriteEditor::toggleBoomerang(){
    boomerang = true;
    forward = false;
    reverse = false;
}

void SpriteEditor::toggleForward(){
    boomerang = false;
    forward = true;
    reverse = false;
    boomerangDirection = true;
}

void SpriteEditor::toggleReverse(){
    boomerang = false;
    forward = false;
    reverse = true;
    boomerangDirection = false;
}

void SpriteEditor::toggleActualSize(int state){
    if(state == 0){
        isScaledPreview = true;
    }
    else if(state == 2){
        isScaledPreview = false;
    }
}

void SpriteEditor::adjustFPS(int FPS){
    int adjustedFPS = 1000 / FPS;
    this->timer->setInterval(adjustedFPS);
}

void SpriteEditor::toggleOnionSkin(int state){
    if(state == 0){
        this->isOnionSkinOn = false;
        displayCurrentFrame();
    }
    else if(state == 2){
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

    QImage *frame = &frames[currentFrame];

    QImage onionFrame = this->generateOnionSkin();
    QImage mergedFrame = QImage(width, height, QImage::Format_ARGB32);
    mergedFrame.fill(qRgba(0, 0, 0, 0));

    if(this->isOnionSkinOn){
        for(int y = 0; y < this->height; y++){
            for(int x = 0; x < this->width; x++){
                QRgb onionColor = onionFrame.pixel(x, y);
                QRgb currentColor = frame->pixel(x, y);

                if (qAlpha(onionColor) == 0 && qAlpha(currentColor) == 0) {
                    continue;
                } else if (qAlpha(onionColor) == 0) {
                    mergedFrame.setPixel(x, y, currentColor);
                } else if (qAlpha(currentColor) == 0) {
                    mergedFrame.setPixel(x, y, onionColor);
                } else {
                    mergedFrame.setPixelColor(x, y, onionColor);
                    mergedFrame.setPixelColor(x, y, currentColor);
                }
            }
        }

        this->onionSkin = &mergedFrame;
        frame = &mergedFrame;
    }

    // alpha channel
    QImage alphaPattern = QImage(frame->size(), QImage::Format_ARGB32);
    int patternRatio = (int)qMax(ratio / 16, 4.0);

    for (int y = 0; y < alphaPattern.height(); y++) {
        for (int x = 0; x < alphaPattern.width(); x++) {
            QRgb color = ((x / patternRatio) % 2 == (y / patternRatio) % 2) ? qRgb(192, 192, 192) : qRgb(220, 220, 220);
            alphaPattern.setPixel(x, y, color);
        }
    }

    for (int y = 0; y < frame->height(); ++y) {
        for (int x = 0; x < frame->width(); ++x) {
            QRgb pixel = frame->pixel(x, y);
            if (qAlpha(pixel) > 0)
                alphaPattern.setPixel(x, y, pixel);
        }
    }

    QImage scaledFrame = alphaPattern.scaled(width * ratio, height * ratio);
    // QImage scaledPreview = alphaPattern.scaled(width * previewRatio, height * previewRatio);
    // emit displayPreview(&scaledPreview);
    emit displayFrame(&scaledFrame);
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
    previewRatio = qMin(maxPreviewSize.x() / width, maxPreviewSize.y() / height);
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

    if (x < 0 || y < 0)
        return;

    lastPreviewColor = frames[currentFrame].pixelColor(x, y);
    drawPixel(x, y, previewColor);
}

bool SpriteEditor::translateAndDraw(int x, int y, bool draw) {
    x -= canvasPosition.x();
    y -= canvasPosition.y();

    if (x < 0 || x >= width * ratio || y < 0 || y >= height * ratio) {
        showCursorPreview(-1, -1);
        return false;
    }

    x = x / ratio;
    y = y / ratio;

    // only draw while the mouse is pressed, otherwise show brush preview for not drawing and erasing
    if (!mousePressed) {
        showCursorPreview(x, y);
        return true;
    } else if (!draw) {
        erasePixel(x, y);
        lastMousePosition = QPoint(-1, -1);
        // showCursorPreview(x, y);
    } else {
        drawPixel(x, y, currentColor);
        lastMousePosition = QPoint(-1, -1);
    }

    return true;
}
//JsonConversion
void SpriteEditor::toJson(){
    QJsonObject json;
    QJsonArray imageFrames;

    for (const QImage &image : frames) {
        QByteArray ba;
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);

        if (!image.isNull()) {
            if (image.save(&buffer, "PNG")) {
                QVariant variant(ba.toBase64()); // Convert QByteArray to base64 string
                QJsonValue tempValue;
                tempValue = variant.toString();
                imageFrames.append(tempValue);
            } else {
                qDebug() << "Failed to save image to buffer";
                // Handle error appropriately
            }
        } else {
            qDebug() << "Image is null";
            // Handle error appropriately or skip adding null images
        }
    }

    json["frames"] = imageFrames;
    emit jsonObject(json);
    // //Json object.
    // QJsonObject json;
    // QJsonArray imageFrames;
    // // QByteArray ba;
    // for(QImage &image : frames){
    //     QByteArray ba;
    //     QBuffer buffer(&ba);
    //     buffer.open(QIODevice::WriteOnly);
    //     image.save(&buffer,"PNG");
    //     QVariant varient = QVariant(image);
    //     QJsonValue tempValue;
    //     tempValue.fromVariant(varient);
    //     imageFrames.append(tempValue);
    // }
    // json["frames"] = imageFrames;
    // emit jsonObject(json);
    // //Json object ot hold array.
    // QImage image;
    // //Conversion to byte array.
    // QByteArray ba;
    // QBuffer buffer(&ba);
    // buffer.open(QIODevice::WriteOnly);
    // image.save(&buffer, "PNG"); // writes image into ba in PNG format.
    // //Store byte array in varient to create json value.
    // QVariant varient = QVariant(image);
    // QJsonValue tempValue;
    // tempValue.fromVariant(varient);
    // //How to append elements to the array.
    // imageFrames.append(tempValue);

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
