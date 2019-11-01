// GPLv3 and shit
// Taken from d_files.cpp in the Eternity Engine
// Thanks to James Haley, and Ioan Chera whose code is used here
// I, Max Waine, am not gonna thank myself for the code there I used here. That's weird.

#include <QtGlobal>
#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>

enum
{
   BASE_ISGOOD,
   BASE_NOTEXIST,
   //BASE_NOTDIR,
   BASE_CANTOPEN,
   BASE_NOTEEBASE,
   BASE_NUMCODES
};

enum
{
   BASE_ENVIRON,
   BASE_HOMEDIR,
   BASE_INSTALL,
   BASE_WORKING,
   BASE_EXEDIR,
   BASE_BASEPARENT, // for user dir only
   BASE_NUMBASE
};

//=============================================================================
//
// /base code
//

int CheckBasePath(const QDir &path)
{
   int ret = -1;

   if(path.exists())
   {
      int score = 0;

      for(QDirIterator itr(path); itr.hasNext(); itr.next())
      {
         const QString filename = itr.fileName();

         if(filename == "startup.wad")
            ++score;
         else if(filename == "root.edf")
            ++score;
         else if(filename == "doom")
            ++score;
      }

      if(score >= 3)
         ret = BASE_ISGOOD;
      else
         ret = BASE_NOTEEBASE;
   }
   else
      ret = BASE_NOTEXIST;

   return ret;
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
   int res = BASE_NOTEXIST, source = BASE_NUMBASE;

   if(qEnvironmentVariableIsSet("ETERNITYBASE"))
   {
      QDir eternityBase(qEnvironmentVariable("ETERNITYBASE"));
      res = CheckBasePath(eternityBase);
      if(res == BASE_ISGOOD)
         basePath = eternityBase;
   }

   const char *s = PlatformInstallDirectory();
   if(res != BASE_ISGOOD && s != nullptr)
   {
      const QDir platformInstallDir(s);
      res = CheckBasePath(platformInstallDir);
      if(res == BASE_ISGOOD)
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
   if(res != BASE_ISGOOD && eternityPath.exists())
   {
      QDir exeBasePath = QCoreApplication::applicationDirPath() + "/base";
      res = CheckBasePath(exeBasePath);
      if(res == BASE_ISGOOD)
         basePath = exeBasePath;
   }

   if(res != BASE_ISGOOD)
   {
      QDir exeWorkingPath = QDir::currentPath() + "/base";
      res = CheckBasePath(exeWorkingPath);
      if(res == BASE_ISGOOD)
         basePath = exeWorkingPath;
      else
      {
         // TODO: Oops
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
      userPath.cd(".config");
   }
   else
      pathSet = false;

   if(pathSet)
   {
      // Try to create this directory and populate it with needed directories.
      userPath.mkdir("eternity");
      userPath.cd("eternity");
      if(userPath.mkdir("user"))
      {
         userPath.cd("user");
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
