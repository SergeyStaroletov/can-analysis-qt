#ifndef FONTCHANGERTHREAD_H
#define FONTCHANGERTHREAD_H

#include <QMainWindow>
#include <QThread>

namespace Ui {
class MainWindow;
}


class FontChangerThread : public QThread
{
public:
    FontChangerThread(Ui::MainWindow *window);


private:
    Ui::MainWindow *window;
};

#endif // FONTCHANGERTHREAD_H
