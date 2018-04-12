#include "readfromcomthread.h"
#include <QCoreApplication>
#include <QDebug>

QString generator() {
  QString result;

  for (int i = 0; i <= 9; i++) {
    int c = qrand() % 256;
    QString cStr = QString("%1").arg(c, 0, 16).toUpper();
    if (cStr.length() > 2) cStr = cStr.right(2);
    result += cStr + " ";
  }

  result += "\n";

  return result;
}

ReadFromComThread::ReadFromComThread(const QString &device) {
  this->device =  device;
}

ReadFromComThread::~ReadFromComThread() { serial->close(); }

bool ReadFromComThread::SetParams(bool busTypeHS, int speed,
                                  QString &outputMsg) {
  // data to send
  unsigned char sendData[4] = {0x19, 0x84, 0, 0};
  // add speed
  switch (speed) {
    case 100:
      sendData[3] = 0;
      break;
    case 125:
      sendData[3] = 1;
      break;
    case 250:
      sendData[3] = 2;
      break;
    case 500:
      sendData[3] = 3;
      break;
    case 1000:
      sendData[3] = 4;
      break;
    default:
      outputMsg = "Only 100,125,250,500,1000 KBPS speed is supported";
      return false;
  }
  // add type
  if (busTypeHS)
    sendData[2] = 0;
  else
    sendData[2] = 1;

  for (int i = 0; i < 1; i++) {
    const char *sending = reinterpret_cast<char *>(sendData);

    QByteArray resp;

    // waiting Arduino to be ready
    outputMsg = "";
    do {
      QCoreApplication::processEvents();
      if (serial->bytesAvailable() > 30) {
        QByteArray data = serial->readAll();
        if (data.length() > 0) resp.append(data);
        outputMsg = QString::fromStdString(resp.toStdString());
      }
    } while (outputMsg.indexOf("\n") < 0);

    // sending setup data buffer
    serial->write(QByteArray::fromRawData(sending, sizeof(sendData)));
    serial->waitForBytesWritten(100);
    QCoreApplication::processEvents();
    serial->flush();
    serial->waitForReadyRead(100);

    // waiting the reply
    resp.clear();
    do {
      QCoreApplication::processEvents();
      // if (serial->bytesAvailable() > 0) {
      QByteArray data = serial->readAll();

      if (data.length() > 0) {
        resp.append(data);
        qDebug() << data;
      }
      outputMsg = QString::fromStdString(resp.toStdString());


    } while ((outputMsg.indexOf("init ok") < 0) &&
             (outputMsg.indexOf("init fail") < 0));

    serial->flush();

    if (outputMsg.indexOf("init ok") > 0) {
      return true;
    }

   // QThread::msleep(1000);
  }

  return false;
}

bool ReadFromComThread::OpenPortik() {
  serial = new QSerialPort(this);
  serial->setPortName(device);

  serial->setBaudRate(115200);
  serial->setDataBits(QSerialPort::Data8);
  serial->setParity(QSerialPort::NoParity);
  serial->setStopBits(QSerialPort::OneStop);
  serial->setFlowControl(QSerialPort::NoFlowControl);

  // return true;
  return serial->open(QIODevice::ReadWrite);
}

void ReadFromComThread::run() {
  // todo: remove tdd
  // if (!serial->isOpen()) return;

  isStopped = false;

  QByteArray data;

  do {
    data.clear();

    bool ok = false;

    while (!ok) {
      // wtf?
      do {
        QByteArray data0;
        // todo: remove tdd
        data0.append(generator());
        // data0.append("F1 ");
        // data0.append("AA BB CC DD EE 11 22 \n");
        // data0 = serial->readAll();
        ok = data0.length() > 0;
        if (ok) data.append(data0);
        if (isStopped) break;
      } while (!ok);

      if (isStopped) return;

      ok = data.contains('\n');
      QThread::msleep(100);
    }

    // process data
    const QString sendMe = QString::fromStdString(data.toStdString());
    emit newDataSignal(sendMe);

  } while (!isStopped);
}
