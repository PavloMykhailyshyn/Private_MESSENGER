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
    //connect(ui->AllContacts, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(ui->AddContact->setEnabled(true)));
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

    thread_ = new boost::thread(boost::bind(&CHAT::CLIENT_THREAD, this));
}

void CHAT::THREAD_DELETE()
{
    delete thread_;
#ifdef QT_DEBUG
    std::cout << "deleted Thread" << std::endl;
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
            ui->messages->append("Your message sent ! This contact is busy.");
            break;
        }
        else if (response == RECEIVER_OFFLINE)
        {
            ui->messages->append("Your message not sent ! This contact is offline.");
            break;
        }
        else if (response == MESSAGE_SENT)
        {
            ui->messages->append("Message sent successfully !");
            break;
        }
        break;
    }
    case RCV_MESSAGE:  // need to implement
    {
        str.erase(0, 1);
        // show window at the bottom right corner with received message for 5-10 sec
        break;
    }
    case STATUS_CHANGED:  // need to implement
    {

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

void CHAT::CLIENT_THREAD()
{
#ifdef QT_DEBUG
    std::cout << "Thread created" << std::endl;
#endif
    std::string str;
    bool p = true;
    while (p)
    {
        if (!client_ptr->GetString(str))
            break;

        if (!ProcessString(static_cast<char>(str[0]), str))
            break;
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

void CHAT::on_AllContacts_clicked(const QModelIndex &index)
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

void CHAT::on_Refresh_clicked()  // need to implement
{
    ui->AllContacts->clearSelection();
    ui->AddContact->setEnabled(false);

    ui->MyContactList->clearSelection();
    ui->Companion->setText("Companion");

    /*std::string get_list = RCV_CONTACT_LIST + username_;
    client_ptr->SendString(get_list);*/
}

void CHAT::on_MyContactList_itemClicked(QListWidgetItem *item)
{
    QString companion_name_and_state = item->text();
    QStringList companion = companion_name_and_state.split('\n');
    ui->Companion->setText(companion.at(0));
}

void CHAT::on_SEND_clicked()
{
    ui->AllContacts->clearSelection();
    ui->AddContact->setEnabled(false);
    if (ui->msg->text() != "" && ui->Companion->text() != "Companion")
    {
        QString NickName = QString::fromUtf8(username_.c_str());
        QString msg = MESSAGE + NickName + DELIMITER + ui->Companion->text() + DELIMITER + ui->msg->text();
        std::string converted_msg = msg.toUtf8().constData();
        client_ptr->SendString(converted_msg);
        ui->msg->clear();
    }
    else
    {
        QMessageBox::information(this, "Info", "Please select the recipient !");
    }
}
