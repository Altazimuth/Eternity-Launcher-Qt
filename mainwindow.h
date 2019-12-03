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
   void postDisplayConfig();

   QStringList getIWADs() const;

private:
   static void openURL(const QString &urlStr);
   static void openFolder(const QString &path);

   void SaveConfig();
   void LoadConfig();

private slots:
   void updateParams();
   void addIWAD();
   void removeIWAD();
   void addFile();
   void removeFile();
   static void openBase();
   static void openUser();

   void on_toolButton_wikiCommandArgs_released();
   void on_actionEternity_wiki_triggered();

   void on_pushButton_warp_choose_released();
   void on_pushButton_warp_clear_released();

   void on_pushButton_viewDemo_choose_released();
   void on_pushButton_viewDemo_clear_released();

   void on_pushButton_network_clear_released();

   void on_pushButton_startGame_released();

private:
   Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
