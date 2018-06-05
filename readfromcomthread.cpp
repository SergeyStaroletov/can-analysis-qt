#include "readfromcomthread.h"
#include <QCoreApplication>
#include <QDateTime>
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
  this->device = device;
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
      outputMsg = "Only 100,125,250,500,1000 KBPS speeds are supported";
      return false;
  }
  // add type
  if (busTypeHS)
    sendData[2] = 0;
  else
    sendData[2] = 1;

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

  QThread::msleep(1000);
  serial->readAll();
  serial->flush();
  // sending setup data buffer
  serial->write(QByteArray::fromRawData(sending, sizeof(sendData)));
  serial->waitForBytesWritten(1000);
  QCoreApplication::processEvents();
  serial->flush();
  serial->waitForReadyRead(100);

  // waiting the reply
  QDateTime timeStart = QDateTime::currentDateTime();
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

    if (timeStart.secsTo(QDateTime::currentDateTime()) > 5) {
      outputMsg = "Init fail with timeout. Try again.";
      break;
    }

  } while ((outputMsg.indexOf("init ok") < 0) &&
           (outputMsg.indexOf("init fail") < 0));

  serial->flush();

  if (outputMsg.indexOf("init ok") > 0) {
    return true;
  }

  return false;
}

bool ReadFromComThread::ClosePortik() {

  // return true;
   if (serial) serial->close();
   return true;
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

  static int nTries;

  isStopped = false;

  QByteArray data;
  QString dataRep;
  QString dataToProcess;

  // pass all data to /n
  int indexOf = -1;
  do {
    QByteArray data0 = serial->readAll();
    if (data0.length() > 0) data.append(data0);
    dataRep = QString::fromStdString(data.toStdString());
      //qDebug() << dataRep << "\n";
    indexOf = dataRep.indexOf("\n");
  } while (indexOf < 0);

  // we get from \n to end (start of new substring)
  dataToProcess = dataRep.mid(indexOf + 1);

  data.clear();
  data.append(dataToProcess);

  do {
    QDateTime timeStartGetting = QDateTime::currentDateTime();
    QString sendMe;
    QThread::msleep(1);

    int runn = 0;
    do {
      QCoreApplication::processEvents();

      QThread::msleep(10);

      do {
        QThread::msleep(0);

        QByteArray data0 = serial->readAll();
        if (data0.length() > 0) data.append(data0);
        dataRep = QString::fromStdString(data.toStdString());
        indexOf = dataRep.indexOf("\n");
      } while (indexOf < 0);
      QString dataToProcessNext = dataRep.mid(indexOf + 1);

      data.clear();
      data.append(dataToProcessNext);

      // process data
      sendMe += dataRep;
      runn++;
    break;
    } while (timeStartGetting.msecsTo(QDateTime::currentDateTime()) <
             100);  // collect all data for not more than 100ms

   //qDebug() << dataRep << "\n";
    //send signal with data collected in 100ms period

    nTries++;

   if ((nTries % 10) == 0)
       emit newDataSignal(dataRep);

  } while (!isStopped);
}
