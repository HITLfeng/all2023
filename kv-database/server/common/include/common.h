#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

// 调试打印开关
#define __DEBUG

#ifdef __DEBUG
#define normal_info(format, ...) printf("["__FILE__                                        \
                                        "][Line: %d][%s]: \033[32m" format "\033[32;0m\n", \
                                        __LINE__, __func__, ##__VA_ARGS__)
#define warning_info(format, ...) printf("["__FILE__                                        \
                                         "][Line: %d][%s]: \033[33m" format "\033[32;0m\n", \
                                         __LINE__, __func__, ##__VA_ARGS__)
#define error_info(format, ...) printf("["__FILE__                                        \
                                       "][Line: %d][%s]: \033[31m" format "\033[32;0m\n", \
                                       __LINE__, __func__, ##__VA_ARGS__)
#else
#define normal_info(format, ...)
#define warn_info(format, ...)
#define error_info(format, ...)
#endif

/*
 * error no
 */
typedef uint32_t Status;

#define GMERR_OK 0
#define GMERR_MEMORY_ALLOC_FAILED 1
#define GMERR_SOCKET_FAILED 2

/*
 * 断言非空函数
 */
#define DB_ASSERT(pointer) assert(pointer)

inline static void DB_POINT(void *pointer1)
{
    assert(pointer1 != NULL);
}

inline static void DB_POINT2(void *pointer1, void *pointer2)
{
    assert(pointer1 != NULL);
    assert(pointer2 != NULL);
}

#ifdef __cplusplus
}
#endif

#endif // __COMMON_H__
