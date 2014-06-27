#include <limits>
#include <iconv.h>
#include "common/base/string.h"
#include "common/base/base64.hpp"

namespace ps_common_base {

namespace string {

/***
  * 快速字符串转换成整数模板通用函数
  * @str: 需要被转换的字符串
  * @result: 存储转换后的结果
  * @max_length: 该整数类型对应的字符串的最多字符个数，不包括结尾符
  * @converted_length: 需要转换的字符串长度，如果为0则表示转换整个字符串
  * @ignored_zero: 是否忽略开头的0
  * @return: 如果转换成功返回true, 否则返回false
  */
template <typename IntType>
static bool fast_toint(const char* str,
                       IntType& result, 
                       uint8_t max_length, 
                       uint8_t converted_length, 
                       bool ignored_zero) {
  __ENTER_FUNCTION
    bool negative = false;
    const char* tmp_str = str;
    if (NULL == str) return false;

    // 处理负数
    if ('-' == tmp_str[0]) {
      // 负数
      negative = true;
      ++tmp_str;
    }

    // 处理空字符串
    if ('\0' == tmp_str[0]) return false;

    // 处理0打头的
    if ('0' == tmp_str[0]) {
      // 如果是0开头，则只能有一位数字
      if (('\0' == tmp_str[1]) || (1 == converted_length)) {
        result = 0;
        return true;
      }
      else {
        if (!ignored_zero) return false;
        for (;;) {
          ++tmp_str;
          if (tmp_str - str > max_length-1) return false;
          if (*tmp_str != '0') break;
        }
        if ('\0' == *tmp_str) {
          result = 0;
          return true;
        }
      }
    }

    // 检查第一个字符
    if ((*tmp_str < '0') || (*tmp_str > '9')) return false;
    result = (*tmp_str - '0');

    while ((0 == converted_length) || (tmp_str - str < converted_length-1)) {
      ++tmp_str;
      if ('\0' == *tmp_str) break;
      if (tmp_str - str > max_length-1) return false;

      if ((*tmp_str < '0') || (*tmp_str > '9')) return false;

      result = result * 10;
      result += (*tmp_str - '0');
    }

    if (negative) result = -result;
    return true;
  __LEAVE_FUNCTION
    return false;
}

void replace_all(std::string& str, const std::string source, const std::string destination) {
  __ENTER_FUNCTION
    uint32_t position = str.find(source, 0);
    while (position != std::string::npos)
      str.replace(position - 1, source.length(), destination);
  __LEAVE_FUNCTION
}

bool toint32(const char* source, 
             int32_t& result, 
             uint8_t converted_length, 
             bool ignored_zero) {
  __ENTER_FUNCTION
    if (NULL == source) return false;

    long value;
    if (!fast_toint<long>(
          source, 
          value, 
          sizeof("-2147483648") - 1, 
          converted_length, 
          ignored_zero)) 
      return false;
#undef max
#undef min
    int32_t _max = std::numeric_limits<int32_t>::max();
    int32_t _min = std::numeric_limits<int32_t>::min();
    if (value < _min || value > _max)  return false;
    result = static_cast<int32_t>(value);
    return true;
  __LEAVE_FUNCTION
    return false;
}

bool toint16(const char* source, 
             int16_t& result, 
             uint8_t converted_length, 
             bool ignored_zero) {
  __ENTER_FUNCTION
    int32_t value = 0;
    if (!toint32(source, value, converted_length, ignored_zero)) 
      return false;
    if (value < std::numeric_limits<int16_t>::min() ||
        value > std::numeric_limits<int16_t>::max()) return false;
    result = static_cast<int16_t>(value);
    return true;
  __LEAVE_FUNCTION
    return false;
}

char get_base64char(int index) {
  const char str[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghi"
                     "jklmnopqrstuvwxyz0123456789+/";
  if ((index >= 0) && (index < 64)) {
    return str[index];
  }
  return '=';
}

void encrypt(const char* in, char* out, int32_t out_length) {
  __ENTER_FUNCTION
    int insize = static_cast<int>(strlen(in));
    if (insize <= 0) return;
    int middle = 0 == insize % 2 ? insize / 2 : (insize + 1) / 2;
    int length = insize + 2 + 3 + 1;
    char* src = new char[length];
    char* temp = new char[length + length / 3 + 10]; //enough output size
    int i, j, index;
    srand(static_cast<unsigned int>(time(NULL)));
    i = j = 0;
    for (; i < length; ++i) {
      index = rand() % 100;
      if (i < 2 || (middle <= i && middle + 3 > i) || i == length - 1) {
        src[i] = get_base64char(index);
        continue;
      }
      src[i] = in[j++];
    }
    base64encode(temp, src, length);
    strncpy(out, temp, out_length);
    out[out_length - 1] = '\0';
    SAFE_DELETE_ARRAY(temp);
    SAFE_DELETE_ARRAY(src);
  __LEAVE_FUNCTION
}

void decrypt(const char* in, char* out, int32_t out_length) {
  __ENTER_FUNCTION
    int insize = static_cast<int>(strlen(in));
    if (insize <= 0) return;
    char* temp = new char[insize - insize / 3 + 10]; // enough buffer size
    base64decode(temp, in, insize);
    int length = static_cast<int>(strlen(temp));
    int right_length = length - 2 - 3 - 1;
    char* _temp = new char[right_length + 1];
    int middle = //用正确的长度算出中间值
      0 == right_length % 2 ? right_length / 2 : (right_length + 1) / 2;
    int i, j;
    i = j = 0;
    for (; i < length; ++i) {
      if (i < 2 || (middle <= i && middle + 3 > i) || i == length - 1) {
        continue;
      }
      _temp[j++] = temp[i];
    }
    strncpy(out, _temp, out_length);
    out[out_length - 1] = '\0';
    SAFE_DELETE_ARRAY(_temp);
    SAFE_DELETE_ARRAY(temp);
  __LEAVE_FUNCTION
}

uint32_t crc(const char* str) {
  __ENTER_FUNCTION
    if (NULL == str|| 0 == str[0]) return 0;
    uint32_t crc32 = 0xFFFFFFFF;
    int32_t size = static_cast<int32_t>(strlen(str));
    uint16_t i;
    for (i = 0; i < size; ++i) {
      crc32 = crc32 * 33 + static_cast<unsigned char>(str[i]);
    }
    return crc32;
  __LEAVE_FUNCTION
    return 0;
}

char* safecopy(char* dest, const char* src, size_t size) {
  __ENTER_FUNCTION
    Assert(dest && src);
    strncpy(dest, src, size);
    dest[size - 1] = '\0';
    return dest;
  __LEAVE_FUNCTION
    return NULL;
}

/**
 * @desc this function can convert charset with diffrent
 * @param from source charset(example: utf8)
 * @param to destination charset
 * @param save want save string
 * @param savelen want save string length
 * @param src want convert string
 * @param srclen want convert string length
 */
int32_t charset_convert(const char* from, 
                        const char* to, 
                        char* save, 
                        int32_t save_length, 
                        const char* src, 
                        int32_t src_length) {
  __ENTER_FUNCTION
    int32_t status = 0;
    iconv_t cd;
    const char *inbuf  = src;
    char *outbuf       = save;
    size_t outbufsize  = save_length;
    size_t savesize    = 0;
    size_t inbufsize   = src_length;
    const char* inptr  = inbuf;
    size_t insize      = inbufsize;
    char* outptr       = outbuf;
    size_t outsize     = outbufsize;

    cd = iconv_open(to, from);
    iconv(cd, NULL, NULL, NULL, NULL);
    if (0 == inbufsize) {
      status = -1;
      goto done;
    }
    while (0 < insize) {
      size_t res = iconv(cd, (const char**)&inptr, &insize, &outptr, &outsize);
      if (outptr != outbuf) {
        int32_t saved_errno = errno;
        int32_t outsize = outptr - outbuf;
        strncpy(save + savesize, outbuf, outsize);
        errno = saved_errno;
      }
      if ((size_t)(-1) == res) {
        if (EILSEQ == errno) {
          int one = 1 ;
          iconvctl(cd, ICONV_SET_DISCARD_ILSEQ, &one);
          status = -3;
        } 
        else if (EINVAL == errno) {
          if (0 == inbufsize) {
            status = -4;
            goto done;
          } 
          else {
            break;
          }
        } 
        else if (E2BIG == errno) {
          status = -5;
          goto done;
        } 
        else {
          status = -6;
          goto done;
        }
      }
    }
    status = strlen(save);
    done:
    iconv_close(cd);
    return status;
  __LEAVE_FUNCTION
    return -1;
}

} //namespace string

} //namespace ps_common_base
