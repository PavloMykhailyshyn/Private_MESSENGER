#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QDesktopWidget>
#include <QListWidget>
#include <QtConcurrent/QtConcurrent>
#include <sstream>
#include <boost/thread.hpp>
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
    void THREAD_DELETE();

    void SetNameAndPass(const std::string&, const std::string&);

private slots:

    void on_GetContactList_clicked();

    void on_AddContact_clicked();

private:
    Ui::CHAT *ui;

    //enum MSG_TYPE : char { CONTACT_LIST = '3', STATUS = '5', REFRESH = '4' };

    boost::thread * thread;

    std::string username;
    std::string password;

    void CLIENT_THREAD();

};

#endif // CHAT_H
