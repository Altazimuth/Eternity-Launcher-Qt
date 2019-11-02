// GPLv3 and shit
// Taken from d_files.cpp in the Eternity Engine
// Thanks to James Haley, and Ioan Chera whose code is used here
// I, Max Waine, am not gonna thank myself for the code there I used here. That's weird.

#include <QtGlobal>
#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>

static QString basePath;
static QString userPath;

QString GetBasePath() { return basePath; }
QString GetUserPath() { return userPath; }

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
   QDir baseDir;
   int res = BASE_NOTEXIST, source = BASE_NUMBASE;

   if(qEnvironmentVariableIsSet("ETERNITYBASE"))
   {
      QDir eternityBase(qEnvironmentVariable("ETERNITYBASE"));
      res = CheckBasePath(eternityBase);
      if(res == BASE_ISGOOD)
         baseDir = eternityBase;
   }

   const char *s = PlatformInstallDirectory();
   if(res != BASE_ISGOOD && s != nullptr)
   {
      const QDir platformInstallDir(s);
      res = CheckBasePath(platformInstallDir);
      if(res == BASE_ISGOOD)
         baseDir = platformInstallDir;
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
         baseDir = exeBasePath;
   }

   if(res != BASE_ISGOOD)
   {
      QDir exeWorkingPath = QDir::currentPath() + "/base";
      res = CheckBasePath(exeWorkingPath);
      if(res == BASE_ISGOOD)
         baseDir = exeWorkingPath;
      else
      {
         // TODO: Oops
      }
   }

   basePath = baseDir.path();
}

//=============================================================================
//
// /user code
//

int CheckUserPath(const QDir &path)
{
   int ret = -1;

   if(path.exists())
   {
      int score = 0;

      for(QDirIterator itr(path); itr.hasNext(); itr.next())
      {
         const QString filename = itr.fileName();

         if(filename == "doom")
            ++score;
         else if(filename == "shots")
            ++score;
      }

      if(score >= 2)
         ret = BASE_ISGOOD;
      else
         ret = BASE_NOTEEBASE;
   }
   else
      ret = BASE_NOTEXIST;

   return ret;
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
   QDir userDir;
   int res = BASE_NOTEXIST, source = BASE_NUMBASE;

   if(qEnvironmentVariableIsSet("ETERNITYUSER"))
   {
      QDir eternityUser(qEnvironmentVariable("ETERNITYUSER"));
      res = CheckUserPath(eternityUser);
      if(res == BASE_ISGOOD)
      {
         userDir = eternityUser;
         source = BASE_ENVIRON;
      }
   }

   // TODO: TEST THIS, GOOD LORD TEST THIS
   // check OS-specific home dir
#ifdef Q_OS_LINUX
   if(res != BASE_ISGOOD)
   {
      bool pathSet = true;
      if(qEnvironmentVariableIsSet("XDG_CONFIG_HOME"))
         userDir = QDir(qEnvironmentVariable("XDG_CONFIG_HOME"));
      else if(qEnvironmentVariableIsSet("HOME"))
      {
         userDir = QDir(qEnvironmentVariable("HOME"));
         userDir.mkdir(".config");
         userDir.cd(".config");
      }
      else
         pathSet = false;

      if(pathSet)
      {
         // Try to create this directory and populate it with needed directories.
         userDir.mkdir("eternity");
         userDir.cd("eternity");
         if(userDir.mkdir("user"))
         {
            userDir.cd("user");
            for(size_t i = 0; i != (sizeof(userdirs) / sizeof(*userdirs)); i++)
               userDir.mkdir(userdirs[i]);
         }

         res = CheckUserPath(userDir);
         if(res == BASE_ISGOOD)
            source = BASE_HOMEDIR;
      }
   }
#endif

#ifdef Q_OS_WIN
   QFileInfo eternityPath = QCoreApplication::applicationDirPath() + "/Eternity.exe";
#else
   QFileInfo eternityPath = QCoreApplication::applicationDirPath() + "/eternity";
#endif
   if(res != BASE_ISGOOD && eternityPath.exists())
   {
      QDir exeUserPath = QCoreApplication::applicationDirPath() + "/user";
      res = CheckUserPath(exeUserPath);
      if(res == BASE_ISGOOD)
      {
         userDir = exeUserPath;
         source = BASE_EXEDIR;
      }
   }

   if(res != BASE_ISGOOD)
   {
      QDir exeWorkingPath = QDir::currentPath() + "/user";
      res = CheckUserPath(exeWorkingPath);
      if(res == BASE_ISGOOD)
      {
         userDir = exeWorkingPath;
         source = BASE_WORKING;
      }
   }

   //if(res != BASE_ISGOOD)
   //{
   //   QDir exeWorkingPath = QDir::currentPath() + "/../user";
   //   res = CheckUserPath(exeWorkingPath);
   //   if(res == BASE_ISGOOD)
   //   {
   //      userDir = exeWorkingPath;
   //      source = BASE_BASEPARENT;
   //   }
   //}

   userPath = userDir.path();
}
