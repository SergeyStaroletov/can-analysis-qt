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

const int nodeCountHalf = 20;

OneCANDisplayNode nodes[nodeCountHalf];


const int offsetX = 5;
const int offsetY = 40;


//create controls

void MainWindow::createControls() {

    int maxHeight = ui->groupBoxData->height() - offsetY;

    int oneHeight = maxHeight / nodeCountHalf;


    for (int i = 0; i < nodeCountHalf; i++) {

        nodes[i].editId = new QLineEdit(ui->groupBoxData);
        nodes[i].editId->setGeometry(offsetX + ui->groupBoxData->x(),
                                     offsetY + ui->groupBoxData->y() * oneHeight,
                                     50, 21);
        this->layout()->addWidget(nodes[i].editId);

        nodes[i].editData = new QLineEdit(ui->groupBoxData);
        nodes[i].editData->setGeometry(offsetX + ui->groupBoxData->x() + 70,
                                     offsetY + ui->groupBoxData->y() * oneHeight,
                                     220, 21);
        this->layout()->addWidget(nodes[i].editData);

        nodes[i].editDecode = new QLineEdit(ui->groupBoxData);
        nodes[i].editDecode->setGeometry(offsetX + ui->groupBoxData->x() + 300,
                                     offsetY + ui->groupBoxData->y() * oneHeight,
                                     80, 21);
        this->layout()->addWidget(nodes[i].editDecode);

        nodes[i].labelTime = new QLabel("0s", ui->groupBoxData);
        nodes[i].labelTime->setGeometry(offsetX + ui->groupBoxData->x() + 390,
                                     offsetY + ui->groupBoxData->y() * oneHeight,
                                     50, 21);
        this->layout()->addWidget(nodes[i].labelTime);


        nodes[i].checkIsRepeat = new QCheckBox("Repeat", ui->groupBoxData);
        nodes[i].checkIsRepeat->setGeometry(offsetX + ui->groupBoxData->x() + 440,
                                     offsetY + ui->groupBoxData->y() * oneHeight,
                                     70, 21);
        this->layout()->addWidget(nodes[i].checkIsRepeat);




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
