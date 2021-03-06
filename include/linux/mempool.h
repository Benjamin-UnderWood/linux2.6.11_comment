/*
 * memory buffer pool support
 */
#ifndef _LINUX_MEMPOOL_H
#define _LINUX_MEMPOOL_H

#include <linux/wait.h>

typedef void * (mempool_alloc_t)(int gfp_mask, void *pool_data);
typedef void (mempool_free_t)(void *element, void *pool_data);

/**
 * 内存池描述符
 */
typedef struct mempool_s {
	/**
	 * 用来保护对象字段的自旋锁。
	 */
	spinlock_t lock;
	/**
	 * 内存池中元素的最大个数。
	 */
	int min_nr;		/* nr of elements at *elements */
	/**
	 * 当前内存池中元素的个数。
	 */
	int curr_nr;		/* Current nr of elements at *elements */
	/**
	 * 指向一个数组的指针，该数组由指向保留元素的指针组成。
	 */
	void **elements;

	/**
	 * 池的拥有者可获得的私有数据。
	 */
	void *pool_data;
	/**
	 * 分配一个元素的方法。
	 */
	mempool_alloc_t *alloc;
	/**
	 * 释放一个元素的方法。
	 */
	mempool_free_t *free;
	/**
	 * 当内存池为空时使用的等待队列。
	 */
	wait_queue_head_t wait;
} mempool_t;
extern mempool_t * mempool_create(int min_nr, mempool_alloc_t *alloc_fn,
				 mempool_free_t *free_fn, void *pool_data);
extern int mempool_resize(mempool_t *pool, int new_min_nr, int gfp_mask);
extern void mempool_destroy(mempool_t *pool);
extern void * mempool_alloc(mempool_t *pool, int gfp_mask);
extern void mempool_free(void *element, mempool_t *pool);

/*
 * A mempool_alloc_t and mempool_free_t that get the memory from
 * a slab that is passed in through pool_data.
 */
void *mempool_alloc_slab(int gfp_mask, void *pool_data);
void mempool_free_slab(void *element, void *pool_data);

#endif /* _LINUX_MEMPOOL_H */
