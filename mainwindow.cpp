#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(SpriteEditor editor, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    background(800, 800, QImage::Format_ARGB32)
{
    ui->setupUi(this);

    startdialog.setModal(true);
    startdialog.show();
    background.fill(qRgba(255, 255, 255, 255));
    ui->MainEditorFrame->setPixmap(QPixmap::fromImage(background));

    // connect(&startdialog, QDialog::accept, &editor, [&editor]()
    //         {editor.onNewProject(startdialog.height.value(), startdialog.width.value(), startdialog.projectName.value());});
}

MainWindow::~MainWindow()
{
    delete ui;
}
