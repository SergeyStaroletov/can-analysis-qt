#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCheckBox>
#include <QDateTime>
#include <QDebug>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QMap>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QString>

#include "fontchangerthread.h"
#include "readfromcomthread.h"

OneCANDisplayNode nodes[nodeCount];  // array to store all CAN data nodes.
QMap<unsigned char, OneCANDisplayNode *> nodeById;  // map CAN id -> data node

const int offsetX = 5;  // offset to start drawind from the left of the groupBox
const int offsetY = 40;  // offset to start drawind from the top of the groupBox

/*
 * Create controls dynamically
 */
void MainWindow::createControls() {
  int maxHeight = ui->groupBoxData->height() - offsetY;

  int oneHeight = maxHeight / nodeCountHalf;  // logical window to draw each
                                              // node

  // first column
  for (int i = 0; i < nodeCountHalf; i++) {
    nodes[i].isUsed = false;
    nodes[i].dataLen = 0;

    nodes[i].editId = new QLineEdit(ui->groupBoxData);
    nodes[i].editId->setGeometry(
        offsetX + ui->groupBoxData->x(),
        offsetY + ui->groupBoxData->y() + oneHeight * i, 50, 21);
    this->layout()->addWidget(
        nodes[i].editId);  // I know it is not good. No time to think.

    nodes[i].editData = new QLineEdit(ui->groupBoxData);
    nodes[i].editData->setGeometry(
        offsetX + ui->groupBoxData->x() + 70,
        offsetY + ui->groupBoxData->y() + oneHeight * i, 220, 21);
    this->layout()->addWidget(nodes[i].editData);

    nodes[i].editDecode = new QLineEdit(ui->groupBoxData);
    nodes[i].editDecode->setGeometry(
        offsetX + ui->groupBoxData->x() + 300,
        offsetY + ui->groupBoxData->y() + oneHeight * i, 80, 21);
    this->layout()->addWidget(nodes[i].editDecode);

    nodes[i].labelTime = new QLabel("0s", ui->groupBoxData);
    nodes[i].labelTime->setGeometry(
        offsetX + ui->groupBoxData->x() + 390,
        offsetY + ui->groupBoxData->y() + oneHeight * i, 50, 21);
    this->layout()->addWidget(nodes[i].labelTime);

    nodes[i].checkIsRepeat = new QCheckBox("Rep.", ui->groupBoxData);
    nodes[i].checkIsRepeat->setGeometry(
        offsetX + ui->groupBoxData->x() + 440,
        offsetY + ui->groupBoxData->y() + oneHeight * i, 70, 21);
    this->layout()->addWidget(nodes[i].checkIsRepeat);
  }

  // second column
  const int addWidth =
      ui->groupBoxData->width() /
      2;  // x offset to move the column from the start of first one

  for (int i = 0; i < nodeCountHalf; i++) {
    nodes[i + nodeCountHalf].isUsed = false;
    nodes[i + nodeCountHalf].dataLen = 0;

    nodes[i + nodeCountHalf].editId = new QLineEdit(ui->groupBoxData);
    nodes[i + nodeCountHalf].editId->setGeometry(
        offsetX + ui->groupBoxData->x() + addWidth,
        offsetY + ui->groupBoxData->y() + oneHeight * i, 50, 21);
    this->layout()->addWidget(nodes[i + nodeCountHalf].editId);

    nodes[i + nodeCountHalf].editData = new QLineEdit(ui->groupBoxData);
    nodes[i + nodeCountHalf].editData->setGeometry(
        offsetX + ui->groupBoxData->x() + 70 + addWidth,
        offsetY + ui->groupBoxData->y() + oneHeight * i, 220, 21);
    this->layout()->addWidget(nodes[i + nodeCountHalf].editData);

    nodes[i + nodeCountHalf].editDecode = new QLineEdit(ui->groupBoxData);
    nodes[i + nodeCountHalf].editDecode->setGeometry(
        offsetX + ui->groupBoxData->x() + 300 + addWidth,
        offsetY + ui->groupBoxData->y() + oneHeight * i, 80, 21);
    this->layout()->addWidget(nodes[i + nodeCountHalf].editDecode);

    nodes[i + nodeCountHalf].labelTime = new QLabel("0s", ui->groupBoxData);
    nodes[i + nodeCountHalf].labelTime->setGeometry(
        offsetX + ui->groupBoxData->x() + 390 + addWidth,
        offsetY + ui->groupBoxData->y() + oneHeight * i, 50, 21);
    this->layout()->addWidget(nodes[i + nodeCountHalf].labelTime);

    nodes[i + nodeCountHalf].checkIsRepeat =
        new QCheckBox("Rep.", ui->groupBoxData);
    nodes[i + nodeCountHalf].checkIsRepeat->setGeometry(
        offsetX + ui->groupBoxData->x() + 440 + addWidth,
        offsetY + ui->groupBoxData->y() + oneHeight * i, 70, 21);
    this->layout()->addWidget(nodes[i + nodeCountHalf].checkIsRepeat);
  }
}

/*
 * Remove controls
 */
void MainWindow::removeControls() {
  for (int i = 0; i < nodeCount; i++) {
    this->layout()->removeWidget(nodes[i].editId);
    if (nodes[i].editId != NULL) delete nodes[i].editId;

    this->layout()->removeWidget(nodes[i].editData);
    if (nodes[i].editData != NULL) delete nodes[i].editData;

    this->layout()->removeWidget(nodes[i].editDecode);
    if (nodes[i].editDecode != NULL) delete nodes[i].editDecode;

    this->layout()->removeWidget(nodes[i].labelTime);
    if (nodes[i].labelTime != NULL) delete nodes[i].labelTime;

    this->layout()->removeWidget(nodes[i].checkIsRepeat);
    if (nodes[i].checkIsRepeat != NULL) delete nodes[i].checkIsRepeat;

    // memset(nodes, 0, sizeof(nodes));
  }
}

/*
 * Fill the com port combo box
 */
void MainWindow::FillComPortsBox() {
  ui->comboBoxPort->clear();
  int index = 0;
  Q_FOREACH (QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
    ui->comboBoxPort->addItem(port.portName());

    if (port.portName().indexOf("cu.w") >= 0) //for me
      ui->comboBoxPort->setCurrentIndex(index);

    index++;
  }
}

/*
 * Default constructor
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  // memset(nodes, 0, sizeof(nodes));
  changer = NULL;
  reader = NULL;
  FillComPortsBox();
}

MainWindow::~MainWindow() {
  if (changer) {
    changer->Stop();
    QThread::msleep(500);
    delete changer;
  }

  // delete old com port reader
  if (reader) {
    reader->Stop();
    QObject::disconnect(reader, SIGNAL(newDataSignal(const QString &)), this,
                        SLOT(processData(const QString &)));
    QThread::msleep(500);
    delete reader;
  }

  removeControls();

  delete ui;
}

void MainWindow::on_pushButtonStart_clicked() {}

/*
 *  Starts the analisys from GUI
 */
void MainWindow::on_pushButton_clicked() {
  // check the window size

  ui->groupBoxData->setGeometry(ui->groupBoxData->x(), ui->groupBoxData->y(),
                                this->width(), this->height() - 100);

  if (!changer) {
    changer = new FontChangerThread(this);
    changer->start();
  }

  // delete old com port reader
  if (reader) {
    reader->Stop();
    QObject::disconnect(reader, SIGNAL(newDataSignal(const QString &)), this,
                        SLOT(processData(const QString &)));
    QThread::sleep(500);
    delete reader;
    reader = NULL;
  }

  // new reader with device port
  reader = new ReadFromComThread(ui->comboBoxPort->currentText());

  // check the port
  if (!reader->OpenPortik()) {
    QMessageBox::warning(this, "Problem", "Cannot open port");
    return;
  }

  // setup MCP devices
  QString arduinoMsg = "Please select a correct car";
  bool result = false;

  if (ui->comboBoxVehicle->currentIndex() == 0) {
    // only mazda 6 for now

    if (ui->radioButtonHS->isChecked())
      result = reader->SetParams(true, 500, arduinoMsg);
    else
      result = reader->SetParams(false, 250, arduinoMsg);
  }

  if (!result) {
    QMessageBox::warning(this, "Problem", arduinoMsg);
    delete reader;
    reader = NULL;

    return;
  } else {
    QMessageBox::information(this, "Port setup correctly", arduinoMsg);
  }

  // create controls
  removeControls();
  createControls();

  // connect signal
  QObject::connect(reader, SIGNAL(newDataSignal(const QString &)), this,
                   SLOT(processData(const QString &)));

  // run loop
  reader->start();
}

/*
 * A helper to convert a char to its hex string representation
 * @param c - char (255)
 * @return hex upercase string (FF)
 */
QString char2HexStr(unsigned char c) {
  QString cStr = QString("%1").arg((int)c, 0, 16).toUpper();
  if (cStr.length() > 2) cStr = cStr.right(2);
  return cStr;
}

/*
 * Process data from string like FF 11 22 33 44 55 66 77 88
 * and updates UI. It is a slot for newDataSignal from reader.
 * @param newData - given string
 * @returns modifies nodes array, nodeById map
 */
void MainWindow::processData(const QString &newData) {
  // first: split to lines
  QStringList strings = newData.split("\n");
  for (int s = 0; s < strings.length(); s++) {
    // second: get bytes in array from each line
    QStringList bytes = strings.at(s).split(" ");
    int count = (bytes.length() <= 9) ? bytes.length() : 9;

    unsigned char converted[9];

    // convert to normal chars
    int realCount = 0;
    for (int i = 0; i < count; i++) {
      QString oneByteHex = bytes.at(i);

      bool bStatus = false;
      uint nHex = oneByteHex.toUInt(&bStatus, 16);
      if (!bStatus) continue;

      converted[realCount++] = (unsigned char)nHex;
    }
    count = realCount;

    if (count == 0) continue;  // skip ""

    // find corresponding node by id
    unsigned char id = converted[0];
    OneCANDisplayNode *node = NULL;
    if (nodeById.contains(id)) {
      node = nodeById[id];

    } else {
      // try to find a free node
      bool weFoundNext = false;
      int foundIndex = 0;
      for (int i = 0; i < nodeCount; i++) {
        if (!nodes[i].isUsed) {
          weFoundNext = true;
          foundIndex = i;
          break;
        }
      }
      // we found a free index
      if (weFoundNext) {
        node = &(nodes[foundIndex]);
        node->isUsed = true;
        node->id = id;
        node->lastTime = QDateTime::currentDateTime();

        nodeById.insert(id, node);

      } else {
        // find oldest data and remove it, we will be here
        qint64 maxDiff = -1;
        QDateTime now = QDateTime::currentDateTime();
        int oldestIndex = 0;
        for (int i = 0; i < nodeCount; i++) {
          qint64 currentDiff = nodes[i].lastTime.msecsTo(now);
          if (currentDiff > maxDiff) {
            maxDiff = currentDiff;
            oldestIndex = i;
          }
        }
        // so we change oldexIndex with some date to our new data with new id
        node = &(nodes[oldestIndex]);
        int oldId = node->id;
        nodeById.remove(oldId);
        node->id = id;
        nodeById.insert(id, node);
      }
    }

    if (!node) return;

    // set node id
    node->editId->setText(char2HexStr(id));
    node->editId->setStyleSheet("font-weight: bold");
    node->editId->setFocus();
    // set node time
    QDateTime now = QDateTime::currentDateTime();
    QString timeStr;
    qint64 diff = node->lastTime.msecsTo(now);
    if (diff < 10000)  // 9999ms max
      timeStr = QString::number(diff) + "ms";
    else {
      diff = node->lastTime.secsTo(now);
      if (diff <= 600)  // 10 minutes max
        timeStr = QString::number(diff) + "s";
      else {
        diff = node->lastTime.secsTo(now) / 60;
        timeStr = QString::number(diff) + "m";
      }
    }
    node->labelTime->setText(timeStr);
    node->lastTime = now;
    node->labelTime->setStyleSheet("font-weight: bold");

    // check data chagin: by size and by value
    bool dataChanged = false;

    if (node->dataLen != realCount - 1)
      dataChanged = true;
    else {
      for (int i = 0; i < node->dataLen; i++) {
        if (node->data[i] != converted[i + 1]) {
          dataChanged = true;
          break;
        }
      }
    }
    // fill numberic data
    // if (dataChanged)
    {
      QString dataStr;
      for (int i = 1; i < realCount; i++) {
        node->data[i - 1] = converted[i];
        if (!ui->checkBoxHex->isChecked())
          dataStr = dataStr + QString::number((int)converted[i]) + " ";
        else
          dataStr = dataStr + char2HexStr(converted[i]) + " ";
      }
      node->editData->setText(dataStr);
      node->editData->setStyleSheet("font-weight: bold");
      node->dataLen = realCount - 1;
    }

    node->checkIsRepeat->setChecked(!dataChanged);
    node->checkIsRepeat->setStyleSheet("font-weight: bold");

    // text data from char array
    if (dataChanged) {
      QString textRep;
      for (int i = 0; i < node->dataLen; i++) {
        // check if the symbol is printable
        if (node->data[i] >= 32)
          textRep = textRep + QString(QChar(node->data[i]));
        else
          textRep = textRep + ".";
      }
      node->editDecode->setText(textRep);
      node->editDecode->setStyleSheet("font-weight: bold");
    }
    QCoreApplication::processEvents();

  }  // for strings
}

/*
 * Update (clear) CSS for given widget. Slot for execute from different thread
 * @param wiget - it will be cleared
 */
void MainWindow::clearCSS(QWidget *widget) { widget->setStyleSheet(""); }
