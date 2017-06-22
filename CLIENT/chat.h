#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QDesktopWidget>
#include <QListWidget>
#include <sstream>
#include <pthread.h>
#include "clientsocket.h"
#include "common.h"

namespace Ui
{
class CHAT;
}

class CHAT : public QWidget
{
    Q_OBJECT

public:

    explicit CHAT(QWidget *parent = 0);
    ~CHAT();

    bool ProcessString(char, std::string);

    void THREAD_CREATE();

    void SetNameAndPass(const std::string&, const std::string&);

    /*QListWidget *listWidget_contacts;
    QListWidget *listWidget_allcontacts;*/

private slots:
    void on_GetContactList_clicked();

    void on_AddContact_clicked();

private:
    Ui::CHAT *ui;
    pthread_t thread;
    static void * ClientThread(void *);
    enum MSG_TYPE : char { CONTACT_LIST = '3', MSG_TO_GROUP = '2', STATUS_CHANGED = '5', REFRESH = '4' };

    std::string contacts_;
    std::string username;
    std::string password;

};

static CHAT * chat = nullptr;

#endif // CHAT_H
