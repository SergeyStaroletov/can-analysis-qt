#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void processData(const QString & newData);

private slots:
    void on_pushButton_clicked();

private slots:
    void on_pushButtonStart_clicked();
    
private:
    Ui::MainWindow *ui;

    void createControls();
    void removeControls();
};

#endif // MAINWINDOW_H
