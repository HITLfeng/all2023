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

typedef struct MsgBuf
{
    OperatorCode opCode;
    uint32_t bufLen;
    char msg[BUF_SIZE];
} MsgBufT;



#ifdef __cplusplus
}
#endif



#endif  // __OUT_TYPE_DEFS__