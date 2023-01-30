#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow  // inheriting from QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();


    void on_pretty_button_clicked();

    void on_ConvertJson_2_clicked();

    void on_Decompress_clicked();

    void on_Compress_clicked();

    void on_chooseFile_button_clicked();

    void on_InputText_textChanged();

    void on_minify_button_clicked();

    void on_toJson_clicked();

   void on_back_button_clicked();

    void on_CheckErrors_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
