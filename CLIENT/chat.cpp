#include "chat.h"
#include "ui_chat.h"


CHAT::CHAT(QWidget *parent) : QWidget(parent), ui(new Ui::CHAT)
{
    ui->setupUi(this);

    this->move(QApplication::desktop()->screen()->rect().center() - this->rect().center());

    //contacts_ = new QStringList();
    //listWidget_contacts = ui->MyContactList;
    //listWidget_allcontacts = ui->AllContacts;
    //connect(ui->AllContacts, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(ui->AddContact->se));
}

CHAT::~CHAT()
{
    pthread_cancel(thread);
    std::cout << "Thread closed" << std::endl;
    client_ptr->clientsocket::ResetInstance();
    delete ui;
}

void CHAT::THREAD_CREATE()
{
    pthread_create(&thread, nullptr, ClientThread, nullptr);
}

bool CHAT::ProcessString(char symbol, std::string str)
{
    switch (symbol)
    {
    case CONTACT_LIST:
    {
        str.erase(0, 1);
        //QStringList contacts;
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
            contacts_ += username_and_state + DELIMITER;
        }
        //QMetaObject::invokeMethod(win->ui->editText, "setText", Q_ARG(QString, "random string"));
       //QMetaObject::invokeMethod(ui->AllContacts, "clear");
        //contacts_ = contacts;
        //moveToThread(QApplication::instance()->thread());
        //ui->AllContacts->clear();
        //ui->AllContacts->addItems(contacts);
        //QCoreApplication::postEvent(chat, ui->AllContacts->clear());


        //std::cout << str << std::endl;
        break;
    }
    case STATUS_CHANGED:
    {

        break;
    }
    case REFRESH:
    {

        break;
    }
    default:
    {
        //std::cerr << "Unrecognized message type: " << symbol << "\n";
        break;
    }
    }
    return true;
}

void * CHAT::ClientThread(void *)
{
    std::cout << "Thread created" << std::endl;
    std::string str;
    bool p = true;
    while (p)
    {
        if (!client_ptr->GetString(str))
            break;

        if (!chat->ProcessString(static_cast<char>(str[0]), str))
            break;
    }
    std::cerr << "Lost connection to the server.\n";
    //pthread_cancel(chat->thread);
    //client_ptr->clientsocket::ResetInstance();
    return nullptr;
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
    //QMetaObject::invokeMethod(ui->AllContacts, "clear", Qt::AutoConnection);
    //QMetaObject::invokeMethod(ui->AllContacts, "addItems", Qt::AutoConnection, Q_ARG(QStringList, contacts));
    std::string get_list = RCV_CONTACT_LIST + username;
    client_ptr->SendString(get_list);
    QStringList contacts;
    std::istringstream stream(contacts_);
    std::string username_and_state;
    while (std::getline(stream, username_and_state, DELIMITER))
    {
        contacts += QString::fromUtf8(username_and_state.c_str());
    }
    //wait();
    ui->AllContacts->clear();
    ui->AllContacts->addItems(contacts);
    /*pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutex);
    std::string get_list = RCV_CONTACT_LIST + username;
    std::string str;
    client_ptr->SendString(get_list);
    client_ptr->GetString(str);
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
    pthread_mutex_unlock(&mutex);*/
}

void CHAT::on_AddContact_clicked()
{
    ui->MyContactList->addItem(ui->AllContacts->currentItem());
    //ui->AllContacts->
}
