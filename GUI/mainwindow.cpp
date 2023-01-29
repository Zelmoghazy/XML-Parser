#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QMessageBox"
#include "QString"
#include "QFileDialog"
#include "QIODevice"
#include "QTextStream"
#include "QFile"
#include "iostream"
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButton_3_clicked()
{

}

void MainWindow::on_pushButton_2_clicked()
{

}
void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::on_pushButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_pushButton_6_clicked()
{
    QApplication::quit();
}


/*
void MainWindow::on_pushButton_7_clicked()
{
        QString fileContent;
        QString filename= QFileDialog::getOpenFileName(this, "Choose File");
       if(filename.isEmpty())
           return;

       QFile file(filename);

       if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
           return;

       QTextStream in(&file);
       fileContent = in.readAll();
       file.close();
        ui->textEdit->clear();
       ui->textEdit->setPlainText(fileContent);
}
*/

void MainWindow::on_pretty_button_clicked()
{
    cout<<"Pretty button";
}


void MainWindow::on_ConvertJson_2_clicked()
{
    cout<<"convert to json  button"<<endl;
}


void MainWindow::on_Decompress_clicked()
{
      cout<<"decompress button"<<endl;
}


void MainWindow::on_Compress_clicked()
{
    cout<<"compress button"<<endl;

}


void MainWindow::on_chooseFile_button_clicked()
{
    QString fileContent;
    QString filename= QFileDialog::getOpenFileName(this, "Choose File");
   if(filename.isEmpty())
       return;

   QFile file(filename);

   if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
       return;

   QTextStream in(&file);
   fileContent = in.readAll();
   file.close();
   ui->InputText->clear();
   ui->InputText->setPlainText(fileContent);
}


void MainWindow::on_InputText_textChanged()
{

}

