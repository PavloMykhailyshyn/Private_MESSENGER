#ifndef LOGIN_H
#define LOGIN_H

#include "chat.h"
#include <QMessageBox>

namespace Ui
{
class LogIn;
}

class LogIn : public QWidget
{
    Q_OBJECT

    std::string ip_addr_ = IP_ADDRESS;
    std::int32_t port_ = PORT;

public:

    explicit LogIn(QWidget *parent = 0);
    ~LogIn();

private slots:

    void on_START_clicked();

    void on_CreateAnAcc_clicked();

    void CHECK(QString);

private:

    Ui::LogIn *ui;
    CHAT *ch;

    bool flag_thread_ = false;
};

#endif // LOGIN_H
