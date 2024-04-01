#ifndef STARTDIALOG_H
#define STARTDIALOG_H

#include <QDialog>
#include <QString>

/*!
 * Authors: Kevin Soto, Zak Holt, Walker Hall, Brenden Suess, and Markus Buckwalter
 * CS 3505 Assignment 8: Sprite Editor Implementation
 * Date: 4/1/24
 *
 * This file contains the components that control the start dialog window of the sprite editor.
 */
namespace Ui{
class StartDialog;
}

/*!
 * \brief The StartDialog class
 */
class StartDialog : public QDialog{
    Q_OBJECT

public:
    /*!
     * \brief StartDialog - Constructor for StartDialog.
     * \param parent - null pointer.
     */
    explicit StartDialog(QWidget *parent = nullptr);

    /*!
     * \brief ~StartDialog - Deconstructor for StartDialog.
     */
    ~StartDialog();

public slots:
    /*!
     * \brief onAccepted - slot to be used when the accept button
     * is pressed in the start dialog
     */
    void onAccepted();

signals:
    /*!
     * \brief onProjectAccepted - signal to be sent when the accept button is pressed.
     * \param width - width of project given by the user.
     * \param height - height of project given by the user.
     * \param name - name of project.
     */
    void onProjectAccepted(int width, int height, QString name);

private:
    Ui::StartDialog *ui;
};

#endif // STARTDIALOG_H
