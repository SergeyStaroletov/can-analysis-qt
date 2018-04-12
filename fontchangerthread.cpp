#include "fontchangerthread.h"
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QString>

#include "mainwindow.h"

FontChangerThread::FontChangerThread(MainWindow *window) {
  this->window = window;
  // connect signal
  QObject::connect(this, SIGNAL(clearMeCSS(QWidget *)), this->window,
                   SLOT(clearCSS(QWidget *)));
  isStopped = false;
}


FontChangerThread::~FontChangerThread() {
  QObject::disconnect(this, SIGNAL(clearMeCSS(QWidget *)), this->window,
                      SLOT(clearCSS(QWidget *)));
}

void FontChangerThread::run() {

  while (!isStopped) {


    for (int i = 0; i < nodeCount; i++) {
      OneCANDisplayNode *node = &(nodes[i]);
      QDateTime now = QDateTime::currentDateTime();

      // if data timed more than 5 second from now...
      if (node->isUsed && (node->lastTime.secsTo(now) > 5)) {
        // change the font to normal in the gui thread
        emit clearMeCSS(node->editData);
        emit clearMeCSS(node->editId);
        emit clearMeCSS(node->editDecode);
        emit clearMeCSS(node->checkIsRepeat);
        emit clearMeCSS(node->labelTime);
      }
    }

    QThread::msleep(500);
  }
}
