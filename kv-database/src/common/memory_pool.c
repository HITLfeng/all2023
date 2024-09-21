#include "include/common.h"

// jemalloc tcmalloc
// 内核空间/用户空间 管理堆内存、共享内存（均为虚拟内存）

// 分配内存 如一次从操作系统申请4K 避免内存碎片

// 配置内存池page和block大小
#define MEM_PAGE_SIZE 4096

#define MEM_BLOCK_SIZE_LEVEL1 4
#define MEM_BLOCK_SIZE_LEVEL2 8
#define MEM_BLOCK_SIZE_LEVEL3 16
#define MEM_BLOCK_SIZE_LEVEL4 32
#define MEM_BLOCK_SIZE_LEVEL5 64
#define MEM_BLOCK_SIZE_LEVEL6 128
#define MEM_BLOCK_SIZE_LEVEL7 256
#define MEM_BLOCK_SIZE_LEVEL8 512
#define MEM_BLOCK_SIZE_LEVEL9 1024
// 层级：4，8，16，32，64，128，256，512，1024
#define MEM_POOL_SLOT_CNT 9

KVMemoryPoolManagerT *g_kvMemoryPoolManager;

typedef struct KVMemoryPool
{
    // |****|****|****|****|****|****|****|****|****|****|****|****|****|****|****|****|****|****|****|****|****|****|
    // memPtr 指向内存块起始地址                      currMemPtr 指向当前正在使用的地址
    void *memPtr;          // 指向内存块起始地址
    void *currMemPtr;      // 指向当前正在使用的地址
    uint32_t memBlockSize; // 内存块大小，如4k一块申请下来后，按照 32 字节为单位分配，这里可以分为128份 // 这里是 32 字节
    uint32_t freeBlockCnt; // 当前内存块中空闲内存块数量
} KVMemoryPoolT;

typedef struct KVMemoryPoolManager
{
    // 层级：4，8，16，32，64，128，256，512，1024
    KVMemoryPoolT *memoryMangerPool[MEM_POOL_SLOT_CNT];
} KVMemoryPoolManagerT;

static uint32_t GetSlotBlockSizeByIdx(uint32_t idx)
{
    DB_ASSERT(idx < MEM_POOL_SLOT_CNT);
    switch (idx)
    {
    case 0:
        return MEM_BLOCK_SIZE_LEVEL1;
    case 1:
        return MEM_BLOCK_SIZE_LEVEL2;
    case 2:
        return MEM_BLOCK_SIZE_LEVEL3;
    case 3:
        return MEM_BLOCK_SIZE_LEVEL4;
    case 4:
        return MEM_BLOCK_SIZE_LEVEL5;
    case 5:
        return MEM_BLOCK_SIZE_LEVEL6;
    case 6:
        return MEM_BLOCK_SIZE_LEVEL7;
    case 7:
        return MEM_BLOCK_SIZE_LEVEL8;
    case 8:
        return MEM_BLOCK_SIZE_LEVEL9;
    default:
        log_error("GetSlotBlockSizeByIdx idx is invalid when exec KVMemoryPoolInit.");
        return 0;
    }
    log_error("GetSlotBlockSizeByIdx idx is invalid when exec KVMemoryPoolInit.");
    return 0;
}

Status KVMemoryPoolInit()
{
    // DB_POINT(memoryPool);

    g_kvMemoryPoolManager = (KVMemoryPoolManagerT *)malloc(sizeof(KVMemoryPoolManagerT));
    if (g_kvMemoryPoolManager == NULL)
    {
        log_error("malloc memoryPoolManager memory failed when exec KVMemoryPoolInit.");
        return GMERR_MEMORY_ALLOC_FAILED;
    }

    // 预计初始化36M内存资源池
    KVMemoryPoolT *memPtrArray = (KVMemoryPoolT *)malloc(MEM_PAGE_SIZE * MEM_POOL_SLOT_CNT);
    if (memPtrArray == NULL)
    {
        log_error("malloc memoryPool memory failed when exec KVMemoryPoolInit.");
        return GMERR_MEMORY_ALLOC_FAILED;
    }
    memset(memPtrArray, 0, MEM_PAGE_SIZE);

    for (uint32_t i = 0; i < MEM_POOL_SLOT_CNT; i++)
    {
        memPtrArray[i].memPtr = memPtrArray + i;
        memPtrArray[i].currMemPtr = memPtrArray[i].memPtr;
        memPtrArray[i].memBlockSize = GetSlotBlockSizeByIdx(i);
        memPtrArray[i].freeBlockCnt = MEM_PAGE_SIZE / memPtrArray[i].memBlockSize;

        // 初始 block: 使用情况如下
        // 1 2 3 4 5 6 7 ...
        // 某一刻释放了 2 5 下次申请，从几开始呢？如何管理空闲内存块
        // 32 bytes中抽出4/8个字节，作为指针，将所有block串联起来，形成链表

        // 如果使用 *pCurList，我们只能存储一个 uint8_t 类型的值，这不足以存储一个指针。
        // 而使用 *(uint8_t **)(pCurList)，我们可以存储一个指向 uint8_t 类型的指针，从而正确地建立空闲内存块链表。
        // 初始化空闲内存块链表
        uint8_t *pCurList = (uint8_t *)memPtrArray[i].memPtr;
        for (uint32_t j = 0; j < memPtrArray[i].freeBlockCnt - 1; j++)
        {
            *(uint8_t **)(pCurList) = pCurList + memPtrArray[i].memBlockSize;
            pCurList += memPtrArray[i].memBlockSize;
        }
        // 最后一个内存块的指针设置为NULL，表示链表结束
        *(uint8_t **)(pCurList) = NULL;
    }

    return GMERR_OK;
}

// 尝试从memoryPool中分配一块内存 分配失败则调用malloc
void *KVMemPoolAlloc(KVMemoryPoolT *memoryPool, uint32_t allocSize)
{
    DB_POINT(memoryPool);
    if (allocSize > memoryPool->memBlockSize)
    {
        warn_error("allocSize is too large when exec KVMemAlloc.");
        return NULL;
    }
    if (memoryPool->freeBlockCnt == 0)
    {
        // TODO: 拓展page？
        warn_error("no free block when exec KVMemAlloc.");
        return NULL;
    }

    // 从空闲内存块链表中取出一个内存块
    void *ptr = memoryPool->currMemPtr;
    memoryPool->currMemPtr = *(uint8_t **)ptr;
    memoryPool->freeBlockCnt--;
    return ptr;
}

void KVMemPoolFree(KVMemoryPoolT *memoryPool, void *ptr)
{
    DB_POINT(memoryPool);
    // 将ptr指向的内存块插入空闲内存块链表
    *(uint8_t **)ptr = memoryPool->currMemPtr;
    memoryPool->currMemPtr = ptr;
    memoryPool->freeBlockCnt++;
}