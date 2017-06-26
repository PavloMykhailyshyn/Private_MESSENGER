#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QDesktopWidget>
#include <QListWidget>
#include <QMessageBox>
#include <QTimer>
#include <sstream>
#include <boost/thread.hpp>
#include "clientsocket.h"
#include "common.h"
#include "messagesdlg.h"


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

    void THREAD_CREATE();
    void THREAD_DELETE();

    void CREATE_DLG();
    void CREATE_TIMER();

    void SetNameAndPass(const std::string&, const std::string&);

private slots:

    void on_GetContactList_clicked();

    void on_ChangeState_clicked();

    void on_Refresh_clicked();

    void on_SEND_clicked();

    void on_AllContacts_itemClicked(QListWidgetItem*);

private:

    Ui::CHAT * ui;

    boost::thread * thread_;

    std::string username_;
    std::string password_;

    MessagesDlg * dlg;
    QTimer * timer;

    void CLIENT_THREAD();
    void CLIENT_THREAD_MSG();

    bool ProcessString(char, std::string);

};

#endif // CHAT_H
