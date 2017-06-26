#include "chat.h"
#include "ui_chat.h"


CHAT::CHAT(QWidget *parent) : QWidget(parent), ui(new Ui::CHAT)
{
#ifdef QT_DEBUG
    std::cout << "CHAT created" << std::endl;
#endif
    ui->setupUi(this);

    qRegisterMetaType<QTextCursor>("QTextCursor");
    qRegisterMetaType<QItemSelection>("QItemSelection");

    this->move(QApplication::desktop()->screen()->rect().center() - this->rect().center());

    ui->AllContacts->setSelectionMode(QAbstractItemView::ExtendedSelection);
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
                break;
            case 'f':
                username_and_state.erase(username_and_state.end() - 2, username_and_state.end());
                username_and_state += "\n| offline |\n";
                break;
            case 'b':
                username_and_state.erase(username_and_state.end() - 2, username_and_state.end());
                username_and_state += "\n| busy     |\n";
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
#ifdef QT_DEBUG
        std::cout << response << std::endl;
#endif
        if(response == RECEIVER_BUSY)
        {
            ui->messages->append("Message not sent ! You are busy.");
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
    case RCV_MESSAGE:
    {
        str.erase(0, 1);
        QString sender_and_msg = QString::fromUtf8(str.c_str());
        QStringList sender_and_msg_splitted = sender_and_msg.split(DELIMITER);

        dlg->Clear();
        dlg->SetTitleAndText(sender_and_msg_splitted.at(0), sender_and_msg_splitted.at(1));
        dlg->show();

        moveToThread(QApplication::instance()->thread());
        QMetaObject::invokeMethod(timer, "start", Qt::QueuedConnection);

        break;
    }
    case BROADCAST_MESSAGE:
    {
        std::string response;
        response.push_back(str[0]);
        response.push_back(str[1]);
#ifdef QT_DEBUG
        std::cout << response << std::endl;
#endif
        if (response == BROADCAST_MESSAGE_SENT)
        {
            QString data = QString::fromUtf8(str.c_str());
            QString receivers = ui->Companion->text();
            QStringList data_list = data.split(DELIMITER);
            QStringList receivers_list = receivers.split(',');
            for (std::int32_t i(0); i < receivers_list.size(); ++i)
            {
                if(data_list.at(i + 1) == '0')
                {
                    QString temp_str = "Message not sent ! Contact " + static_cast<QString>("[ ") + static_cast<QString>(receivers_list.at(i)) + static_cast<QString>(" ]") + static_cast<QString>(" is offline.");
                    ui->messages->append(temp_str);
                }
                else if(data_list.at(i + 1) == '1')
                {
                    QString temp_str = "Message sent successfully to contact " + static_cast<QString>("[ ") + static_cast<QString>(receivers_list.at(i)) + static_cast<QString>(" ] .");
                    ui->messages->append(temp_str);
                }
            }
        }
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

void CHAT::CREATE_DLG()
{
    dlg = new MessagesDlg(this);
}

void CHAT::CREATE_TIMER()
{
    timer = new QTimer(this);
    timer->setInterval(3000);
    connect(timer, SIGNAL(timeout()), dlg, SLOT(close()));
}

void CHAT::SetNameAndPass(const std::string& username, const std::string& password)
{
    this->username_ = username;
    this->password_ = password;
    QString NickName = QString::fromUtf8(username_.c_str());
    ui->NickName->setText(NickName);
}

void CHAT::on_AllContacts_itemClicked(QListWidgetItem * item)
{
    QString companion_name_and_state = item->text();
    QStringList companion = companion_name_and_state.split(END_OF_STR);
    ui->Companion->setText(companion.at(0));
}

void CHAT::on_GetContactList_clicked()
{
    std::string get_list = RCV_CONTACT_LIST + username_;
    client_ptr->SendString(get_list);
}

void CHAT::on_ChangeState_clicked()
{
    ui->AllContacts->clearSelection();

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

    ui->Companion->setText("Companion");

    std::string refresh = RCV_CONTACT_LIST + username_;
    client_ptr->SendString(refresh);
}

void CHAT::on_SEND_clicked()
{
    if(ui->Status->currentText() != "OFFLINE")
    {
        if(ui->Status->currentText() != "BUSY")
        {
            if (ui->Companion->text() != "Companion")
            {
                if (ui->msg->text() != "")
                {
                    if(ui->AllContacts->selectedItems().size() > 1)
                    {
                        QString data = BROADCAST_MESSAGE + ui->NickName->text() + DELIMITER + QString::number(ui->AllContacts->selectedItems().size()) + DELIMITER;
                        QString receivers;
                        QList<QListWidgetItem*> selected_contacts = ui->AllContacts->selectedItems();
                        foreach (QListWidgetItem* contact, selected_contacts)
                        {
                            QString companion_name_and_state = contact->text();
                            QStringList companion = companion_name_and_state.split(END_OF_STR);
                            data += companion.at(0) + DELIMITER;
                            receivers += companion.at(0) + ',';
                        }
                        data += ui->msg->text();
                        receivers.resize(receivers.size() - 1);
                        ui->Companion->setText(receivers);
                        std::string converted_data = data.toUtf8().constData();
#ifdef QT_DEBUG
                        std::cout << converted_data << std::endl;
#endif
                        client_ptr->SendString(converted_data);
                        ui->msg->clear();
                    }
                    else
                    {
                        QString NickName = QString::fromUtf8(username_.c_str());
                        QString msg = MESSAGE + NickName + DELIMITER + ui->Companion->text() + DELIMITER + ui->msg->text();
                        std::string converted_msg = msg.toUtf8().constData();
                        client_ptr->SendString(converted_msg);
                        ui->msg->clear();
                    }
                }
            }
            else
            {
                QMessageBox::information(this, "Info", "Please select the recipient !");
            }
        }
        else
        {
            QMessageBox::information(this, "Info", "You cannot send a message when you are BUSY !");
        }
    }
    else
    {
        QMessageBox::information(this, "Info", "You cannot send a message when you are OFFLINE !");
    }
}
