#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopServices>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindow)
{
   ui->setupUi(this);
}

MainWindow::~MainWindow()
{
   delete ui;
}


void MainWindow::on_pushButton_addIWAD_released()
{
   const QString fileStr = QFileDialog::getOpenFileName(
      this, tr("Open File"), QString(), tr("DOOM Game Files (*.wad *.iwad *.pke)")
   );
   if(ui->comboBox_IWAD->count() == 0)
      ui->comboBox_IWAD->insertItem(0, fileStr);
   else
      ui->comboBox_IWAD->addItem(fileStr);
}

void MainWindow::on_pushButton_removeIWAD_released()
{
   if(ui->comboBox_IWAD->count() != 0 && ui->comboBox_IWAD->currentIndex() != -1)
      ui->comboBox_IWAD->removeItem(ui->comboBox_IWAD->currentIndex());
}

void MainWindow::on_pushButton_addFile_released()
{
   const QString fileStr = QFileDialog::getOpenFileName(
      this, tr("Open File"), QString(), tr("DOOM Game Files (*.wad *.pke)")
   );
   ui->listWidget_files->addItem(fileStr);
}

void MainWindow::on_pushButton_removeFile_released()
{
   const auto currItem = ui->listWidget_files->currentItem();
   if(currItem != nullptr)
      delete currItem;
}
