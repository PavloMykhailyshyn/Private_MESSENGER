#include "chat.h"
#include "ui_chat.h"


CHAT::CHAT(QWidget *parent) : QWidget(parent), ui(new Ui::CHAT)
{
    std::cout << "CHAT created" << std::endl;

    ui->setupUi(this);

    this->move(QApplication::desktop()->screen()->rect().center() - this->rect().center());

    //connect(ui->AllContacts, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(ui->AddContact->se));
}

CHAT::~CHAT()
{
    delete ui;
    std::cout << "deleted CHAT" << std::endl;
}

void CHAT::THREAD_CREATE()
{

    thread = new boost::thread(boost::bind(&CHAT::CLIENT_THREAD, this));
}

void CHAT::THREAD_DELETE()
{
    delete thread;
    std::cout << "deleted Thread" << std::endl;
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
                username_and_state += "\n| busy    |\n";
                username_and_state = "  " + username_and_state;
                break;
            }
            contacts += QString::fromUtf8(username_and_state.c_str());
        }
        ui->AllContacts->clear();
        ui->AllContacts->addItems(contacts);

        break;
    }
    case MSG_TO_SMB:
    {
        break;
    }
    case STATUS_CHANGED:
    {

        break;
    }
    case REFRESH_CONTACTS:
    {

        break;
    }
    default:
    {
        std::cerr << "Unrecognized message type: " << symbol << "\n";
        break;
    }
    }
    return true;
}

void CHAT::CLIENT_THREAD()
{
    std::cout << "Thread created" << std::endl;
    std::string str;
    bool p = true;
    while (p)
    {
        if (!client_ptr->GetString(str))
            break;

        if (!ProcessString(static_cast<char>(str[0]), str))
            break;
    }
    std::cerr << "Lost connection to the server.\n";
}

void CHAT::SetNameAndPass(const std::string& username, const std::string& password)
{
    this->username = username;
    this->password = password;
    QString NickName = QString::fromUtf8(username.c_str());
    ui->NickName->setText(NickName);
}


void CHAT::on_GetContactList_clicked()
{
    std::string get_list = RCV_CONTACT_LIST + username;
    client_ptr->SendString(get_list);
}

void CHAT::on_AddContact_clicked()
{
    ui->MyContactList->addItem(ui->AllContacts->currentItem());
}
