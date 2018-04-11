#include "readfromcomthread.h"

ReadFromComThread::ReadFromComThread(const QString &device) {
  this->device = device;
}

bool ReadFromComThread::OpenPortik() {
  serial = new QSerialPort(this);
  serial->setPortName(device);

  serial->setBaudRate(115200);
  return serial->open(QIODevice::ReadOnly);
}

void ReadFromComThread::run() {
  if (!serial->isOpen()) return;

  isStopped = false;

  bool ok = false;
  QByteArray data;

  do {
    data.clear();

    while (!ok) {
      do {
        QByteArray data0;
        data0 = serial->readAll();
        ok = data0.length() > 0;
        if (ok) data.append(data0);
        if (isStopped) break;
        QThread::msleep(500);
      } while (ok);

      // qDebug() << data <<"\n";

      if (isStopped) return;

      ok = data.contains('\n');
    }

    // process data

  } while (!isStopped);
}
