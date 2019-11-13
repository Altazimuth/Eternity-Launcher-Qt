#include "config.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>

void SaveConfig()
{
   const QString cfgStr(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));
   if(cfgStr.isEmpty())
      return;

   QDir cfgDir(cfgStr);
   if(!cfgDir.exists())
   {
      if(QDir().mkpath(cfgStr))
      {
         // TODO: Error or warning or something
         return;
      }
   }

   const QString cfgFileStr = cfgDir.path() + "/config.cfg";

   QFile cfgFile(cfgFileStr);
   if(!cfgFile.open(QIODevice::WriteOnly))
   {
      // TODO: Error or warning or something
      return;
   }

   cfgFile.write("[IWADs]");

   // ...

   cfgFile.close();
}

void LoadConfig()
{
   const QString cfgStr(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));
   if(cfgStr.isEmpty())
      return;

   const QDir cfgDir(cfgStr);
   if(!cfgDir.exists())
      return;

   const QFileInfo cfgFilePath(cfgDir.path() + "/config.cfg");
   if(!cfgFilePath.exists())
      return;

   // ...
}
