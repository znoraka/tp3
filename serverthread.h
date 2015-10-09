#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QtCore>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QNetworkInterface>
#include <QObject>
#include <QVector>

class ServerThread : public QObject
{
    Q_OBJECT
public:
    ServerThread();
    void run();

public slots:
    void onNewClient();
    void onSeasonChangeRequest();

private:
    QTcpServer server;
    QVector<QTcpSocket*> clients;
};

#endif // SERVERTHREAD_H
