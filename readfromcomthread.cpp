#include "readfromcomthread.h"

ReadFromComThread::ReadFromComThread(const QString &device) {
  this->device = device;
}

 ReadFromComThread::~ReadFromComThread()
{
}

bool ReadFromComThread::OpenPortik() {
  serial = new QSerialPort(this);
  serial->setPortName(device);

  serial->setBaudRate(115200);
  //todo: remove tdd
  return true;
  //return serial->open(QIODevice::ReadOnly);
}

void ReadFromComThread::run() {

  //todo: remove tdd
  //if (!serial->isOpen()) return;

  isStopped = false;

  QByteArray data;

  do {
    data.clear();

    bool ok = false;

    while (!ok) {

        //wtf?
      do {
        QByteArray data0;
        //todo: remove tdd
        data0.append("F1 ");
        data0.append("AA BB CC DD EE 11 22 \n");
        //data0 = serial->readAll();
        ok = data0.length() > 0;
        if (ok) data.append(data0);
        if (isStopped) break;
      } while (!ok);

      if (isStopped) return;

      ok = data.contains('\n');
      QThread::msleep(500);
    }

    // process data
    const QString sendMe = QString::fromStdString(data.toStdString());
    emit newDataSignal(sendMe);

  } while (!isStopped);
}
