#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QTcpSocket>
#include "dataconverter.h"

class MyClient : public QObject
{
Q_OBJECT
private:
    QTcpSocket* m_pTcpsocket;
    quint16 m_next_block_size;
    QString last_query, server_answer;
    dataConverter converter;

public:
    MyClient(const QString& strHost, QObject* pwgt = 0);
    void SendToServer(const QString & cmd);
    void Get();

private slots:
    void slotReadyRead();
};

#endif // MYCLIENT_H
