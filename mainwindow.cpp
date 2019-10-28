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

void MainWindow::updateParams()
{
   QStringList argsList;
   QPlainTextEdit *const argBox = ui->plainTextEdit_commandLine;

   if(ui->comboBox_IWAD->currentIndex() != -1)
      argsList.append("-iwad " + ui->comboBox_IWAD->currentText());

   if(ui->listWidget_files->count())
   {
      QString filesArgStr("-files");
      for(int i = 0; i < ui->listWidget_files->count(); i++)
      {
         const QListWidgetItem *const item = ui->listWidget_files->item(i);
         filesArgStr += " " + item->text();
      }
      argsList.append(filesArgStr);
   }

   if(!ui->lineEdit_difficulty->text().isEmpty())
      argsList.append("-skill " + ui->lineEdit_difficulty->text());
   if(!ui->lineEdit_warp->text().isEmpty())
      argsList.append("-warp " + ui->lineEdit_warp->text());

   if(ui->checkBox_respawnMonsters->isChecked())
      argsList.append("-respawn");
   if(ui->checkBox_fastMonsters->isChecked())
      argsList.append("-fast");
   if(ui->checkBox_noMonsters->isChecked())
      argsList.append("-nomonsters");
   if(ui->checkBox_vanilla->isChecked())
      argsList.append("-vanilla");

   if(!ui->lineEdit_demoPlay->text().isEmpty())
      argsList.append("-record " + ui->lineEdit_demoPlay->text());

   // Actually write results
   argBox->clear();
   for(const QString &str : argsList)
      argBox->appendPlainText(str + "\n");
}

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

void MainWindow::openURL(const QString &urlStr)
{
   QUrl eternityWikiURL(urlStr);
   QDesktopServices::openUrl(eternityWikiURL);
}

//=============================================================================
//
// Events (slots)
//

void MainWindow::on_pushButton_addIWAD_released()    { addIWAD(); }
void MainWindow::on_action_addWAD_triggered()        { addIWAD(); }
void MainWindow::on_pushButton_removeIWAD_released() { removeIWAD(); }
void MainWindow::on_action_removeIWAD_triggered()    { removeIWAD(); }

void MainWindow::on_pushButton_addFile_released()    { addFile(); }
void MainWindow::on_action_addFile_triggered()       { addFile(); }
void MainWindow::on_pushButton_removeFile_released() { removeFile(); }
void MainWindow::on_action_removeFile_triggered()    { removeFile(); }

void MainWindow::on_pushButton_wikiCommandArgs_released() { openURL("http://eternity.youfailit.net/index.php?title=List_of_command_line_parameters"); }
void MainWindow::on_actionEternity_wiki_triggered()       { openURL("http://eternity.youfailit.net/wiki/Main_Page"); }

void MainWindow::on_pushButton_startGame_released()
{
   // Run EE w/ the appropriate args then kill this application
}
