#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopServices>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindow)
{
   ui->setupUi(this);

   // Set up events that use common functions
   connect(ui->pushButton_addIWAD,    SIGNAL(released()),  this, SLOT(addIWAD()));
   connect(ui->action_addIWAD,        SIGNAL(triggered()), this, SLOT(addIWAD()));
   connect(ui->pushButton_removeIWAD, SIGNAL(released()),  this, SLOT(removeIWAD()));
   connect(ui->action_removeIWAD,     SIGNAL(triggered()), this, SLOT(removeIWAD()));

   connect(ui->pushButton_addFile,    SIGNAL(released()),  this, SLOT(addFile()));
   connect(ui->action_addFile,        SIGNAL(triggered()), this, SLOT(addFile()));
   connect(ui->pushButton_removeFile, SIGNAL(released()),  this, SLOT(removeFile()));
   connect(ui->action_removeFile,     SIGNAL(triggered()), this, SLOT(removeFile()));

   // The infinite things that update params. It's sloppy and expensive (relatively) but it works.
   connect(ui->comboBox_IWAD, SIGNAL(currentIndexChanged(int)), this, SLOT(updateParams()));

   connect(ui->lineEdit_difficulty, SIGNAL(textChanged(QString)), this, SLOT(updateParams()));
   connect(ui->lineEdit_warp,       SIGNAL(textChanged(QString)), this, SLOT(updateParams()));

   connect(ui->checkBox_respawnMonsters, SIGNAL(stateChanged(int)), this, SLOT(updateParams()));
   connect(ui->checkBox_fastMonsters,    SIGNAL(stateChanged(int)), this, SLOT(updateParams()));
   connect(ui->checkBox_noMonsters,      SIGNAL(stateChanged(int)), this, SLOT(updateParams()));
   connect(ui->checkBox_vanilla,         SIGNAL(stateChanged(int)), this, SLOT(updateParams()));
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

   // Non-tab stuff
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

   // "Warp" tab
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

   // "View Demo" tab


   // "Network" tab

   // Actually write results
   argBox->clear();
   for(const QString &str : argsList)
      argBox->appendPlainText(str); // Adds newline by default
}

void MainWindow::addIWAD()
{
   const QString fileStr = QFileDialog::getOpenFileName(
      this, tr("Open File"), QString(), tr("DOOM Game Files (*.wad *.iwad *.pke)")
   );
   if(fileStr.size() == 0)
      return;
   if(ui->comboBox_IWAD->count() == 0)
      ui->comboBox_IWAD->insertItem(0, fileStr);
   else
      ui->comboBox_IWAD->addItem(fileStr);

   updateParams();
}

void MainWindow::removeIWAD()
{
   if(ui->comboBox_IWAD->count() != 0 && ui->comboBox_IWAD->currentIndex() != -1)
      ui->comboBox_IWAD->removeItem(ui->comboBox_IWAD->currentIndex());

   updateParams();
}

void MainWindow::addFile()
{
   const QString fileStr = QFileDialog::getOpenFileName(
      this, tr("Open File"), QString(), tr("DOOM Game Files (*.wad *.pke)")
   );
   ui->listWidget_files->addItem(fileStr);

   updateParams();
}

void MainWindow::removeFile()
{
   const auto currItem = ui->listWidget_files->currentItem();
   if(currItem != nullptr)
      delete currItem;

   updateParams();
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

void MainWindow::on_pushButton_wikiCommandArgs_released() { openURL("http://eternity.youfailit.net/index.php?title=List_of_command_line_parameters"); }
void MainWindow::on_actionEternity_wiki_triggered()       { openURL("http://eternity.youfailit.net/wiki/Main_Page"); }

void MainWindow::on_pushButton_startGame_released()
{
   // Run EE w/ the appropriate args then kill this application
}
