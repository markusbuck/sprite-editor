#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(SpriteEditor& editor, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    startdialog.setModal(true);
    startdialog.show();

    connect(&startdialog, &StartDialog::onProjectAccepted, &editor, &SpriteEditor::onNewProject);
    connect(&editor, &SpriteEditor::displayFrame, this, &MainWindow::onDisplayCurrentFrame);
    connect(ui->AddFrameButton, &QPushButton::clicked, &editor, &SpriteEditor::addFrame );

    //mouse

    connect(this, &MainWindow::mousePress, &editor, &SpriteEditor::onMousePressed);
    connect(this, &MainWindow::mouseMove, &editor, &SpriteEditor::onMouseMoved);

    // canvas

    connect(this, &MainWindow::setCanvasPosition, &editor, &SpriteEditor::currentCanvasPosition);

    auto canvasPostion = ui->MainEditorCanvas->pos();
    emit setCanvasPosition(canvasPostion.x(), canvasPostion.y());
}

void MainWindow::onDisplayCurrentFrame(QImage *frame) {
    ui->MainEditorCanvas->setPixmap(QPixmap::fromImage(*frame));

    qDebug() << "on display";
}

MainWindow::~MainWindow()
{
    delete ui;
}

// mouse

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {

        emit mousePress(true);

    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton))
        emit mouseMove((int)event->position().x(), (int) event->position().y());
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        emit mousePress(false);
}

// void MainWindow::paintEvent(QPaintEvent *event)
// {
//     // QPainter painter(this);
//     // QRect dirtyRect = event->rect();
//     // painter.drawImage(dirtyRect, image, dirtyRect);
// }

// void MainWindow::drawLineTo(const QPoint &endPoint)
// {
//     // QPainter painter(&image);
//     // painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
//     //                     Qt::RoundJoin));
//     // painter.drawLine(lastPoint, endPoint);
//     // modified = true;

//     // int rad = (myPenWidth / 2) + 2;
//     // update(QRect(lastPoint, endPoint).normalized()
//     //            .adjusted(-rad, -rad, +rad, +rad));
//     // lastPoint = endPoint;
// }
