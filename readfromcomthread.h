#ifndef READFROMCOMTHREAD_H
#define READFROMCOMTHREAD_H

#include <QSerialPort>
#include <QString>
#include <QThread>

class ReadFromComThread : public QThread {
  Q_OBJECT

public:
  bool OpenPortik();
  bool ClosePortik();

  bool SetParams(bool busTypeHS, int speed,
                 QString &outputMsg); // set params, see arduino sketch
  inline void Stop() { isStopped = true; }
  ReadFromComThread(const QString &device);
  virtual ~ReadFromComThread();
  void run() override;

signals:
  void newDataSignal(const QString &dataString);

private:
  QString device;
  QSerialPort *serial;
  bool isStopped;
};

#endif // READFROMCOMTHREAD_H
