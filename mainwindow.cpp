#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qapplication.h>
#include <qpushbutton.h>
#include <qfont.h>
#include    <QFileDialog>
#include    <QMessageBox>
#include "huffman.h"
#include<iostream>
#include<QTimer>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("憨憨压缩");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,"Please choose a file to compress",QDir::currentPath(),"All files(*.*)");
        string s_filename = filename.toStdString( );

        if(!filename.isNull()){
            QMessageBox::information(this,"Compress","Successfully choosed",QMessageBox::Ok);
            QString dir = QFileDialog::getExistingDirectory(this,tr("please choose a path to keep"),"/home",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
            string dir2 = dir.toStdString();

            string s_filename1(s_filename.substr(s_filename.find_last_of("/")+1));
            cout<<"s_f1 =   "<<s_filename1<<endl;
            string s_filename2 = s_filename1.substr(0,s_filename1.rfind("."));
            string s_fileSuffixname = s_filename1.substr(s_filename1.find_last_of('.')+1);
            string out_filename = dir2+"/"+s_filename2+"_"+s_fileSuffixname+".lzt";


            compress(s_filename, out_filename);
            QMessageBox::information(this,"Compress","Successfully compressed",QMessageBox::Ok);

        }else
            QMessageBox::information(this,"Document","Cannot find the document",QMessageBox::Ok|QMessageBox::Cancel);

}

void MainWindow::on_pushButton_2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,"Please choose a file to decompress",QDir::currentPath(),"lztfiles(*.lzt)");
        if(!filename.isNull()){
            QString dir = QFileDialog::getExistingDirectory(this,tr("Please choose the file you want to decompress"),"/home",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
            string dir2 = dir.toStdString();

            string s_filename = filename.toStdString();
            string s_filename1(s_filename.substr(s_filename.find_last_of("/")+1));
            string s_filename2 = s_filename1.substr(0,s_filename1.rfind("."));
            string s_filename3 = s_filename2.substr(s_filename2.find_last_of("_")+1);
            string s_filename4 = s_filename2.substr(0,s_filename2.rfind("_"));
            string out_filename = dir2+"/"+s_filename4+"."+s_filename3;

            decompress(s_filename, out_filename);
            QMessageBox::information(this,"Decompress","Successfully decompress",QMessageBox::Ok);

        }
        else
            QMessageBox::information(this,"Document","Cannot find the document",QMessageBox::Ok|QMessageBox::Cancel);


}
