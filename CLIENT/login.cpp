#include "login.h"
#include "ui_login.h"


LogIn::LogIn(QWidget *parent) : QWidget(parent), ui(new Ui::LogIn)
{
#ifdef QT_DEBUG
    std::cout << "LOGIN created" << std::endl;
#endif

    ui->setupUi(this);
    ch = new CHAT;

    this->setFixedSize(QSize(313, 188));

    client_ptr->SetIpv4Addr(ip_addr_);
    client_ptr->SetPort(port_);

    if (!client_ptr->Connect())
    {
#ifdef QT_DEBUG
        std::cerr << "Connection ERROR!\n";
#endif
    }

    ui->password->setEchoMode(QLineEdit::Password);

    connect(ui->login, SIGNAL(textChanged(QString)), SLOT(CHECK(QString)));
    connect(ui->password, SIGNAL(textChanged(QString)), SLOT(CHECK(QString)));
}

LogIn::~LogIn()
{
    if (flag_thread_)
        ch->THREAD_DELETE();
    delete ch;
    delete ui;
#ifdef QT_DEBUG
    std::cout << "deleted LOGIN" << std::endl;
#endif
    client_ptr->clientsocket::ResetInstance();
}

void LogIn::on_START_clicked()
{
    if (ui->login->text() != "" && ui->password->text() != "")
    {
        QString q_login = ui->login->text();
        QString q_password = ui->password->text();
        std::string login = q_login.toUtf8().constData();
        std::string password = q_password.toUtf8().constData();
        std::string login_and_pass = SEND_LOGIN_PASS + login + DELIMITER + password;
#ifdef QT_DEBUG
        std::cout << login_and_pass << std::endl;
#endif
        std::string response;
        if (!client_ptr->SendString(login_and_pass))
        {
#ifdef QT_DEBUG
            std::cerr << "ERROR sending login and password!\n";
#endif
        }
        else
        {
            if(client_ptr->GetString(response))
            {
#ifdef QT_DEBUG
                std::cout << response << std::endl;
#endif
                if (response == SUCCESS_LOGIN)
                {
                    hide();
                    ch->CREATE_DLG();
                    ch->CREATE_TIMER();
                    ch->THREAD_CREATE();
                    flag_thread_ = true;
                    ch->SetNameAndPass(login, password);
                    ch->show();
                    close();
                }
                else if(response == ALREADY_IN_USE)
                {
                    QMessageBox::warning(this, "!!!", "This username is online !");
                }
                else
                {
                    QMessageBox::information(this, "Info", "Username or password is incorrect !");
                }
            }
            else
            {
#ifdef QT_DEBUG
                std::cerr << "Could not get response from server!\n";
#endif
            }
        }
    }
    else
    {
        QMessageBox::information(this, "Info", "Please fill in all fields !");
    }
}

void LogIn::on_CreateAnAcc_clicked()
{
    if (ui->login->text() != "" && ui->password->text() != "")
    {
        QString q_username = ui->login->text();
        QString q_password = ui->password->text();
        std::string username = q_username.toUtf8().constData();
        std::string password = q_password.toUtf8().constData();
        std::string login_and_pass = SEND_USERNAME_PASS + username + DELIMITER + password;
        std::string response;
        std::cout << login_and_pass << std::endl;
        if (!client_ptr->SendString(login_and_pass))
        {
#ifdef QT_DEBUG
            std::cerr << "ERROR sending login and password!\n";
#endif
        }
        else
        {
            if(client_ptr->GetString(response))
            {
#ifdef QT_DEBUG
                std::cout << response << std::endl;
#endif
                if (response == SUCCESS_CREATION_ACC)
                {
                    hide();
                    ch->CREATE_DLG();
                    ch->CREATE_TIMER();
                    ch->THREAD_CREATE();
                    flag_thread_ = true;
                    ch->SetNameAndPass(username, password);
                    ch->show();
                    close();
                }
                else
                {
                    QMessageBox::information(this, "Info", "This username already exists...");
                }
            }
            else
            {
#ifdef QT_DEBUG
                std::cerr << "Cannot get response from server!\n";
#endif
            }
        }
    }
    else
    {
        QMessageBox::information(this, "Info", "Please fill in all fields !");
    }
}

void LogIn::CHECK(QString)
{
    QRegExp rx1 ("^[A-Za-z0-9_-]{4,16}$");  // for login
    QRegExp rx2 ("^.{6,}$");                // for password
    if (rx1.exactMatch(ui->login->text()) && rx2.exactMatch(ui->password->text()))
    {
        ui->START->setEnabled(true);
        ui->CreateAnAcc->setEnabled(true);
    }
    else
    {
        ui->START->setEnabled(false);
        ui->CreateAnAcc->setEnabled(false);
    }
}
