#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLayout>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QDateTime>


struct OneCANDisplayNode {

    QLineEdit *editId;          //id of message
    QLineEdit *editData;        //message data
    QLineEdit *editDecode;      //ascii decode data
    QLabel *labelTime;          //label for show time diff after previous message
    QCheckBox *checkIsRepeat;   //on if the last message was repeated

    char id;
    char data[8];
    QDateTime lastTime;

};

const int nodeCountHalf = 25;

OneCANDisplayNode nodes[nodeCountHalf * 2];


const int offsetX = 5;
const int offsetY = 40;


//create controls

void MainWindow::createControls() {

    int maxHeight = ui->groupBoxData->height() - offsetY;

    int oneHeight = maxHeight / nodeCountHalf;

    //first column
    for (int i = 0; i < nodeCountHalf; i++) {

        nodes[i].editId = new QLineEdit(ui->groupBoxData);
        nodes[i].editId->setGeometry(offsetX + ui->groupBoxData->x(),
                                     offsetY + ui->groupBoxData->y() + oneHeight * i,
                                     50, 21);
        this->layout()->addWidget(nodes[i].editId);

        nodes[i].editData = new QLineEdit(ui->groupBoxData);
        nodes[i].editData->setGeometry(offsetX + ui->groupBoxData->x() + 70,
                                     offsetY + ui->groupBoxData->y() + oneHeight * i,
                                     220, 21);
        this->layout()->addWidget(nodes[i].editData);

        nodes[i].editDecode = new QLineEdit(ui->groupBoxData);
        nodes[i].editDecode->setGeometry(offsetX + ui->groupBoxData->x() + 300,
                                     offsetY + ui->groupBoxData->y() + oneHeight * i,
                                     80, 21);
        this->layout()->addWidget(nodes[i].editDecode);

        nodes[i].labelTime = new QLabel("0s", ui->groupBoxData);
        nodes[i].labelTime->setGeometry(offsetX + ui->groupBoxData->x() + 390,
                                     offsetY + ui->groupBoxData->y() + oneHeight * i,
                                     50, 21);
        this->layout()->addWidget(nodes[i].labelTime);


        nodes[i].checkIsRepeat = new QCheckBox("Rep.", ui->groupBoxData);
        nodes[i].checkIsRepeat->setGeometry(offsetX + ui->groupBoxData->x() + 440,
                                     offsetY + ui->groupBoxData->y() + oneHeight * i,
                                     70, 21);
        this->layout()->addWidget(nodes[i].checkIsRepeat);
    }

    //second column

    const int addWidth = ui->groupBoxData->width() / 2;

    for (int i = 0; i < nodeCountHalf; i++) {

        nodes[i + nodeCountHalf].editId = new QLineEdit(ui->groupBoxData);
        nodes[i + nodeCountHalf].editId->setGeometry(offsetX + ui->groupBoxData->x() + addWidth,
                                     offsetY + ui->groupBoxData->y() + oneHeight * i,
                                     50, 21);
        this->layout()->addWidget(nodes[i + nodeCountHalf].editId);

        nodes[i + nodeCountHalf].editData = new QLineEdit(ui->groupBoxData);
        nodes[i + nodeCountHalf].editData->setGeometry(offsetX + ui->groupBoxData->x() + 70 + addWidth,
                                     offsetY + ui->groupBoxData->y() + oneHeight * i,
                                     220, 21);
        this->layout()->addWidget(nodes[i + nodeCountHalf].editData);

        nodes[i + nodeCountHalf].editDecode = new QLineEdit(ui->groupBoxData);
        nodes[i + nodeCountHalf].editDecode->setGeometry(offsetX + ui->groupBoxData->x() + 300 + addWidth,
                                     offsetY + ui->groupBoxData->y() + oneHeight * i,
                                     80, 21);
        this->layout()->addWidget(nodes[i + nodeCountHalf].editDecode);

        nodes[i + nodeCountHalf].labelTime = new QLabel("0s", ui->groupBoxData);
        nodes[i + nodeCountHalf].labelTime->setGeometry(offsetX + ui->groupBoxData->x() + 390 + addWidth,
                                     offsetY + ui->groupBoxData->y() + oneHeight * i,
                                     50, 21);
        this->layout()->addWidget(nodes[i + nodeCountHalf].labelTime);


        nodes[i + nodeCountHalf].checkIsRepeat = new QCheckBox("Rep.", ui->groupBoxData);
        nodes[i + nodeCountHalf].checkIsRepeat->setGeometry(offsetX + ui->groupBoxData->x() + 440 + addWidth,
                                     offsetY + ui->groupBoxData->y() + oneHeight * i,
                                     70, 21);
        this->layout()->addWidget(nodes[i + nodeCountHalf].checkIsRepeat);
    }





}

void MainWindow::removeControls() {

}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonStart_clicked()
{   
}



void MainWindow::on_pushButton_clicked()
{
    createControls();

}
