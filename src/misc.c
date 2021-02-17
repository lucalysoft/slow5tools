// Miscellaneous helper functions

#include <zlib.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "misc.h"
#include "error.h"
#include "fast5.h"

#define DBL_STRING_BUF_FIXED_CAP (64) // 2^6

int uint_check(const char *str);
int int_check(const char *str);
int float_check(const char *str);

// FAST5

/* Check if path has the fast5 extension
 *
 * @param       file path
 * @return      whether path has the fast5 extension
 */
bool has_fast5_ext(const char *f_path) {
    bool ret = false;

    if (f_path != NULL) {
        size_t f_path_len = strlen(f_path);
        size_t fast5_ext_len = strlen(FAST5_EXTENSION);

        if (f_path_len >= fast5_ext_len &&
                strcmp(f_path + (f_path_len - fast5_ext_len), FAST5_EXTENSION) == 0) {
            ret = true;
        }
    }

    return ret;
}

// From https://code.woboq.org/userspace/glibc/string/strsep.c.html
char *
strsep_mine (char **stringp, const char *delim)
{
  char *begin, *end;
  begin = *stringp;
  if (begin == NULL)
    return NULL;
  /* Find the end of the token.  */
  end = begin + strcspn (begin, delim);
  if (*end)
    {
      /* Terminate the token and set *STRINGP past NUL character.  */
      *end++ = '\0';
      *stringp = end;
    }
  else
    /* No more delimiters; this is the last token.  */
    *stringp = NULL;
  return begin;
}

int uint_check(const char *str) {

    // Check for:
    // empty string
    // first number is not 0 if more letters in string
    if (strlen(str) == 0 || (strlen(str) > 1 && str[0] == '0')) {
        return -1;
    }

    // Ensure only integers in string
    for (size_t i = 0; i < strlen(str); ++ i) {
        if (!isdigit(str[i])) {
            return -1;
        }
    }

    return 0;
}

int int_check(const char *str) {

    // Check for:
    // empty string
    // first number is not 0 if more letters in string
    if (strlen(str) == 0 || (strlen(str) > 1 && str[0] == '0')) {
        return -1;
    }

    // Ensure only integers in string
    for (size_t i = 0; i < strlen(str); ++ i) {
        if (!(isdigit(str[i]) || str[i] == '-')) {
            return -1;
        }
    }

    return 0;
}

int float_check(const char *str) {

    // Ensure no empty string
    if (strlen(str) == 0) {
        return -1;
    }

    // Ensure only integers in string, '.', '-'
    for (size_t i = 0; i < strlen(str); ++ i) {
        if (!(isdigit(str[i]) || str[i] == '.' || str[i] == '-')) {
            return -1;
        }
    }

    return 0;
}

// Atoi but to uint64_t
// and without any symbols
// and without 0 prefixing
uint64_t ato_uint64(const char *str, int *err) {
    uint64_t ret = 0;

    if (uint_check(str) == -1) {
        *err = -1;
        return ret;
    }

    unsigned long long int tmp = strtoull(str, NULL, 10);
    if (ret == ULLONG_MAX && errno == ERANGE) {
        *err = -1;
        return ret;
    }

    if (tmp > UINT64_MAX) {
        *err = -1;
        return ret;
    }

    ret = (uint64_t) tmp;
    *err = 0;
    return ret;
}

// Atoi but to uint32_t
// and without any symbols
// and without 0 prefixing
uint32_t ato_uint32(const char *str, int *err) {
    uint32_t ret = 0;
    if (uint_check(str) == -1) {
        *err = -1;
        return ret;
    }

    unsigned long int tmp = strtoul(str, NULL, 10);
    if (tmp == ULONG_MAX && errno == ERANGE) {
        *err = -1;
        return ret;
    }

    if (tmp > UINT32_MAX) {
        *err = -1;
        return ret;
    }

    ret = (uint32_t) tmp;
    *err = 0;
    return ret;
}

// Atoi but to uint16_t
// and without any symbols
// and without 0 prefixing
uint16_t ato_uint16(const char *str, int *err) {
    uint16_t ret = 0;
    if (uint_check(str) == -1) {
        *err = -1;
        return ret;
    }

    unsigned long int tmp = strtoul(str, NULL, 10);
    if (tmp == ULONG_MAX && errno == ERANGE) {
        *err = -1;
        return ret;
    }

    if (tmp > UINT16_MAX) {
        *err = -1;
        return ret;
    }

    ret = (uint16_t) tmp;
    *err = 0;
    return ret;
}

// Atoi but to uint8_t
// and without any symbols
// and without 0 prefixing
uint8_t ato_uint8(const char *str, int *err) {
    uint8_t ret = 0;
    if (uint_check(str) == -1) {
        *err = -1;
        return ret;
    }

    unsigned long int tmp = strtoul(str, NULL, 10);
    if (tmp > UINT8_MAX) {
        *err = -1;
        return ret;
    }

    ret = (uint8_t) tmp;
    *err = 0;
    return ret;
}

// Atoi but to int64_t
// and without any symbols
// and without 0 prefixing
int64_t ato_int64(const char *str, int *err) {
    int64_t ret = 0;
    if (int_check(str) == -1) {
        *err = -1;
        return ret;
    }

    long int tmp = strtol(str, NULL, 10);
    if (tmp > INT64_MAX || tmp < INT64_MIN) {
        *err = -1;
        return ret;
    }

    ret = (int64_t) tmp;
    *err = 0;
    return ret;
}

// Atoi but to int32_t
// and without any symbols
// and without 0 prefixing
int32_t ato_int32(const char *str, int *err) {
    int32_t ret = 0;
    if (int_check(str) == -1) {
        *err = -1;
        return ret;
    }

    long int tmp = strtol(str, NULL, 10);
    if (tmp > INT32_MAX || tmp < INT32_MIN) {
        *err = -1;
        return ret;
    }

    ret = (int32_t) tmp;
    *err = 0;
    return ret;
}

// Atoi but to int16_t
// and without any symbols
// and without 0 prefixing
int16_t ato_int16(const char *str, int *err) {
    int16_t ret = 0;
    if (int_check(str) == -1) {
        *err = -1;
        return ret;
    }

    long int tmp = strtol(str, NULL, 10);
    if (tmp > INT16_MAX || tmp < INT16_MIN) {
        *err = -1;
        return ret;
    }

    ret = (int16_t) tmp;
    *err = 0;
    return ret;
}

// Atoi but to int8_t
// and without any symbols
// and without 0 prefixing
int8_t ato_int8(const char *str, int *err) {
    int8_t ret = 0;
    if (int_check(str) == -1) {
        *err = -1;
        return ret;
    }

    long int tmp = strtol(str, NULL, 10);
    if (tmp > INT8_MAX || tmp < INT8_MIN) {
        *err = -1;
        return ret;
    }

    ret = (int8_t) tmp;
    *err = 0;
    return ret;
}

float strtof_check(const char *str, int *err) {
    float ret = 0;

    if (float_check(str) == -1) {
        *err = -1;
        return ret;
    }

    ret = strtof(str, NULL);
    if (errno == ERANGE && (ret == HUGE_VAL || ret == -HUGE_VAL || ret == (float) 0)) {
        *err = -1;
        return ret;
    }

    *err = 0;
    return ret;
}

double strtod_check(const char *str, int *err) {
    double ret = 0;

    if (float_check(str) == -1) {
        *err = -1;
        return ret;
    }

    ret = strtod(str, NULL);
    if (errno == ERANGE && (ret == HUGE_VAL || ret == -HUGE_VAL || ret == (double) 0)) {
        *err = -1;
        return ret;
    }

    *err = 0;
    return ret;
}

// Convert double to decimal string without trailing 0s
char *double_to_str(double x) {
    char *str = (char *) malloc(DBL_STRING_BUF_FIXED_CAP * sizeof *str);
    MALLOC_CHK(str);

    int len = sprintf(str, "%f", x);

    char *ptr = NULL;
    for (int i = len - 1; i >= 1; -- i) {
        if (str[i] == '0') {
            ptr = str + i;
        } else if (str[i] == '.') {
            // Set pointer on decimal point if it was just after it
            if (ptr == str + i + 1) {
                ptr = str + i;
            }
            *ptr = '\0';
            break;
        }
    }

    return str;
}

uint8_t get_type_size(const char *name) {
    uint8_t size = 0;

    if CHECK_TYPE(name, int8_t)
    else if CHECK_TYPE(name, uint8_t)
    else if CHECK_TYPE(name, int16_t)
    else if CHECK_TYPE(name, uint16_t)
    else if CHECK_TYPE(name, int32_t)
    else if CHECK_TYPE(name, uint32_t)
    else if CHECK_TYPE(name, int64_t)
    else if CHECK_TYPE(name, uint64_t)
    else if CHECK_TYPE(name, float)
    else if CHECK_TYPE(name, double)
    else if CHECK_TYPE(name, char)

    return size;
}

int memcpy_type(uint8_t *data, const char *value, const char *name) {
    int err = -1;

    // TODO fix this is disgusting :(
    if (TYPE(name, int8_t)) {
        int8_t value_conv = ato_int8(value, &err);
        if (err != -1) {
            memcpy(data, &value_conv, sizeof value_conv);
        }
    } else if (TYPE(name, uint8_t)) {
        uint8_t value_conv = ato_uint8(value, &err);
        if (err != -1) {
            memcpy(data, &value_conv, sizeof value_conv);
        }
    } else if (TYPE(name, int16_t)) {
        int16_t value_conv = ato_int16(value, &err);
        if (err != -1) {
            memcpy(data, &value_conv, sizeof value_conv);
        }
    } else if (TYPE(name, uint16_t)) {
        uint16_t value_conv = ato_uint16(value, &err);
        if (err != -1) {
            memcpy(data, &value_conv, sizeof value_conv);
        }
    } else if (TYPE(name, int32_t)) {
        int32_t value_conv = ato_int32(value, &err);
        if (err != -1) {
            memcpy(data, &value_conv, sizeof value_conv);
        }
    } else if (TYPE(name, uint32_t)) {
        uint32_t value_conv = ato_uint32(value, &err);
        if (err != -1) {
            memcpy(data, &value_conv, sizeof value_conv);
        }
    } else if (TYPE(name, int64_t)) {
        int64_t value_conv = ato_int64(value, &err);
        if (err != -1) {
            memcpy(data, &value_conv, sizeof value_conv);
        }
    } else if (TYPE(name, uint64_t)) {
        uint64_t value_conv = ato_uint64(value, &err);
        if (err != -1) {
            memcpy(data, &value_conv, sizeof value_conv);
        }
    } else if (TYPE(name, float)) {
        float value_conv = strtof_check(value, &err);
        if (err != -1) {
            memcpy(data, &value_conv, sizeof value_conv);
        }
    } else if (TYPE(name, double)) {
        double value_conv = strtod_check(value, &err);
        if (err != -1) {
            memcpy(data, &value_conv, sizeof value_conv);
        }
    } else if (TYPE(name, char)) {
        char value_conv = value[0];
        if (err != -1) {
            memcpy(data, &value_conv, sizeof value_conv);
        }
    } else if (TYPE(name, char*)) {
        err = 0;
        memcpy(data, value, strlen(value) + 1);
    }

    // TODO handle arrays

    return err;
}
