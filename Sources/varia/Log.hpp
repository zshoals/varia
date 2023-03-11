#pragma once

#include "varia/Vcommon.hpp"

#include "varia/ds/DumbBuf.hpp"



#define VARIA_G_STRING_BUF_SIZE 100000

typedef vds_dumbbuf_t<char> varia_stringbuf_t;

//"Global" scratch buffer
void Glog_initialize(void);
varia_stringbuf_t * Glog_reference(void);

void Glog_print(void);
void Glog_clear_buffer(void);
void Glog_copy_to(void * destination, size_t destination_length);

void Glog_string(char const * string);
void Glog_newline(void);
void Glog_float(double value);
void Glog_double(double value);
void Glog_int(int value);
void Glog_int32(int32_t value);
void Glog_uint32(uint32_t value);
void Glog_int64(int64_t value);
void Glog_uint64(uint64_t value);
void Glog_size(size_t value);
void Glog_time(void);

//Generic buffer
void log_print(varia_stringbuf_t * buf);
void log_clear_buffer(varia_stringbuf_t * buf);
void log_copy_to(varia_stringbuf_t const * buf, void * destination, size_t destination_length);

void log_string(varia_stringbuf_t * buf, char const * string);
void log_newline(varia_stringbuf_t * buf);
void log_float(varia_stringbuf_t * buf, double value);
void log_double(varia_stringbuf_t * buf, double value);
void log_int(varia_stringbuf_t * buf, int value);
void log_int32(varia_stringbuf_t * buf, int32_t value);
void log_uint32(varia_stringbuf_t * buf, uint32_t value);
void log_int64(varia_stringbuf_t * buf, int64_t value);
void log_uint64(varia_stringbuf_t * buf, uint64_t value);
void log_size(varia_stringbuf_t * buf, size_t value);
void log_time(varia_stringbuf_t * buf);