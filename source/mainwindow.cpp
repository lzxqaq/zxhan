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
    QString filename = QFileDialog::getOpenFileName(this,"请选择需要压缩的文件",QDir::currentPath(),"All files(*.*)");
        string s_filename = filename.toStdString( );

        if(!filename.isNull()){
            QMessageBox::information(this,"压缩","选择成功",QMessageBox::Ok);
            QString dir = QFileDialog::getExistingDirectory(this,tr("选择压缩文件存放路径"),"./",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
            string dir2 = dir.toStdString();

            string s_filename1(s_filename.substr(s_filename.find_last_of("/")+1));
            string s_filename2 = s_filename1.substr(0,s_filename1.rfind("."));
            string s_fileSuffixname = s_filename1.substr(s_filename1.find_last_of('.')+1);
            string out_filename = dir2+"/"+s_filename2+"_"+s_fileSuffixname+".lzt";

            compress(s_filename, out_filename);

            QMessageBox::information(this,"压缩","压缩成功",QMessageBox::Ok);

        }else
            QMessageBox::information(this,"出错","没有选择文件",QMessageBox::Ok|QMessageBox::Cancel);

}

void MainWindow::on_pushButton_2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,"请选择需要解压缩的文件",QDir::currentPath(),"lztfiles(*.lzt)");
        if(!filename.isNull()){

            QString dir = QFileDialog::getExistingDirectory(this,tr("选择解压缩文件存放路径"),"./",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
            string dir2 = dir.toStdString();

            string s_filename = filename.toStdString();
            string s_filename1(s_filename.substr(s_filename.find_last_of("/")+1));
            string s_filename2 = s_filename1.substr(0,s_filename1.rfind("."));
            string s_filename3 = s_filename2.substr(s_filename2.find_last_of("_")+1);
            string s_filename4 = s_filename2.substr(0,s_filename2.rfind("_"));
            string out_filename = dir2+"/"+s_filename4+"."+s_filename3;

            decompress(s_filename, out_filename);

            QMessageBox::information(this,"解压","解压成功",QMessageBox::Ok);

        }
        else
            QMessageBox::information(this,"出错","没有选择文件",QMessageBox::Ok|QMessageBox::Cancel);


}
