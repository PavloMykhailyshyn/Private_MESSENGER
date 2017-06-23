#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QDesktopWidget>
#include <QListWidget>
#include <QMessageBox>
#include <QDebug>
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

    void on_AllContacts_clicked(const QModelIndex&);

    void on_ChangeState_clicked();

    void on_Refresh_clicked(); // need to implement

    void on_MyContactList_itemClicked(QListWidgetItem *item);

    void on_SEND_clicked();

private:
    Ui::CHAT *ui;

    boost::thread * thread_;

    std::string username_;
    std::string password_;

    void CLIENT_THREAD();

};

#endif // CHAT_H
