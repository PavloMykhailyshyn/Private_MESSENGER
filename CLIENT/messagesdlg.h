#ifndef MESSAGESDLG_H
#define MESSAGESDLG_H

#include <QDialog>
#include <QDesktopWidget>
#include <iostream>

namespace Ui
{
class MessagesDlg;
}

class MessagesDlg : public QDialog
{
    Q_OBJECT

public:

    explicit MessagesDlg(QWidget *parent = 0);
    ~MessagesDlg();

    void SetTitleAndText(const QString&, const QString&);

    void Clear();

private:

    Ui::MessagesDlg *ui;

};

#endif // MESSAGESDLG_H
