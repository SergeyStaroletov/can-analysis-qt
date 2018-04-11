#ifndef READFROMCOMTHREAD_H
#define READFROMCOMTHREAD_H

#include <QThread>
#include <QSerialPort>

class ReadFromComThread : public QThread
{
    Q_OBJECT

public:
    bool OpenPortik();
    inline void Stop() {isStopped = true;}
    ReadFromComThread(const QString & device);
    virtual ~ReadFromComThread();
    void run() override;

signals:
    void newDataSignal(const QString & dataString);

private:
    QString device;
    QSerialPort *serial;
    bool isStopped;
};

#endif // READFROMCOMTHREAD_H
