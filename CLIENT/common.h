#ifndef COMMON_H
#define COMMON_H

#define IP_ADDRESS       /*"172.20.8.135"*/         "127.0.0.1"               // server ip
#define PORT                      /*4547 */         1111                      // communication port

#define DELIMITER                       '#'
#define END_OF_STR                      '\n'
#define SEND_LOGIN_PASS                 '1'
#define SUCCESS_LOGIN                   "11"
#define ALREADY_IN_USE                  "12"
#define SEND_USERNAME_PASS              '2'
#define SUCCESS_CREATION_ACC            "21"
#define RCV_CONTACT_LIST                '3'
#define MESSAGE                         '4'
#define MESSAGE_SENT                    "41"
#define RECEIVER_BUSY                   "42"
#define RECEIVER_OFFLINE                "40"
#define RCV_MESSAGE                     '5'
#define STATUS_CHANGED                  '6'
#define REFRESH_CONTACTS                '7'

#endif // COMMON_H
