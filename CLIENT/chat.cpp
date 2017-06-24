#include "chat.h"
#include "ui_chat.h"


CHAT::CHAT(QWidget *parent) : QWidget(parent), ui(new Ui::CHAT)
{
#ifdef QT_DEBUG
    std::cout << "CHAT created" << std::endl;
#endif
    ui->setupUi(this);

    this->move(QApplication::desktop()->screen()->rect().center() - this->rect().center());

    ui->AddContact->setEnabled(false);
}

CHAT::~CHAT()
{
    delete ui;
#ifdef QT_DEBUG
    std::cout << "deleted CHAT" << std::endl;
#endif
}

void CHAT::THREAD_CREATE()
{
    thread1_ = new boost::thread(boost::bind(&CHAT::CLIENT_THREAD, this));
    thread2_ = new boost::thread(boost::bind(&CHAT::CLIENT_THREAD_MSG, this));
}

void CHAT::THREAD_DELETE()
{
    delete thread1_;
    delete thread2_;
#ifdef QT_DEBUG
    std::cout << "deleted Thread1" << std::endl;
    std::cout << "deleted Thread2" << std::endl;
#endif
}

bool CHAT::ProcessString(char symbol, std::string str)
{
    switch (symbol)
    {
    case RCV_CONTACT_LIST:
    {
        str.erase(0, 1);
        QStringList contacts;
        std::istringstream stream(str);
        std::string username_and_state;
        while (std::getline(stream, username_and_state, END_OF_STR))
        {
            switch (username_and_state.back())
            {
            case 'o':
                username_and_state.erase(username_and_state.end() - 2, username_and_state.end());
                username_and_state += "\n| online  |\n";
                username_and_state = "  " + username_and_state;
                break;
            case 'f':
                username_and_state.erase(username_and_state.end() - 2, username_and_state.end());
                username_and_state += "\n| offline |\n";
                username_and_state = "  " + username_and_state;
                break;
            case 'b':
                username_and_state.erase(username_and_state.end() - 2, username_and_state.end());
                username_and_state += "\n| busy     |\n";
                username_and_state = "  " + username_and_state;
                break;
            }
            contacts += QString::fromUtf8(username_and_state.c_str());
        }
        ui->AllContacts->clear();
        ui->AllContacts->addItems(contacts);

        break;
    }
    case MESSAGE:
    {
        std::string response;
        response.push_back(str[0]);
        response.push_back(str[1]);
        if(response == RECEIVER_BUSY)
        {
            ui->messages->append("Message sent ! This contact is busy.");
            break;
        }
        else if (response == RECEIVER_OFFLINE)
        {
            ui->messages->append("Message not sent ! This contact is offline.");
            break;
        }
        else if (response == MESSAGE_SENT)
        {
            ui->messages->append("Message sent successfully !");
            break;
        }
        break;
    }
    case STATUS_CHANGED:  // need to implement
    {
        str.erase(0, 1);

        break;
    }
    case REFRESH_CONTACTS: // need to implement
    {
        str.erase(0, 1);

        break;
    }
    default:
    {
#ifdef QT_DEBUG
        std::cerr << "Unrecognized message type: " << symbol << "\n";
#endif
        break;
    }
    }
    return true;
}

void CHAT::ProcessStringMsg(std::string str)
{
    str.erase(0, 1);
    QString sender_and_msg = QString::fromUtf8(str.c_str());
    QStringList sender_and_msg_splitted = sender_and_msg.split(DELIMITER);
    QTimer * timer = new QTimer(this);
    timer->setInterval(5000);
    MessagesDlg * dlg = new MessagesDlg;
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->SetTitleAndText(sender_and_msg_splitted.at(0), sender_and_msg_splitted.at(1));
    connect(timer, SIGNAL(timeout()), dlg, SLOT(close()));
    dlg->show();
    timer->start();
}

void CHAT::CLIENT_THREAD()
{
#ifdef QT_DEBUG
    std::cout << "Thread1 created" << std::endl;
#endif
    std::string str;
    bool p = true;
    while (p)
    {
        if (!client_ptr->GetString(str))
            break;
        if (static_cast<char>(str[0]) != RCV_MESSAGE)
        {
            if (!ProcessString(static_cast<char>(str[0]), str))
                break;
        }
    }
#ifdef QT_DEBUG
    std::cerr << "Lost connection to the server.\n";
#endif
}

void CHAT::CLIENT_THREAD_MSG()
{
#ifdef QT_DEBUG
    std::cout << "Thread2 created" << std::endl;
#endif
    std::string str;
    bool p = true;
    while (p)
    {
        if (!client_ptr->GetString(str))
            break;

        if (static_cast<char>(str[0]) == RCV_MESSAGE)
        {
            ProcessStringMsg(str);
        }
    }
#ifdef QT_DEBUG
    std::cerr << "Lost connection to the server.\n";
#endif
}

void CHAT::SetNameAndPass(const std::string& username, const std::string& password)
{
    this->username_ = username;
    this->password_ = password;
    QString NickName = QString::fromUtf8(username_.c_str());
    ui->NickName->setText(NickName);
}


void CHAT::on_GetContactList_clicked()
{
    std::string get_list = RCV_CONTACT_LIST + username_;
    client_ptr->SendString(get_list);
    ui->AddContact->setEnabled(false);
}

void CHAT::on_AddContact_clicked()
{
    QList<QListWidgetItem *> check = ui->MyContactList->findItems(ui->AllContacts->currentItem()->text(), Qt::MatchContains);
    if (check.count() < 1)
    {
        ui->MyContactList->addItem(ui->AllContacts->currentItem()->text());
    }
    ui->AllContacts->clearSelection();
    ui->AddContact->setEnabled(false);
}

void CHAT::on_AllContacts_clicked(const QModelIndex&)
{
    ui->AddContact->setEnabled(true);
}

void CHAT::on_ChangeState_clicked()
{
    ui->AllContacts->clearSelection();
    ui->AddContact->setEnabled(false);

    ui->MyContactList->clearSelection();
    ui->Companion->setText("Companion");

    QString status = ui->Status->currentText();
    std::string converted_status;
    if (status == "OFFLINE")
    {
        converted_status = STATUS_CHANGED + username_ + DELIMITER + 'f';
    }
    else if (status == "BUSY")
    {
        converted_status = STATUS_CHANGED + username_ + DELIMITER + 'b';
    }
    else if (status == "ONLINE")
    {
        converted_status = STATUS_CHANGED + username_ + DELIMITER + 'o';
    }
    client_ptr->SendString(converted_status);
}

void CHAT::on_Refresh_clicked()
{
    ui->AllContacts->clearSelection();
    ui->AddContact->setEnabled(false);

    ui->MyContactList->clearSelection();
    ui->Companion->setText("Companion");

    std::string refresh = REFRESH_CONTACTS + username_;
    client_ptr->SendString(refresh);
}

void CHAT::on_MyContactList_itemClicked(QListWidgetItem * item)
{
    QString companion_name_and_state = item->text();
    QStringList companion = companion_name_and_state.split(END_OF_STR);
    ui->Companion->setText(companion.at(0));
}

void CHAT::on_SEND_clicked()
{
    ui->AllContacts->clearSelection();
    ui->AddContact->setEnabled(false);
    if(ui->Status->currentText() != "OFFLINE")
    {
        if (ui->Companion->text() != "Companion")
        {
            if (ui->msg->text() != "")
            {
                QString NickName = QString::fromUtf8(username_.c_str());
                QString msg = MESSAGE + NickName + DELIMITER + ui->Companion->text() + DELIMITER + ui->msg->text();
                std::string converted_msg = msg.toUtf8().constData();
                client_ptr->SendString(converted_msg);
                ui->msg->clear();
            }
        }
        else
        {
            QMessageBox::information(this, "Info", "Please select the recipient !");
        }
    }
    else
    {
        QMessageBox::information(this, "Info", "You cannot send a message when you are OFFLINE !");
    }
}
