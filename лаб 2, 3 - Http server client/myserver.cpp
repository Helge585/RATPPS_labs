#include "myserver.h"
#include <QDataStream>
#include <iostream>

MyServer::MyServer(int n_port, QObject* pwgt/* = 0*/) : QObject(pwgt),
                                                        m_next_block_size(0)
{
    m_ptsp_server = new QTcpServer(this);
    if (!m_ptsp_server->listen(QHostAddress::Any, n_port)) {
        m_ptsp_server->close();
        return;
    }
    connect(m_ptsp_server, SIGNAL(newConnection()),
            this, SLOT(slotNewConnection()));
    is_finish_answer = false;
    task = "{\"K\":10,\"Sums\":[1.01,2.02],\"Muls\":[1,4]}";
}

void MyServer::slotNewConnection()
{
    QTcpSocket* client_socket = m_ptsp_server->nextPendingConnection();
    connect(client_socket, SIGNAL(disconnected()),
            client_socket, SLOT(deleteLater()));
    connect(client_socket, SIGNAL(readyRead()),
            this, SLOT(slotReadClient()));
}

void MyServer::slotReadClient()
{
    QTcpSocket* client_socket = (QTcpSocket*)sender();
    QDataStream in(client_socket);
    in.setVersion(QDataStream::Qt_5_3);
    for (;;) {
        if (!m_next_block_size) {
            if (client_socket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_next_block_size;
        }
        if (client_socket->bytesAvailable() < m_next_block_size) {
            break;
        }
        if (is_finish_answer) {
            in >> answer;
            std::cout << answer.toStdString() << std::endl;
        } else {
            QString cmd;
            in >> cmd;
            createAnswer(client_socket, cmd);
        }
        m_next_block_size = 0;
    }
}

void MyServer::createAnswer(QTcpSocket* p_socket, const QString& cmd)
{
    if (cmd == "Ping") {
        if (p_socket->state() == QTcpSocket::ConnectedState) {
            sendToClient(p_socket, "HttpStatusCode.Ok(200)");
        } else {
            sendToClient(p_socket, "HttpStatusCode.Error");
        }
    } else if (cmd == "GetInputData") {
        sendToClient(p_socket, task);
    } else if (cmd == "WriteAnswer") {
        is_finish_answer = true;
    }
}

void MyServer::sendToClient(QTcpSocket* p_socket, const QString& str)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out << quint16(0) << str;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    p_socket->write(arrBlock);
}

