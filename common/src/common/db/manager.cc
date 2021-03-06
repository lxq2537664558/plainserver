#include "common/db/manager.h"

namespace ps_common_db {

Manager::Manager(dbconnector_type_t connector_type) {
  __ENTER_FUNCTION
    connector_type_ = connector_type;
    odbc_system_ = NULL;
  __LEAVE_FUNCTION
}

Manager::~Manager() {
  __ENTER_FUNCTION
    SAFE_DELETE(odbc_system_);
  __LEAVE_FUNCTION
}

bool Manager::init(const char *connection_or_dbname,
                   const char *username,
                   const char *password) {
  __ENTER_FUNCTION
    bool result = true;
    switch (connector_type_) {
      case kDBConnectorTypeODBC: {
        odbc_system_ = new odbc::System();
        Assert(odbc_system_);
        result = 
          odbc_system_->init(connection_or_dbname, username, password);
        break;
      }
      default:
        result = false;
        break;
    }
    return result;
  __LEAVE_FUNCTION
    return false;
}

dbconnector_type_t Manager::get_connector_type() const {
  return connector_type_;
}

bool Manager::query() {
  __ENTER_FUNCTION
    bool result = true;
    switch (connector_type_) {
      case kDBConnectorTypeODBC:
        result = odbc_system_->query();
        break;
      default:
        result = false;
        break;
    }
    return result;
  __LEAVE_FUNCTION
    return false;
}

bool Manager::fetch(int32_t orientation, int32_t offset) {
  __ENTER_FUNCTION
    bool result = true;
    switch (connector_type_) {
      case kDBConnectorTypeODBC:
        result = odbc_system_->fetch(orientation, offset);
        break;
      default:
        result = false;
        break;
    }
    return result;
  __LEAVE_FUNCTION
    return false;
}

db_query_t *Manager::get_internal_query() {
  __ENTER_FUNCTION
    db_query_t *query_pointer = NULL;
    switch (connector_type_) {
      case kDBConnectorTypeODBC:
        query_pointer = odbc_system_->get_internal_query();
        break;
      default:
        query_pointer = NULL;
        break;
    }
    return query_pointer;
  __LEAVE_FUNCTION
    return NULL;
}

int32_t Manager::get_affectcount() const {
  __ENTER_FUNCTION
    int32_t result = 0;
    switch (connector_type_) {
      case kDBConnectorTypeODBC:
        result = odbc_system_->get_result_count();
        break;
      default:
        result = -1;
        break;
    }
    return result;
  __LEAVE_FUNCTION
    return -1;
}

bool Manager::check_db_connect() {
  __ENTER_FUNCTION
    bool result = true;
    switch (connector_type_) {
      case kDBConnectorTypeODBC:
        result = odbc_system_->check_db_connect();
        break;
      default:
        result = false;
        break;
    }
    return result;
  __LEAVE_FUNCTION
    return false;
}

float Manager::get_float(int32_t column_index, int32_t& error_code) {
  __ENTER_FUNCTION
    float result = 0.0f;
    switch (connector_type_) {
      case kDBConnectorTypeODBC:
        result = 
          odbc_system_->getinterface()->get_float(column_index, error_code);
        break;
      default:
        break;
    }
    return result;
  __LEAVE_FUNCTION
    return 0.0f;
}

int64_t Manager::get_int64(int32_t column_index, int32_t& error_code) {
  __ENTER_FUNCTION
    int64_t result = -1;
    switch (connector_type_) {
      case kDBConnectorTypeODBC:
        result = 
          odbc_system_->getinterface()->get_int64(column_index, error_code);
        break;
      default:
        break;
    }
    return result;
  __LEAVE_FUNCTION
    return -1;
}

uint64_t Manager::get_uint64(int32_t column_index, int32_t& error_code) {
  __ENTER_FUNCTION
    uint64_t result = 0;
    switch (connector_type_) {
      case kDBConnectorTypeODBC:
        result = 
          odbc_system_->getinterface()->get_uint64(column_index, error_code);
        break;
      default:
        break;
    }
    return result;
  __LEAVE_FUNCTION
    return 0;
}

int32_t Manager::get_int32(int32_t column_index, int32_t& error_code) {
  __ENTER_FUNCTION
    int32_t result = -1;
    switch (connector_type_) {
      case kDBConnectorTypeODBC:
        result = 
          odbc_system_->getinterface()->get_int32(column_index, error_code);
        break;
      default:
        break;
    }
    return result;
  __LEAVE_FUNCTION
    return -1;
}

uint32_t Manager::get_uint32(int32_t column_index, int32_t& error_code) {
  __ENTER_FUNCTION
    uint32_t result = 0;
    switch (connector_type_) {
      case kDBConnectorTypeODBC:
        result = 
          odbc_system_->getinterface()->get_uint32(column_index, error_code);
        break;
      default:
        break;
    }
    return result;
  __LEAVE_FUNCTION
    return 0;
}
   
int16_t Manager::get_int16(int32_t column_index, int32_t& error_code) {
  __ENTER_FUNCTION
    int16_t result = -1;
    switch (connector_type_) {
      case kDBConnectorTypeODBC:
        result = 
          odbc_system_->getinterface()->get_int16(column_index, error_code);
        break;
      default:
        break;
    }
    return result;
  __LEAVE_FUNCTION
    return -1;
}

uint16_t Manager::get_uint16(int32_t column_index, int32_t& error_code) {
  __ENTER_FUNCTION
    uint16_t result = 0;
    switch (connector_type_) {
      case kDBConnectorTypeODBC:
        result = 
          odbc_system_->getinterface()->get_uint16(column_index, error_code);
        break;
      default:
        break;
    }
    return result;
  __LEAVE_FUNCTION
    return 0;
}
   
int8_t Manager::get_int8(int32_t column_index, int32_t& error_code) {
  __ENTER_FUNCTION
    int8_t result = -1;
    switch (connector_type_) {
      case kDBConnectorTypeODBC:
        result = 
          odbc_system_->getinterface()->get_int8(column_index, error_code);
        break;
      default:
        break;
    }
    return result;
  __LEAVE_FUNCTION
    return -1;
}

uint8_t Manager::get_uint8(int32_t column_index, int32_t& error_code) {
  __ENTER_FUNCTION
    uint8_t result = 0;
    switch (connector_type_) {
      case kDBConnectorTypeODBC:
        result = 
          odbc_system_->getinterface()->get_int8(column_index, error_code);
        break;
      default:
        break;
    }
    return result;
  __LEAVE_FUNCTION
    return 0;
}
   
int32_t Manager::get_string(int32_t column_index, 
                            char *buffer, 
                            int32_t buffer_length, 
                            int32_t& error_code) {
  __ENTER_FUNCTION
    int32_t result = -1;
    switch (connector_type_) {
      case kDBConnectorTypeODBC:
        result = 
          odbc_system_->getinterface()->get_string(column_index,
                                                   buffer,
                                                   buffer_length,
                                                   error_code);
        break;
      default:
        break;
    }
    return result;
  __LEAVE_FUNCTION
    return -1;
}

int32_t Manager::get_field(int32_t column_index, 
                           char *buffer, 
                           int32_t buffer_length, 
                           int32_t& error_code) {
  __ENTER_FUNCTION
    int32_t result = -1;
    switch (connector_type_) {
      case kDBConnectorTypeODBC:
        result = 
          odbc_system_->getinterface()->get_field(column_index,
                                                  buffer,
                                                  buffer_length,
                                                  error_code);
        break;
      default:
        break;
    }
    return result;
  __LEAVE_FUNCTION
    return -1;
}

int32_t Manager::get_binary(int32_t column_index, 
                            char *buffer, 
                            int32_t buffer_length, 
                            int32_t& error_code) {
  __ENTER_FUNCTION
    int32_t result = -1;
    switch (connector_type_) {
      case kDBConnectorTypeODBC:
        result = 
          odbc_system_->getinterface()->get_binary(column_index,
                                                   buffer,
                                                   buffer_length,
                                                   error_code);
        break;
      default:
        break;
    }
    return result;
  __LEAVE_FUNCTION
    return -1;
}

int32_t Manager::get_binary_withdecompress(int32_t column_index, 
                                           char *buffer, 
                                           int32_t buffer_length, 
                                           int32_t& error_code) {
  __ENTER_FUNCTION
    int32_t result = -1;
    switch (connector_type_) {
      case kDBConnectorTypeODBC:
        result = 
          odbc_system_->getinterface()->get_binary_withdecompress(
              column_index,
              buffer,
              buffer_length,
              error_code);
        break;
      default:
        break;
    }
    return result;
  __LEAVE_FUNCTION
    return -1;
}

} //namespace ps_common_db
