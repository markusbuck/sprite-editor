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
    connect(ui->AddFrameButton, &QPushButton::clicked, &editor, &SpriteEditor::addFrame );
}

void MainWindow::onDisplayCurrentFrame(QImage *frame) {
    ui->MainEditorFrame->setPixmap(QPixmap::fromImage(*frame));
}

MainWindow::~MainWindow()
{
    delete ui;
}
