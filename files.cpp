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
      QDir exeBasePath = QCoreApplication::applicationDirPath() + "/base";
      if(exeBasePath.exists() && CheckBasePath(exeBasePath))
      {
         basePath = exeBasePath;
         return;
      }
   }

   if(!basePath.exists())
   {
      QDir exeWorkingPath = QDir::currentPath() + "/base";
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

#ifdef Q_OS_LINUX
static const char *const userdirs[] =
{
   "/doom",
   "/doom2",
   "/hacx",
   "/heretic",
   "/plutonia",
   "/shots",
   "/tnt",
};
#endif

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

   // TODO: TEST THIS, GOOD LORD TEST THIS
   // check OS-specific home dir
#ifdef Q_OS_LINUX
   bool pathSet = true;
   if(qEnvironmentVariableIsSet("XDG_CONFIG_HOME"))
      userPath = QDir(qEnvironmentVariable("XDG_CONFIG_HOME"));
   else if(qEnvironmentVariableIsSet("HOME"))
   {
      userPath = QDir(qEnvironmentVariable("HOME"));
      userPath.mkdir(".config");
      userPath.setPath(userPath.path() + "/.config");
   }
   else
      pathSet = false;

   if(pathSet)
   {
      // Try to create this directory and populate it with needed directories.
      userPath.mkdir("eternity");
      userPath.setPath(userPath.path() + "/eternity");
      if(userPath.mkdir("user"))
      {
         userPath.setPath(userPath.path() + "/user");
         for(size_t i = 0; i != (sizeof(userdirs) / sizeof(*userdirs)); i++)
            userPath.mkdir(userdirs[i]);
      }
   }
#endif

#ifdef Q_OS_WIN
   QFileInfo eternityPath = QCoreApplication::applicationDirPath() + "/Eternity.exe";
#else
   QFileInfo eternityPath = QCoreApplication::applicationDirPath() + "/eternity";
#endif
   if(eternityPath.exists())
   {
      QDir exeUserPath = QCoreApplication::applicationDirPath() + "/user";
      if(exeUserPath.exists() && CheckUserPath(exeUserPath))
      {
         userPath = exeUserPath;
         return;
      }
   }

   if(!userPath.exists())
   {
      QDir exeWorkingPath = QDir::currentPath() + "/user";
      if(exeWorkingPath.exists() && CheckUserPath(exeWorkingPath))
      {
         userPath = exeWorkingPath;
         return;
      }
   }

   if(!userPath.exists())
   {
      QDir exeWorkingPath = QDir::currentPath() + "/../user";
      if(exeWorkingPath.exists() && CheckUserPath(exeWorkingPath))
      {
         userPath = exeWorkingPath;
         return;
      }
   }
}
