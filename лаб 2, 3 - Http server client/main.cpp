#include <QCoreApplication>
#include <iostream>
#include "myserver.h"
#include "myclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString rq = " http://127.0.0.1:";
    int port;
    std::cin >> port;
    MyServer server(port);
    rq = rq + QString::number(port);
    MyClient client(rq);
    client.Get();
    return a.exec();
}
