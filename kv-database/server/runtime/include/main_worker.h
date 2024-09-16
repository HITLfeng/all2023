#ifndef __MAIN_WORKER_H__
#define __MAIN_WORKER_H__

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include "out_type_defs.h"
#include "../../common/include/common.h"

#ifdef __cplusplus
extern "C"
{
#endif

Status MainWorkerStart(void);



#ifdef __cplusplus
}
#endif

#endif // __MAIN_WORKER_H__