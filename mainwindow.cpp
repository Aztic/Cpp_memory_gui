#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "maintable.h"
#include <QSpinBox>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("memory"));
    this->ui->WaysQu->setMaximum(2048);
    this->ui->WordsQu->setMaximum(2048);
    this->ui->BlockSiz->setMaximum(2048);
    this->ui->WaysQu->setMinimum(1);
    this->ui->WordsQu->setMinimum(1);
    this->ui->BlockSiz->setMinimum(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::is_valid(int ways, int words, int block_size){
    int words_per_way = words/ways;
    return words%ways==0 && block_size <= words_per_way && words_per_way%block_size==0;
}

void MainWindow::on_pushButton_clicked()
{

    int ways = this->ui->WaysQu->value();
    int words = this->ui->WordsQu->value();
    int block_size = this->ui->BlockSiz->value();
    bool totally = this->ui->TotallyAssoCheckBock->isChecked();
    if(is_valid(ways,words,block_size)){
        MainTable m(ways,words,block_size,totally);
        /* Validations go here */
        m.setModal(true);
        m.exec();
    }

}

void MainWindow::on_actionExit_triggered()
{
    QCoreApplication::quit();
}
