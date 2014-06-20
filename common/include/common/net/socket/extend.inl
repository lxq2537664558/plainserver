/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id extend.inl
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.it@gmail.com>
 * @date 2014/06/19 15:23
 * @uses for socket extend inline functions(epoll or icop)
 */
#ifndef PS_COMMON_NET_SOCKET_EXTEND_INL_
#define PS_COMMON_NET_SOCKET_EXTEND_INL_

#if __LINUX__
#include <sys/epoll.h>
#include <poll.h>
#endif

#if __LINUX__ /* { */
typedef struct {
  enum {
    kEventIn = EPOLLIN,
    kEventOut = EPOLLOUT,
    kEventError = EPOLLERR
  };
  int32_t fd_;
  int32_t maxcount_;
  int32_t result_num_;
  int32_t event_index_;
  struct epoll_event* result_event_;
} polldata_t;
#endif /* } */

#if __LINUX__ /* { */

inline int32_t poll_create(polldata_t& polldata, int32_t maxcount) {
  int32_t fd = epoll_create(maxcount);
  if (fd > 0) {
    polldata.fd_ = fd;
    polldata.maxcount_ = maxcount;
    polldata.result_event_ = new epoll_event[maxcount];
    signal(SIGPIPE, SIG_IGN);
  }
  return fd;
}

inline int32_t poll_add(polldata_t& polldata, int32_t fd, int32_t mask) {
  struct epoll_event _epoll_event;
  memset(&_epoll_event, 0, sizeof(_epoll_event));
  _epoll_event.events = mask;
  _epoll_event.data.fd = fd;
  int32_t result = epoll_ctl(polldata.fd_, EPOLL_CTL_ADD, fd, &_epoll_event);
  return result;
}

inline int32_t poll_mod(polldata_t& polldata, int32_t fd, int32_t mask) {
  struct epoll_event _epoll_event;
  memset(&_epoll_event, 0, sizeof(_epoll_event));
  _epoll_event.events = mask;
  _epoll_event.data.fd = fd;
  int32_t result = epoll_ctl(polldata.fd_, EPOLL_CTL_MOD, fd, &_epoll_event);
  return result;
}

inline int32_t poll_delete(polldata_t& polldata, int32_t fd) {
  struct epoll_event _epoll_event;
  memset(&_epoll_event, 0, sizeof(_epoll_event));
  _epoll_event.events = 0;
  _epoll_event.data.fd = fd;
  int32_t result = epoll_ctl(polldata.fd_, EPOLL_CTL_DEL, fd, &_epoll_event);
  return result;
}

inline int32_t poll_wait(polldata_t& polldata, int32_t timeout) {
  Assert(polldata.result_event_);
  Assert(polldata.maxcount_ > 0);
  polldata.result_num_ = epoll_wait(polldata.fd_, 
                                    polldata.result_event_, 
                                    polldata.maxcount_, 
                                    timeout);
  polldata.event_index_ = 0;
  return polldata.result_num_;
}

inline int32_t poll_destory(polldata_t& polldata) {
  ps_common_file::api::closeex(polldata.fd_);
  if (polldata.result_event_) {
    SAFE_DELETE_ARRAY(polldata.result_event_);
  }
  return 0;
}

inline int32_t poll_event(polldata_t& polldata, 
                          int32_t* fd, 
                          int32_t * events) {
  if (polldata.event_index_ < polldata.result_num_) {
    struct epoll_event& _epoll_event = 
      polldata.result_event_[polldata.event_index_++];
    *events = _epoll_event.events;
    *fd = _epoll_event.data.fd;
    return 0;
  }
  return -1;
}

#endif /* } */

#endif //PS_COMMON_NET_SOCKET_EXTEND_INL_