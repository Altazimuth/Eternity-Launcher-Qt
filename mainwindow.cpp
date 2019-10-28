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

//=============================================================================
//
// Common event code
//

void MainWindow::addIWAD()
{
   const QString fileStr = QFileDialog::getOpenFileName(
      this, tr("Open File"), QString(), tr("DOOM Game Files (*.wad *.iwad *.pke)")
   );
   if(ui->comboBox_IWAD->count() == 0)
      ui->comboBox_IWAD->insertItem(0, fileStr);
   else
      ui->comboBox_IWAD->addItem(fileStr);
}

void MainWindow::removeIWAD()
{
   if(ui->comboBox_IWAD->count() != 0 && ui->comboBox_IWAD->currentIndex() != -1)
      ui->comboBox_IWAD->removeItem(ui->comboBox_IWAD->currentIndex());
}

void MainWindow::addFile()
{
   const QString fileStr = QFileDialog::getOpenFileName(
      this, tr("Open File"), QString(), tr("DOOM Game Files (*.wad *.pke)")
   );
   ui->listWidget_files->addItem(fileStr);
}

void MainWindow::removeFile()
{
   const auto currItem = ui->listWidget_files->currentItem();
   if(currItem != nullptr)
      delete currItem;
}

void MainWindow::openWiki()
{
   QUrl eternityWikiURL("http://eternity.youfailit.net/wiki/Main_Page");
   QDesktopServices::openUrl(eternityWikiURL);
}

//=============================================================================
//
// Events (slots)
//

void MainWindow::on_pushButton_addIWAD_released()    { addIWAD(); }
void MainWindow::on_pushButton_removeIWAD_released() { removeIWAD(); }

void MainWindow::on_pushButton_addFile_released()    { addFile(); }
void MainWindow::on_action_addFile_triggered()       { addFile(); }
void MainWindow::on_pushButton_removeFile_released() { removeFile(); }
void MainWindow::on_action_removeFile_triggered()    { removeFile(); }

void MainWindow::on_pushButton_wiki_released() { openWiki(); }
void MainWindow::on_actionEternity_Wiki_triggered() { openWiki(); }
