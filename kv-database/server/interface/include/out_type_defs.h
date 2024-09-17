#ifndef __OUT_TYPE_DEFS__
#define __OUT_TYPE_DEFS__

#include <stdint.h>

#define BUF_SIZE 1024

#ifdef __cplusplus
extern "C"
{
#endif


typedef enum OperatorCode
{
    OP_ADD_TEST = 0,
    OP_BUTT,
} OperatorCode;


typedef struct MsgBufRequestHead
{
    OperatorCode opCode;
    uint32_t requestBufLen;
} MsgBufRequestHeadT;
typedef struct MsgBufResponseHead
{
    uint32_t status; // 服务端返回值
    uint32_t responseBufLen;
} MsgBufResponseHeadT;

typedef struct MsgBufRequest
{
    OperatorCode opCode;
    uint32_t requestBufLen;
    char requestMsg[BUF_SIZE];
} MsgBufRequestT;

typedef struct MsgBufResponse
{
    uint32_t status; // 服务端返回值
    uint32_t responseBufLen;
    char requestMsg[BUF_SIZE];
} MsgBufResponseT;

#ifdef __cplusplus
}
#endif



#endif  // __OUT_TYPE_DEFS__