// -*-C++-*-
/* This file is part of the KDE libraries
    Copyright (C) 1997 Christian Esken (esken@kde.org)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#ifndef KAUDIO_H
#define KAUDIO_H "$Id$"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qobject.h>
#include <qtimer.h>

class KAudio : public QObject
{
  Q_OBJECT
public:

  /**
    * Create an Audio player
    */
  KAudio();

  /**
    * Play the Wav last set via setFilename(filename) or play(filename)
    */
  bool play();

  /**
    * Set filename as current media name and play it
    */
  bool play(const char *filename);

  /**
    * Set filename as current media name and play it
    */
  bool play(QString& filename);

  /**
    * Set the "current" Filename. This file can be played later with ::play()
    */
  bool setFilename(const char *filename);

  /**
    * Set the "current" Filename. This file can be played later with ::play()
    */
  bool setFilename(QString& filename);

  /**
    * If true is given, every play call is synced directly.
    */
  void setAutosync(bool autosync);

  /** If you want to recieve a Qt signal when your media is finished, you must
    *  call setSignals(true) before you play your media.
    */
  void setSignals(bool sigs=true);

  /**
    * Stop current media
    */
  bool stop();

  /** Sync media. This effectively blocks the calling process until the
    * media is played fully
    */
  void sync();

  /** Query Server status. 0 means OK. You MUST check server status after
    * creating a KAudio object.
    */
  int  serverStatus();

signals:

  /** 
    * Signal gets emitted after current media has been finished.
    * This signal only gets emitted, if setSignals(true) was called before.
    * Do not forget to include this signal in your own derived class if you
    * want signals.
    */
  void playFinished();

private:
  bool		ServerContacted;
  bool		autosync;
  char		*WAVname;
  MdCh_FNAM*	FnamChunk;
  MdCh_KEYS*	KeysChunk;
  MdCh_IHDR*	IhdrChunk;
  MdCh_STAT*	StatChunk;

  QTimer        *finishTimer;
  uint8		currentId;

private slots:

  /**
    * Internal check for "finished play media", called upon finishTimer
    */
  void checkFinished();
};

#endif
