// GPLv3 and shit
// Taken from d_files.cpp in the Eternity Engine
// Thanks to James Haley, and Ioan Chera whose code is used here
// I, Max Waine, am not gonna thank myself for the code there I used here. That's weird.

#include <QtGlobal>
#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>

//=============================================================================
//
// /base code
//

bool CheckBasePath(const QDir &path)
{
   QDirIterator itr(path);

   int score = 0;
   while(itr.hasNext())
   {
      const QString filename = itr.fileName();

      if(filename == "startup.wad")
         ++score;
      else if(filename == "root.edf")
         ++score;
      else if(filename == "doom")
         ++score;

      itr.next();
   }

   return score >= 3;
}

const char *PlatformInstallDirectory()
{
#ifdef Q_OS_LINUX
   struct stat sbuf;

   // Prefer /usr/local, but fall back to just /usr.
   if(!stat("/usr/local/share/eternity/base", &sbuf) && S_ISDIR(sbuf.st_mode))
      return "/usr/local/share/eternity/base";
   else
      return "/usr/share/eternity/base";
#endif

   return nullptr;
}

void SetBasePath()
{
   QDir basePath;

   if(qEnvironmentVariableIsSet("ETERNITYBASE"))
   {
      QDir eternityBase(qEnvironmentVariable("ETERNITYBASE"));
      if(eternityBase.exists() && CheckBasePath(eternityBase))
      {
         basePath = eternityBase;
         return;
      }
   }

   const char *s = PlatformInstallDirectory();
   if(s != nullptr)
   {
      const QDir platformInstallDir(s);
      if(CheckBasePath(platformInstallDir))
      {
         basePath = platformInstallDir;
         return;
      }
   }

#ifdef Q_OS_WIN
   QFileInfo eternityPath = QCoreApplication::applicationDirPath() + "/Eternity.exe";
#else
   QFileInfo eternityPath = QCoreApplication::applicationDirPath() + "/eternity";
#endif
   if(eternityPath.exists())
   {
      QDir exeBasePath = QCoreApplication::applicationDirPath() + QDir::separator() + "base";
      if(exeBasePath.exists() && CheckBasePath(exeBasePath))
      {
         basePath = exeBasePath;
         return;
      }
   }

   if(!basePath.exists())
   {
      QDir exeWorkingPath = QDir::currentPath() + QDir::separator() + "base";
      if(exeWorkingPath.exists() && CheckBasePath(exeWorkingPath))
      {
         basePath = exeWorkingPath;
         return;
      }
   }
}

//=============================================================================
//
// /user code
//

bool CheckUserPath(const QDir &path)
{
   QDirIterator itr(path);

   int score = 0;
   while(itr.hasNext())
   {
      const QString filename = itr.fileName();

      if(filename == "doom")
         ++score;
      else if(filename == "shots")
         ++score;

      itr.next();
   }

   return score >= 2;
}

void SetUserPath()
{
   QDir userPath;

   if(qEnvironmentVariableIsSet("ETERNITYUSER"))
   {
      QDir eternityUser(qEnvironmentVariable("ETERNITYUSER"));
      if(eternityUser.exists() && CheckUserPath(eternityUser))
      {
         userPath = eternityUser;
         return;
      }
   }

#ifdef Q_OS_LINUX
   // TODO: Oh good heavens
   if(qEnvironmentVariableIsSet("XDG_CONFIG_HOME"))
      userPath = qEnvironmentVariable("XDG_CONFIG_HOME");
   else
   {
      userPath = qEnvironmentVariable("HOME");
      // create userPath + "/.config";
   }
#endif

#ifdef Q_OS_WIN
   QFileInfo eternityPath = QCoreApplication::applicationDirPath() + "/Eternity.exe";
#else
   QFileInfo eternityPath = QCoreApplication::applicationDirPath() + "/eternity";
#endif
   if(eternityPath.exists())
   {
      QDir exeUserPath = QCoreApplication::applicationDirPath() + QDir::separator() + "user";
      if(exeUserPath.exists() && CheckUserPath(exeUserPath))
      {
         userPath = exeUserPath;
         return;
      }
   }

   if(!userPath.exists())
   {
      QDir exeWorkingPath = QDir::currentPath() + QDir::separator() + "user";
      if(exeWorkingPath.exists() && CheckUserPath(exeWorkingPath))
      {
         userPath = exeWorkingPath;
         return;
      }
   }

   if(!userPath.exists())
   {
      QDir exeWorkingPath = QDir::currentPath() + QDir::separator() + ".." + QDir::separator() + "user";
      if(exeWorkingPath.exists() && CheckUserPath(exeWorkingPath))
      {
         userPath = exeWorkingPath;
         return;
      }
   }
}
