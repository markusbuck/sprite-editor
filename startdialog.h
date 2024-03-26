#ifndef STARTDIALOG_H
#define STARTDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class StartDialog;
}

class StartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StartDialog(QWidget *parent = nullptr);
    ~StartDialog();

public slots:
    void onAccepted();

signals:
    void onProjectAccepted(int width, int height, QString name);

private:
    Ui::StartDialog *ui;
};

#endif // STARTDIALOG_H
