#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>

/*!
 * Authors: Kevin Soto, Zak Holt, Walker Hall, Brenden Suess, and Markus Buckwalter
 * CS 3505 Assignment 8: Sprite Editor Implementation
 * Date: 4/1/24
 *
 * This file contains the components that control the help dialog window of the sprite editor.
 */
namespace Ui {
class HelpDialog;
}


/*!
 * \brief The HelpDialog class
 */
class HelpDialog : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief HelpDialog - Constructor for HelpDialog.
     * \param parent - null pointer.
     */
    explicit HelpDialog(QWidget *parent = nullptr);

    /*!
     * \brief ~HelpDialog - Deconstructor for HelpDialog.
     */
    ~HelpDialog();

private:
    Ui::HelpDialog *ui;
};

#endif // HELPDIALOG_H
