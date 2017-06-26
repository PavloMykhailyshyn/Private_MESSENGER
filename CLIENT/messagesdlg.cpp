#include "messagesdlg.h"
#include "ui_messagesdlg.h"

MessagesDlg::MessagesDlg(QWidget *parent) : QDialog(parent), ui(new Ui::MessagesDlg)
{
#ifdef QT_DEBUG
    std::cout << "MessagesDlg created" << std::endl;
#endif

    ui->setupUi(this);

    this->move(QApplication::desktop()->screen()->rect().topRight() - this->rect().topRight());

}

MessagesDlg::~MessagesDlg()
{
    delete ui;
#ifdef QT_DEBUG
    std::cout << "MessagesDlg deleted" << std::endl;
#endif
}

void MessagesDlg::SetTitleAndText(const QString& title, const QString& text)
{
    this->setWindowTitle(title);
    ui->TEXT->setText(text);
}

void MessagesDlg::Clear()
{
    this->setWindowTitle("");
    ui->TEXT->setText("");
}
