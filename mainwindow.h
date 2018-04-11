#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>

//forward
class QLineEdit;
class QLabel;
class QCheckBox;
class ReadFromComThread;
class FontChangerThread;

namespace Ui {
class MainWindow;
}

// each CAN UI line and internal data for it
struct OneCANDisplayNode {
  QLineEdit *editId;         // id of message
  QLineEdit *editData;       // message data
  QLineEdit *editDecode;     // ascii decode data
  QLabel *labelTime;         // label for show time diff after previous message
  QCheckBox *checkIsRepeat;  // on if the last message was repeated
  bool isUsed;            // isUsed = true if the data has already been filled
  unsigned char id;       // CAN message id
  unsigned char data[8];  // CAN data
  int dataLen;            // size of CAN data (0..7)
  QDateTime lastTime;     // last time of data modifycation
};

//bad but this is for not havig deal with extern/static const so on
#define nodeCountHalf 27            // count of data nodes in one column. up to 35@1440x900
#define nodeCount (nodeCountHalf * 2)  // total data nodes

extern  OneCANDisplayNode nodes[nodeCount];  // array to store all CAN data nodes.

//after it  - normal class declaration


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    inline Ui::MainWindow * getUI() {return this->ui;}

public slots:
    void processData(const QString & newData);
    void clearCSS(QWidget * widget);

private slots:
    void on_pushButton_clicked();

private slots:
    void on_pushButtonStart_clicked();
    
private:
    Ui::MainWindow *ui;
    ReadFromComThread *reader;    // reader thread object
    FontChangerThread *changer;    // reader thread object


    void createControls();
    void removeControls();
};

#endif // MAINWINDOW_H
