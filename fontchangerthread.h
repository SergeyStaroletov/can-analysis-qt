#ifndef FONTCHANGERTHREAD_H
#define FONTCHANGERTHREAD_H

#include <QMainWindow>
#include <QThread>

class MainWindow;

class FontChangerThread : public QThread {
  Q_OBJECT
public:
  FontChangerThread(MainWindow *window);
  virtual ~FontChangerThread();
  void run() override;
  inline void Stop() { isStopped = true; }
signals:
  void clearMeCSS(QWidget *widget);

private:
  MainWindow *window;
  bool isStopped;
};

#endif // FONTCHANGERTHREAD_H
