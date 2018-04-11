#ifndef READFROMCOMTHREAD_H
#define READFROMCOMTHREAD_H

#include <QThread>
#include <QSerialPort>

class ReadFromComThread : public QThread
{
public:
    bool OpenPortik();
    ReadFromComThread(const QString & device);
    void run() override;


private:
    QString device;
    QSerialPort *serial;
    bool isStopped;
};

#endif // READFROMCOMTHREAD_H
