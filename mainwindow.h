#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   MainWindow(QWidget *parent = nullptr);
   ~MainWindow();

private:
   void addIWAD();
   void removeIWAD();
   void addFile();
   void removeFile();
   void openWiki();

private slots:
   void on_pushButton_addIWAD_released();
   void on_action_addWAD_triggered();
   void on_pushButton_removeIWAD_released();
   void on_action_removeIWAD_triggered();

   void on_pushButton_addFile_released();
   void on_action_addFile_triggered();
   void on_pushButton_removeFile_released();
   void on_action_removeFile_triggered();

   void on_pushButton_wiki_released();
   void on_actionEternity_Wiki_triggered();

private:
   Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
