#include "serverthread.h"

ServerThread::ServerThread()
{
    if (!server.listen(QHostAddress::LocalHost, 1234)) {
        qDebug() << "Unable to start the server";
        return;
    }
    QString ipAddress;
    ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    qDebug() << "address = " << server.serverAddress() << "\nport = " << server.serverPort();
    connect(&server, SIGNAL(newConnection()), this, SLOT(onNewClient()));
}

void ServerThread::run()
{
    forever {
    }
}

void ServerThread::onNewClient()
{
    qDebug() << "new client connected!";
    QTcpSocket *clientConnection = server.nextPendingConnection();
    clients.push_back(clientConnection);
}

void ServerThread::onSeasonChangeRequest()
{
    qDebug() << "button pressed";
    QString s = "change season please";
       foreach (QTcpSocket *socket, clients) {
        socket->write(s.toUtf8());
        socket->flush();
    }
}
