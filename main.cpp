#include "mainwindow.h"
#include "files.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   MainWindow w;

   if(GetExePath().isEmpty())
   {
      QMessageBox::critical(&w, "Eternity Launcher", "Failed to find Eternity Engine executable.");
      return 1;
   }

   w.show();
   w.postDisplayConfig();
   return a.exec();
}
