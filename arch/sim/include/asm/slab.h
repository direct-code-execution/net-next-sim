#ifndef _ASM_SIM_SLAB_H
#define _ASM_SIM_SLAB_H

void *kmalloc(size_t size, gfp_t flags);
void *__kmalloc(size_t size, gfp_t flags);
void *kmem_cache_alloc(struct kmem_cache *, gfp_t);

#endif /* _ASM_SIM_SLAB_H */
