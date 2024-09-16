
#include "main_worker.h"

int main(int argc, char *argv[])
{
    normal_info("start kv database server.");
    Status ret = GMERR_OK;
    ret = MainWorkerStart();
    return ret;
}