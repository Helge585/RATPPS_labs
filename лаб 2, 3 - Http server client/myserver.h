#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QTcpSocket>


class MyServer : public QObject
{
Q_OBJECT
private:
    QTcpServer* m_ptsp_server;
    quint16 m_next_block_size;
    bool is_finish_answer;
    QString answer, task;

private:
    void sendToClient(QTcpSocket* p_socket, const QString& str);
    void createAnswer(QTcpSocket* p_socket, const QString& cmd);

public:
    MyServer(int n_port, QObject* pwgt = 0);
    void Stop() { m_ptsp_server->close(); }

public slots:
    virtual void slotNewConnection();
    void slotReadClient();
};

#endif // MYSERVER_H
