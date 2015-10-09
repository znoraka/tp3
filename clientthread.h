#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>
#include <QObject>

class ClientThread : public QObject
{
    Q_OBJECT
public:
    ClientThread();
    void run();

public slots:
    void onData();
    void onError();

signals:
    void seasonChangeSignal();

private:
    int blockSize;
    QTcpSocket socket;
};

#endif // CLIENTTHREAD_H
