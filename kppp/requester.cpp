/*
 *              kPPP: A pppd Front End for the KDE project
 *
 * $Id$
 *
 *              Copyright (C) 1997,98 Bernd Johannes Wuebben,
 *		                      Mario Weilguni,
 *                                    Harri Porten
 *
 *
 * This file was contributed by Harri Porten <porten@tu-harburg.de>
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/uio.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <sys/socket.h> 
#include <sys/un.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>

#include "qtimer.h"
#include "auth.h"
#include "pppdata.h"
#include "requester.h"

#include "log.h"

Requester *Requester::rq = 0L;

Requester::Requester(int s) : socket(s) {
  assert(rq==0L);
  rq = this;
}

Requester::~Requester() {
}

// 
// Receive file name and file descriptors from envoy
//  
int Requester::recvFD(char *filename, int size) {
  struct { struct cmsghdr cmsg; int fd; } control;
  struct msghdr	msg;
  struct ResponseHeader response;

  struct iovec iov[2];
  int flags = 0, fd, cmsglen, len;

  msg.msg_name = 0L;
  msg.msg_namelen = 0;
  msg.msg_iov = &iov[0];
  msg.msg_iovlen = 2;
  msg.msg_control = &control;
  msg.msg_controllen = sizeof(control);

  iov[0].iov_base = &response;
  iov[0].iov_len = sizeof(struct ResponseHeader);
  iov[1].iov_base = filename;
  iov[1].iov_len = size;
#ifdef CMSG_LEN
  cmsglen = CMSG_LEN(sizeof(int));
#else
  cmsglen = sizeof(struct cmsghdr) + sizeof(int);
#endif

  fd = -1;

  // set alarm in case recvmsg() hangs 
  //  signal(SIGALRM, recv_timeout);
  //  alarm(2);

  len = recvmsg(socket, &msg, flags);

  alarm(0);
  signal(SIGALRM, SIG_DFL);
  
  if(len <= 0) {
    perror("recvmsg failed");
    return -1;
  } else if (msg.msg_controllen != (unsigned)cmsglen) {
    perror("recvmsg: truncated message");
    exit(1);
  } else {
    filename[size-1] = '\0';
#ifdef CMSG_DATA
    fd = *((int *)CMSG_DATA(&control.cmsg));
#else
    fd = *((int *) control.cmsg.cmsg_data);
#endif
    Debug("response.status: %i", response.status);
    assert(response.status <= 0);
    if(response.status < 0)
      return response.status;
  }

  return fd;
}

bool Requester::recvResponse() {

  struct msghdr	msg;
  struct iovec iov;
  struct ResponseHeader response;
  int flags = 0, len;

  msg.msg_name = 0L;
  msg.msg_namelen = 0;
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  msg.msg_control = 0L;
  msg.msg_controllen = 0;

  iov.iov_base = &response;
  iov.iov_len = sizeof(struct ResponseHeader);
  Debug("recvResponse(): waiting for message");
  len = recvmsg(socket, &msg, flags);
  Debug("recvResponse(): received message");
  if (len <= 0) {
    perror("recvmsg failed");
  } else {
    Debug("response.status: %i", response.status);
  }

  return (response.status == 0);
}

int Requester::openModem(const char *dev) {

  struct OpenModemRequest req;
  req.header.type = Opener::OpenDevice;
  strncpy(req.modemPath, dev, Opener::MaxPathLen);
  req.modemPath[Opener::MaxPathLen] = '\0';

  sendRequest((struct RequestHeader *) &req, sizeof(req));
  return recvFD(buffer, sizeof(buffer));
}


int Requester::openLockfile(char *file, int flags) {

  struct OpenLockRequest req;

  req.header.type = Opener::OpenLock;
  strncpy(req.file, file, Opener::MaxPathLen);
  req.file[Opener::MaxPathLen] = '\0';
  req.flags = flags;

  sendRequest((struct RequestHeader *) &req, sizeof(req));
  return recvFD(buffer, sizeof(buffer));
}


bool Requester::removeLockfile() {

  struct RemoveLockRequest req;

  req.header.type = Opener::RemoveLock;

  sendRequest((struct RequestHeader *) &req, sizeof(req));
  return recvResponse();
}


int Requester::openResolv(int flags) {

  struct OpenResolvRequest req;

  req.header.type = Opener::OpenResolv;
  req.flags = flags;
  sendRequest((struct RequestHeader *) &req, sizeof(req));
  return recvFD(buffer, sizeof(buffer));
}


int Requester::openSysLog() {

  struct OpenLogRequest req;

  req.header.type = Opener::OpenSysLog;
  sendRequest((struct RequestHeader *) &req, sizeof(req));
  return recvFD(buffer, sizeof(buffer));
}


bool Requester::setPAPSecret(const char *name, const char *password) {
  if(gpppdata.authMethod() != AUTH_PAP)
    return false;

  assert(name!=0);
  assert(password!=0);

  struct SetSecretRequest req;
  req.header.type = Opener::SetSecret;
  req.authMethod = Opener::PAP;
  strncpy(req.username, name, Opener::MaxStrLen);
  req.username[Opener::MaxStrLen] = '\0';
  strncpy(req.password, password, Opener::MaxStrLen);
  req.password[Opener::MaxStrLen] = '\0';
  sendRequest((struct RequestHeader *) &req, sizeof(req));
  return recvResponse();
}

bool Requester::setCHAPSecret(const char *name, const char *password) {
  if(gpppdata.authMethod() != AUTH_CHAP)
    return false;

  assert(name!=0);
  assert(password!=0);

  struct SetSecretRequest req;
  req.header.type = Opener::SetSecret;
  req.authMethod = Opener::CHAP;
  strncpy(req.username, name, Opener::MaxStrLen);
  req.username[Opener::MaxStrLen] = '\0';
  strncpy(req.password, password, Opener::MaxStrLen);
  req.password[Opener::MaxStrLen] = '\0';

  sendRequest((struct RequestHeader *) &req, sizeof(req));

  return recvResponse();
}

bool Requester::removeSecret(int authMethod) {
  struct RemoveSecretRequest req;
  req.header.type = Opener::RemoveSecret;
  if(authMethod == AUTH_PAP && gpppdata.authMethod() == AUTH_PAP)
    req.authMethod = Opener::PAP;
  else
    if(authMethod == AUTH_CHAP && gpppdata.authMethod() == AUTH_CHAP)
      req.authMethod = Opener::CHAP;
    else
      return false;

  sendRequest((struct RequestHeader *) &req, sizeof(req));
  return recvResponse();
}


bool Requester::stop() {

  struct StopRequest req;
  req.header.type = Opener::Stop;
  sendRequest((struct RequestHeader *) &req, sizeof(req));

  //  return recvResponse();
  return true;
}


bool Requester::sendRequest(struct RequestHeader *request, int len) {

  request->len = len - sizeof(struct RequestHeader);

  struct msghdr	msg;
  struct iovec iov;

  iov.iov_base = (void *) request;
  iov.iov_len = len;
  
  msg.msg_name = 0L;
  msg.msg_namelen = 0;
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  msg.msg_control = 0L;
  msg.msg_controllen = 0;
  Debug("sendRequest: trying to send msg type %i", request->type);
  sendmsg(socket, &msg, 0);
  Debug("sendRequest: sent message");

  return true;
}

void recv_timeout(int) {
  Debug("timeout()");
}