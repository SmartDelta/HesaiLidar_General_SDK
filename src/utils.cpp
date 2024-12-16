/******************************************************************************
 * Copyright 2018 The Hesai Technology Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "sys_inc.h"

#include "src/util.h"

#include <chrono>

#ifdef _WIN32

#else
#define closesocket close
#endif

// TODO: We cannot use errno on Windows to get WS errors
// You'd need WSAGetLastError for that. But that's not on linux ofc

#define DEFAULT_TIMEOUT 10 /*secondes waitting for read/write*/

int net_readn(int fd, void* vptr, int n) {
  // printf("start sys_readn: %d....\n", n);
  int nleft, nread;
  char* ptr;

  ptr = (char*)vptr;
  nleft = n;
  while (nleft > 0) {
    // printf("start read\n");
    if ((nread = recv(fd, ptr, nleft, 0)) == SOCKET_ERROR) {
      if (errno == EINTR)
        nread = 0;
      else
        return -1;
    } else if (nread == 0) {
      break;
    }
    // printf("end read, read: %d\n", nread);
    nleft -= nread;
    ptr += nread;
  }
  // printf("stop sys_readn....\n");

  return n - nleft;
}

int tcp_open(const char* ipaddr, int port) {
  int sockfd;
  const char* isIpV6 = strchr(ipaddr, ':');
  if(isIpV6 == NULL){
    struct sockaddr_in servaddr;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) return -1;

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if (inet_pton(AF_INET, ipaddr, &servaddr.sin_addr) <= 0) {
      closesocket(sockfd);
      return -1;
    }

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
      closesocket(sockfd);
      return -1;
    }
    printf("open !!!!!\n");
    return sockfd;

  }
  else{
    struct sockaddr_in6 addr;
    if ((sockfd = socket(AF_INET6, SOCK_STREAM, 0)) == -1) return -1;
    memset(&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET6;
    if (!inet_pton(AF_INET6, ipaddr, &(addr.sin6_addr))) {
      closesocket(sockfd);
      return -1;
    }
    addr.sin6_port = htons(port);
    if (connect(sockfd, (const sockaddr*)&addr, sizeof(addr)) == -1) {
      closesocket(sockfd);
      return -1;
    }
    return sockfd;

  }
}

/**
 *Description:check the socket  state
 *
 * @param
 * fd: socket
 * timeout:the time out of select
 * wait_for:socket state(r,w,conn)
 *
 * @return 1 if everything was ok, 0 otherwise
 */
int select_fd(int fd, int timeout, int wait_for) {
  fd_set fdset;
  fd_set *rd = NULL, *wr = NULL;
  struct timeval tmo;
  int result;

  FD_ZERO(&fdset);
  FD_SET(fd, &fdset);
  if (wait_for == WAIT_FOR_READ) {
    rd = &fdset;
  }
  if (wait_for == WAIT_FOR_WRITE) {
    wr = &fdset;
  }
  if (wait_for == WAIT_FOR_CONN) {
    rd = &fdset;
    wr = &fdset;
  }

  tmo.tv_sec = timeout;
  tmo.tv_usec = 0;
  do {
    result = select(fd + 1, rd, wr, NULL, &tmo);
  } while (result < 0 && errno == EINTR);

  return result;
}
double getNowTimeSec() {
    std::chrono::system_clock::time_point time = std::chrono::system_clock::now();
    std::chrono::nanoseconds nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(time.time_since_epoch());
    return (double)nanos.count() / 1E9;
    //They used clock_gettime with CLOCK_REALTIME here. That would just be wall-clock, so system_clock
    //On Windows that is not nanosecond-precise but I don't know if we can just swap this clock for a different clock or if its value is interpreted as unix time somewhere
}
