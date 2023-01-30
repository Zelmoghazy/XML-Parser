#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QMessageBox"
#include "QString"
#include "QFileDialog"
#include "QIODevice"
#include "QTextStream"
#include "QFile"
#include "iostream"
#include "xmlparser.h"
#include <fstream>
#include <fstream>
#include "compressionDecompression.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    cout<<ui->stackedWidget->currentIndex();
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

void MainWindow::on_pushButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    cout<<ui->stackedWidget->currentIndex();
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    cout<<ui->stackedWidget->currentIndex();
}


void MainWindow::on_pushButton_6_clicked()
{
    QApplication::quit();


}




// formating (prettify)
void MainWindow::on_pretty_button_clicked()
{

    cout<<ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(2);

    XMLDocument doc;
    // loading

    if(!XMLDocument_load(&doc,"C:\\Users\\hp\\Desktop\\Mans1611\\University\\Datas\\project\\GUI\\test.xml")){
        cout<<"fail"<<endl;
    }


    XMLDocument_write(&doc,"C:\\Users\\hp\\Desktop\\Mans1611\\University\\Datas\\project\\GUI\\out.xml",'f');
    XMLDocument_free (&doc);

       ui->screen3_label->setText("Pretitfied Json");

       QFile file("C:\\Users\\hp\\Desktop\\Mans1611\\University\\Datas\\project\\GUI\\out.xml");
       if(!file.open(QIODevice::ReadOnly)) {
           QMessageBox::information(0, "error", file.errorString());
       }
       QTextStream in(&file);
        QString line;
       while(!in.atEnd()) {
            line +=  in.readLine() + "\n";
            ui->prettified_label->setText(line);
       }
       file.close();


}


void MainWindow::on_ConvertJson_2_clicked()
{
    XMLDocument doc;

    // loading
    if(XMLDocument_load(&doc,"test.xml")){
        cout<<"succ"<<endl;
    }

}


void MainWindow::on_Decompress_clicked()
{
    decompress("C:\\Users\\hp\\Desktop\\Mans1611\\University\\Datas\\project\\GUI\\compressed.huf");
    ui->stackedWidget->setCurrentIndex(2);
    ui->screen3_label->setText("Decompressed File");
    XMLDocument doc;
    // loading


    if(!XMLDocument_load(&doc,"C:\\Users\\hp\\Desktop\\Mans1611\\University\\Datas\\project\\GUI\\decompressed.xml")){
        cout<<"fail"<<endl;
    }
    XMLDocument_write(&doc,"C:\\Users\\hp\\Desktop\\Mans1611\\University\\Datas\\project\\GUI\\decompressed.xml",'f');
    XMLDocument_free (&doc);

   QFile file("C:\\Users\\hp\\Desktop\\Mans1611\\University\\Datas\\project\\GUI\\decompressed.xml");
   if(!file.open(QIODevice::ReadOnly)) {
       QMessageBox::information(0, "error", file.errorString());
   }
   QTextStream in(&file);
   QString line;
   while(!in.atEnd()) {
        line +=  in.readLine() + "\n";
        ui->prettified_label->setText(line);
   }
   file.close();


}


void MainWindow::on_Compress_clicked()
{
    compress("C:\\Users\\hp\\Desktop\\Mans1611\\University\\Datas\\project\\GUI\\out.xml");
    ui->stackedWidget->setCurrentIndex(2);
   ui->screen3_label->setText("Compressed File");

   QFile file("C:\\Users\\hp\\Desktop\\Mans1611\\University\\Datas\\project\\GUI\\compressed.huf");
   if(!file.open(QIODevice::ReadOnly)) {
       QMessageBox::information(0, "error", file.errorString());
   }

   QTextStream in(&file);
   QString line;
   while(!in.atEnd()) {
        line +=  in.readLine() + "\n";
        ui->prettified_label->setText(line);
   }
   file.close();

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


void MainWindow::on_minify_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);

    XMLDocument doc;
    // loading

    if(!XMLDocument_load(&doc,"C:\\Users\\hp\\Desktop\\Mans1611\\University\\Datas\\project\\GUI\\test.xml")){
        cout<<"fail"<<endl;
    }

    XMLDocument_write(&doc,"C:\\Users\\hp\\Desktop\\Mans1611\\University\\Datas\\project\\GUI\\out.xml",'m');
    XMLDocument_free (&doc);


    ui->screen3_label->setText("Minfied XML");
    QFile file("C:\\Users\\hp\\Desktop\\Mans1611\\University\\Datas\\project\\GUI\\out.xml");
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }
    QTextStream in(&file);
    QString line;
    while(!in.atEnd()) {

        line += in.readLine();
        ui->prettified_label->setText(line);

    }

    file.close();


}


void MainWindow::on_toJson_clicked()
{
    //ui->stackedWidget->setCurrentIndex(4);

       XMLDocument doc;
       // loading
       cout<<XMLDocument_load(&doc,"test.xml");

       if(!XMLDocument_load(&doc,"C:\\Users\\hp\\Desktop\\Mans1611\\University\\Datas\\project\\GUI\\test.xml")){
           cout<<"fail"<<endl;
       }

       XMLDocument_write(&doc,"C:\\Users\\hp\\Desktop\\Mans1611\\University\\Datas\\project\\GUI\\out.json",'j');
       XMLDocument_free (&doc);

        ui->stackedWidget->setCurrentIndex(2);
       ui->screen3_label->setText("XML TO JSON");

       QFile file("C:\\Users\\hp\\Desktop\\Mans1611\\University\\Datas\\project\\GUI\\out.json");
       if(!file.open(QIODevice::ReadOnly)) {
           QMessageBox::information(0, "error", file.errorString());
       }

       QTextStream in(&file);
        QString line;
       while(!in.atEnd()) {
            line +=  in.readLine() + "\n";
            ui->prettified_label->setText(line);
       }

       file.close();
}


void MainWindow::on_back_button_clicked()
{
     ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_CheckErrors_clicked()
{

    XMLDocument doc;

    // loading
    if(!XMLDocument_load(&doc,"C:\\Users\\hp\\Desktop\\Mans1611\\University\\Datas\\project\\GUI\\test.xml")){
        cout<<"fail"<<endl;
    }
    XMLDocument_write(&doc,"C:\\Users\\hp\\Desktop\\Mans1611\\University\\Datas\\project\\GUI\\out.xml",'f');
    XMLDocument_free (&doc);


}

