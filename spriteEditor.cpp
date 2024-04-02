#include "spriteEditor.h"
#include <QPainter>
#include <QFile>
#include <QJsonDocument>

/*!
 * \brief SpriteEditor::SpriteEditor
 * \param parent
 *
 * Implements the methods defined in spriteEditor.h
 *
 * Reviewed by Brenden Suess and Walker Hall
 */
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

    foreach(const QImage &image, frames) {
        QJsonArray imageDataArray;
        for (int i = 0; i < image.height(); ++i) {
            QJsonArray rowArray;
            for (int j = 0; j < image.width(); ++j) {
                QColor tempColor = image.pixelColor(j, i);
                QJsonArray pixelArray;
                pixelArray.append(tempColor.red());
                pixelArray.append(tempColor.green());
                pixelArray.append(tempColor.blue());
                pixelArray.append(tempColor.alpha());
                rowArray.append(pixelArray);
            }
            imageDataArray.append(rowArray);
        }
        imageFrames.append(imageDataArray);
    }

    // Add other JSON fields
    json["name"] = name;
    json["height"] = height;
    json["width"] = width;
    json["frames"] = imageFrames;

    emit jsonObject(json);

}

void SpriteEditor::toQImage(QString filePath){
    if(filePath.isNull())
        return;

    QFile file(filePath);

    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QString contents = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(contents.toUtf8());

    onNewProject(jsonDoc.object().value("width").toInt(), jsonDoc.object().value("height").toInt(), jsonDoc.object().value("name").toString());
    auto encodedArray = jsonDoc.object().value("frames").toArray();
    frames.clear();
    // Iterate over each item in the imageFrames array.
    for (const QJsonValue& imageDataValue : encodedArray) {
        QJsonArray imageDataArray = imageDataValue.toArray();

        // Create a QImage object with the dimensions of the imageDataArray.
        QImage image(imageDataArray[0].toArray().size(), imageDataArray.size(), QImage::Format_ARGB32);

        // Iterate over each row of the imageDataArray.
        for (int y = 0; y < imageDataArray.size(); ++y) {
            QJsonArray rowArray = imageDataArray[y].toArray();

            // Iterate over each pixel in the rowArray.
            for (int x = 0; x < rowArray.size(); ++x) {
                QJsonArray pixelArray = rowArray[x].toArray();

                // Extract RGBA values from the pixelArray and set pixel color in QImage.
                QColor color(pixelArray[0].toInt(), pixelArray[1].toInt(), pixelArray[2].toInt(), pixelArray[3].toInt());
                image.setPixelColor(x, y, color);
            }
        }
        // Add constructed QImage to the frames vector.
        frames.append(image);
    }

    currentFrame = frames.length() - 1;
    displayCurrentFrame();
    emit updateFrameBox(currentFrame);
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
