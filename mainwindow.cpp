#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(SpriteEditor& editor, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    background(800, 800, QImage::Format_ARGB32)
{
    ui->setupUi(this);

    startdialog.setModal(true);
    startdialog.show();
    background.fill(qRgba(255, 255, 255, 255));
    ui->MainEditorFrame->setPixmap(QPixmap::fromImage(background));

    connect(&startdialog, &StartDialog::onProjectAccepted, &editor, &SpriteEditor::onNewProject);
    connect(&editor, &SpriteEditor::displayFrame, this, &MainWindow::onDisplayCurrentFrame);
    connect(ui->AddFrameButton, &QPushButton::clicked, &editor, &SpriteEditor::addFrame);
    connect(ui->frameAdjustor, &QSpinBox::valueChanged, &editor, &SpriteEditor::adjustFrame);
    connect(&editor, &SpriteEditor::updateFrameBox, ui->frameAdjustor,&QSpinBox::setValue);
    connect(&editor, &SpriteEditor::updateFrameBox, this, &MainWindow::updateMaxFrames);
}

void MainWindow::onDisplayCurrentFrame(QImage *frame) {
    ui->MainEditorFrame->setPixmap(QPixmap::fromImage(*frame));
}

void MainWindow::updateMaxFrames(int max){
    ui->frameAdjustor->setMaximum(max);
}


MainWindow::~MainWindow()
{
    delete ui;
}
