#include "clientthread.h"

ClientThread::ClientThread()
{
    connect(&socket, SIGNAL(readyRead()), this, SLOT(onData()));
    connect(&socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError()));
    blockSize = 0;
    socket.abort();
    socket.connectToHost(QHostAddress::LocalHost, 1234);
    qDebug() << socket.error();
}

void ClientThread::run()
{
}

void ClientThread::onData()
{
    QString s;
    while(socket.bytesAvailable() > 0) {
        QString result = socket.readLine();
        s += result;
    }

//    if(s.compare("\"on season change\"") == 0) {
        emit seasonChangeSignal();
//    }
    qDebug() << "read :" << s;
}

void ClientThread::onError()
{
    qDebug() << "error";
}


