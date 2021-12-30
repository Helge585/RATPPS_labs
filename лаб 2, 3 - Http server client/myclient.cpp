#include "myclient.h"
#include <QDataStream>
#include <iostream>

MyClient::MyClient(const QString& rq,  QObject* pwgt/* = 0 */) : QObject(pwgt),
                                                                 m_next_block_size(0)
{
    m_pTcpsocket = new QTcpSocket(this);
    int l = rq.indexOf("//");
    int r = rq.lastIndexOf(":");
    QString strHost = rq.mid(l + 2, r - l - 2);
    l = r;
    QString str_port = rq.mid(l + 1);
    int n_port = str_port.toInt();
   // std::cout << strHost.toStdString() << " " << n_port << " " << method.toStdString() << std::endl;
    m_pTcpsocket->connectToHost(strHost, n_port);
    connect(m_pTcpsocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
}

void MyClient::Get()
{
    last_query = "Ping";
    SendToServer("Ping");
}

void MyClient::slotReadyRead()
{
    QDataStream in(m_pTcpsocket);
    in.setVersion(QDataStream::Qt_5_3);
    for (;;) {
        if (!m_next_block_size) {
            if (m_pTcpsocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_next_block_size;
        }
        if (m_pTcpsocket->bytesAvailable() < m_next_block_size) {
            break;
        }
        in >> server_answer;
        //std::cout << server_answer.toStdString() << std::endl;
        m_next_block_size = 0;
        std::cout << server_answer.toStdString() << std::endl;
    }
    if (last_query == "Ping") {
        if (server_answer == "HttpStatusCode.Ok(200)") {
            last_query = "GetInputData";
            SendToServer("GetInputData");
        } else {
            std::cout << "Server isn't avalaible" << std::endl;
        }
    } else if (last_query == "GetInputData") {
        last_query = "GetInputData";
        converter.setJsonData(server_answer.toStdString());
        SendToServer("WriteAnswer");
        //std::cout << converter.getJsonData() << std::endl;
        SendToServer(QString::fromStdString(converter.getJsonData()));
    }
}

void MyClient::SendToServer(const QString & data)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    out << quint16(0);
    out << data;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    m_pTcpsocket->write(arrBlock);
    //m_pTcpsocket->waitForReadyRead(1000);
}

