// kfileio.cpp
// Author: Stefan Taferner <taferner@kde.org>

#include <klocale.h>
#include <kapp.h>
#include <kmsgbox.h>
#include <qmsgbox.h>
#include <qstring.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <qfile.h>
#include <qfileinf.h>

#include "kfileio.h"


//-----------------------------------------------------------------------------
static void msgDialog(const char* msg, const char* arg=NULL)
{
  QString str(256);

  if (arg) str.sprintf(msg, arg);
  else str = msg;

  KMsgBox::message(NULL, klocale->translate("File I/O Error"), str,
		   KMsgBox::STOP, klocale->translate("Ok"));
}


//-----------------------------------------------------------------------------
QString kFileToString(const char* aFileName, bool aEnsureNL, bool aVerbose)
{
  QString result;
  QFileInfo info(aFileName);
  unsigned int readLen;
  unsigned int len = info.size();
  QFile file(aFileName);

  assert(aFileName!=NULL);

  if (!info.exists())
  {
    if (aVerbose)
      msgDialog(klocale->translate("The specified file does not exist:\n%s"),
		aFileName);
    return 0;
  }
  if (info.isDir())
  {
    if (aVerbose)
      msgDialog(klocale->translate("This is a directory and not a file:\n%s"),
		aFileName);
    return 0;
  }
  if (!info.isReadable())
  {
    if (aVerbose)
      msgDialog(klocale->translate("You do not have read permissions "
				   "to the file:\n%s"), aFileName);
    return 0;
  }

  if (!file.open(IO_Raw|IO_ReadOnly))
  {
    if (aVerbose) switch(file.status())
    {
    case IO_ReadError:
      msgDialog(klocale->translate("Could not read file:\n%s"), aFileName);
      break;
    case IO_OpenError:
      msgDialog(klocale->translate("Could not open file:\n%s"), aFileName);
      break;
    default:
      msgDialog(klocale->translate("Error while reading file:\n%s"),aFileName);
    }
    return 0;
  }

  result.resize(len+1 + (int)aEnsureNL);
  readLen = file.readBlock(result.data(), len);
  if (aEnsureNL && result[len-1]!='\n')
  {
    result[len++] = '\n';
    readLen++;
  }
  result[len] = '\0';

  if (readLen < len)
  {
    QString msg(256);
    msg.sprintf(klocale->translate("Could only read %u bytes of %u."),
		readLen, len);
    msgDialog(msg);
    return 0;
  }

  return result;
}


//-----------------------------------------------------------------------------
bool kStringToFile(const QString aBuffer, const char* aFileName, 
		   bool aAskIfExists, bool aBackup=TRUE, bool aVerbose)
{
  QFile file(aFileName);
  QFileInfo info(aFileName);
  int writeLen, len, rc;

  assert(aFileName!=NULL);

  debug("WARNING: kStringToFile currently makes no backups and silently"
	"replaces existing files!");

  if (info.exists())
  {
    if (aAskIfExists)
    {
      QString str(256);
      str.sprintf(klocale->translate(
		  "File %s exists.\nDo you want to replace it ?"),
		  aFileName);
      rc = QMessageBox::information(NULL, klocale->translate("Information"),
	   str, klocale->translate("&Ok"), klocale->translate("&Cancel"),
	   0, 1);
      if (rc != 0) return FALSE;
    }
    if (aBackup)
    {
      // make a backup copy
      QString bakName = aFileName;
      bakName += '~';
      unlink(bakName);
      rc = rename(aFileName, bakName);
      if (rc)
      {
	// failed to rename file
	if (!aVerbose) return FALSE;
	rc = QMessageBox::warning(NULL, klocale->translate("Warning"),
	     klocale->translate(
             "Failed to make a backup copy of %s.\nContinue anyway ?"),
	     klocale->translate("&Ok"), klocale->translate("&Cancel"), 0, 1);
	if (rc != 0) return FALSE;
      }
    }
  }

  if (!file.open(IO_Raw|IO_WriteOnly))
  {
    if (aVerbose) switch(file.status())
    {
    case IO_WriteError:
      msgDialog(klocale->translate("Could not write to file:\n%s"), aFileName);
      break;
    case IO_OpenError:
      msgDialog(klocale->translate("Could not open file for writing:\n%s"),
		aFileName);
      break;
    default:
      msgDialog(klocale->translate("Error while writing file:\n%s"),aFileName);
    }
    return FALSE;
  }

  len = aBuffer.size()-1;
  writeLen = file.writeBlock(aBuffer.data(), len);

  if (writeLen < 0) 
  {
    msgDialog(klocale->translate("Could not write to file:\n%s"), aFileName);
    return FALSE;
  }
  else if (writeLen < len)
  {
    QString msg(256);
    msg.sprintf(klocale->translate("Could only write %d bytes of %d."),
		writeLen, len);
    msgDialog(msg);
    return FALSE;
  } 

  return TRUE;
}
