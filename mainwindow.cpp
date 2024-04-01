#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(SpriteEditor &editor, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    startdialog.setModal(true);
    startdialog.show();

    colorDialog.setModal(true);

    // new project
    connect(&startdialog, &StartDialog::onProjectAccepted, &editor, &SpriteEditor::onNewProject);
    connect(ui->action_new, &QAction::triggered, &editor, [this](bool) { startdialog.show(); });

    connect(&colorDialog, &QColorDialog::colorSelected, [&editor](const QColor& newColor){
        editor.setCurrentColor(newColor);
    });
    connect(ui->ColorButton, &QPushButton::clicked, this, [this](){ colorDialog.show(); });

    //save/load
    connect(ui->action_save, &QAction::triggered,&editor,&SpriteEditor::toJson);
    connect(&editor, &SpriteEditor::jsonObject,this, &MainWindow::saveAs);

    connect(ui->action_load, &QAction::triggered, this, &MainWindow::onLoad);
    connect(this, &MainWindow::loadFile, &editor, &SpriteEditor::toQImage);

    // frames
    connect(&editor, &SpriteEditor::displayFrame, this, &MainWindow::onDisplayCurrentFrame);

    // preview
    connect(&editor, &SpriteEditor::displayPreview, this, &MainWindow::onDisplayPreview);
    connect(ui->frameRate, &QSlider::valueChanged, &editor, &SpriteEditor::adjustFPS);
    connect(ui->frameRate, &QSlider::valueChanged, this, &MainWindow::onFrameRateSlide);

    // playback
    connect(ui->boomerangButton, &QPushButton::clicked, &editor, &SpriteEditor::toggleBoomerang);
    connect(ui->reverseButton, &QPushButton::clicked, &editor, &SpriteEditor::toggleReverse);
    connect(ui->forwardButton, &QPushButton::clicked, &editor, &SpriteEditor::toggleForward);
    connect(ui->actualSizeBox, &QCheckBox::stateChanged, &editor, &SpriteEditor::toggleActualSize);

    // add
    connect(ui->AddFrameButton, &QPushButton::clicked, &editor, &SpriteEditor::addFrame);
    connect(ui->frameAdjustor, &QSpinBox::valueChanged, &editor, &SpriteEditor::adjustFrame);
    connect(&editor, &SpriteEditor::updateFrameBox, this, &MainWindow::updateMaxFrames);
    connect(&editor, &SpriteEditor::updateFrameBox, ui->frameAdjustor, &QSpinBox::setValue);
    // delete
    connect(ui->DeleteFrameButton, &QPushButton::clicked, &editor, &SpriteEditor::deleteFrame);
    connect(&editor, &SpriteEditor::deleteModelFrame, this, &MainWindow::deleteViewFrame);

    // toolbar

    connect (ui->DrawButton, &QPushButton::clicked, &editor, &SpriteEditor::onDrawPressed);
    connect (ui->EraseButton, &QPushButton::clicked, &editor, &SpriteEditor::onErasePressed);

    // mouse
    connect(this, &MainWindow::mousePress, &editor, &SpriteEditor::onMousePressed);
    connect(this, &MainWindow::mouseMove, &editor, &SpriteEditor::onMouseMoved);

    // onion skin
    connect(ui->OnionSkin, &QCheckBox::stateChanged, &editor, &SpriteEditor::toggleOnionSkin);

    // canvas
    connect(this, &MainWindow::setCanvasPosition, &editor, &SpriteEditor::currentCanvasPosition);
    connect(&editor, &SpriteEditor::updateCanvasSize, this, &MainWindow::onUpdateCanvasSize);

    auto canvasPostion = ui->MainEditorCanvas->pos();
    emit setCanvasPosition(canvasPostion.x(), canvasPostion.y());

}

void MainWindow::onDisplayCurrentFrame(QImage *frame){
    ui->MainEditorCanvas->setPixmap(QPixmap::fromImage(*frame));
}

void MainWindow::onDisplayPreview(QImage *frame){
    ui->Preview->setPixmap(QPixmap::fromImage(*frame));
}

void MainWindow::onFrameRateSlide(int value){
    ui->frameRateLabel->setNum(value);
}

void MainWindow::updateMaxFrames(int max){
    ui->frameAdjustor->setMaximum(max);
}

MainWindow::~MainWindow(){
    delete ui;
}

//delete
void MainWindow::deleteViewFrame(int value){
    ui->frameAdjustor->setMaximum(ui->frameAdjustor->maximum()-1);
    ui->frameAdjustor->setValue(value);
}
//saveas
void MainWindow::saveAs(QJsonObject json){
    //Open file saving menu and obtain file path.
    QString filePath = QFileDialog::getSaveFileName(nullptr,"","",".json");
    if (!filePath.isEmpty()){
        QFile file(filePath);
        //Convert the json document into a byte format.
        if (file.open(QIODevice::WriteOnly)){
            QJsonDocument jsonDocument;
            jsonDocument.setObject(json);
            QByteArray jsonData = jsonDocument.toJson();
            qint64 bytesWritten = file.write(jsonData);

            if (bytesWritten == -1) {
                qDebug() << "Error writing to file:" << file.errorString();
            } else {
                qDebug() << "File saved successfully";
            }

            file.close();
        } else {
            qDebug() << "Failed to open file for writing:" << file.errorString();
        }
    }
}

void MainWindow::onLoad(){
    QString filepath = QFileDialog::getOpenFileName();

    emit loadFile(filepath);
}
// mouse
void MainWindow::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton)
        emit mousePress((int)event->position().x(), (int)event->position().y(), true);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    emit mouseMove((int)event->position().x(), (int)event->position().y());
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton)
        emit mousePress((int)event->position().x(), (int)event->position().y(), false);
}

void MainWindow::onUpdateCanvasSize(int x, int y){
    auto canvasPostion = ui->MainEditorCanvas->pos();
    ui->MainEditorCanvas->setGeometry(canvasPostion.x(), canvasPostion.y(), x, y);
}
