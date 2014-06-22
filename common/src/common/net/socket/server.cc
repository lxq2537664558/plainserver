#include "common/net/socket/server.h"

namespace ps_common_net {

namespace socket {

Server::Server(uint16_t port, uint32_t backlog) {
  __ENTER_FUNCTION
    bool result = false;
    socket_ = new ps_common_net::socket::Base();
    if (NULL == socket_) { //memory not enough
      ERRORPRINTF("ps_common_net::Server::Server"
                  " new pap_common_net::socket::Base() failed,"
                  " errorcode: %d",
                  socket_->getlast_errorcode());
      throw 1;
    }
    result = socket_->create();
    if (false == result) {
      ERRORPRINTF("ps_common_net::Server::Server"
                  " socket_->create() failed, errorcode: %d",
                  socket_->getlast_errorcode()); 
      throw 1;
    }
    result = socket_->set_reuseaddr();
    if (false == result) {
      ERRORPRINTF("ps_common_net::Server::Server"
                  " socket_->set_reuseaddr() failed, errorcode: %d",
                  socket_->getlast_errorcode());
      throw 1;
    }
    result = socket_->bind(port);
    if (false == result) {
      ERRORPRINTF("ps_common_net::Server::Server"
                  " socket_->bind(%d) failed, errorcode: %d", 
                  port,
                  socket_->getlast_errorcode());
      throw 1;
    }
    result = socket_->listen(backlog);
    if (false == result) {
      ERRORPRINTF("ps_common_net::Server::Server"
                  " socket_->listen(%d) failed, errorcode: %d",
                  backlog,
                  socket_->getlast_errorcode());
      throw 1;
    }
  __LEAVE_FUNCTION
}

Server::~Server() {
  __ENTER_FUNCTION
    if (socket_ != NULL) {
      socket_->close();
      SAFE_DELETE(socket_);
    }
  __LEAVE_FUNCTION
}

void Server::close() {
  if (socket_ != NULL) socket_->close();
}

bool Server::accept(ps_common_net::socket::Base* socket) {
  __ENTER_FUNCTION
    if (NULL == socket) return false;
    socket->close();
    if (0 == socket->port_) {
      socket->socketid_ = socket_->fastaccept();
    }
    else {
      socket->socketid_ = socket_->accept(socket->port_);
    }
    if (SOCKET_INVALID == socket->socketid_) return false;
    return true;
  __LEAVE_FUNCTION
    return false;
}

uint32_t Server::getlinger() const {
  __ENTER_FUNCTION
    uint32_t linger;
    linger = socket_->getlinger();
    return linger;
  __LEAVE_FUNCTION
    return 0;
}

bool Server::setlinger(uint32_t lingertime) {
  __ENTER_FUNCTION
    bool result = false;
    result = socket_->setlinger(lingertime);
    return result;
  __LEAVE_FUNCTION
    return false;
}

bool Server::is_nonblocking() const {
  __ENTER_FUNCTION
    bool result = false;
    result = socket_->is_nonblocking();
    return result;
  __LEAVE_FUNCTION
    return false;
}

bool Server::set_nonblocking(bool on) {
  __ENTER_FUNCTION
    return socket_->set_nonblocking(on);
  __LEAVE_FUNCTION
    return false;
}

uint32_t Server::getreceive_buffersize() const {
  __ENTER_FUNCTION
    uint32_t result = 0;
    result = socket_->getreceive_buffersize();
    return result;
  __LEAVE_FUNCTION
    return 0;
}

bool Server::setreceive_buffersize(uint32_t size) {
  __ENTER_FUNCTION
    bool result = false;
    result = socket_->setreceive_buffersize(size);
    return result;
  __LEAVE_FUNCTION
    return false;
}

int32_t Server::getid() const {
  __ENTER_FUNCTION
    int32_t result = SOCKET_INVALID;
    result = socket_->getid();
    return result;
  __LEAVE_FUNCTION
    return SOCKET_INVALID;
}

} //namespace socket

} //namespace ps_common_net
