/* This file is part of libcnt.
 *
 * Copyright (C) 2005 Cedric Cellier.
 *
 * Libcnt is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * Libcnt is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MicroModel; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
/** \file mem.h
 * Manage a memory area of determined size.
 *
 * Some programs can make use of memory if available to raise performences.
 * Then, the user must provide a memory size that the program is allowed to
 * use.
 * Some functions can then tell what's the memory consuption of the program.
 *
 * Of course, there is only one spool available per process.
 *
 * Some functions can register adresses : that is, they wan't the bloc which
 * countain that adress, if any, to stay in memory even if the creator of the bloc
 * unregister it (references count).
 *
 */

#ifndef CNT_MEM_H_041026
#define CNT_MEM_H_041026
#include <stdlib.h>
#include <stdbool.h>
/** Initialisation.
 * Must be called at the begining or after mem_end().
 * The given size is only used as an indication.
 * \param pool_size the size in bytes of the spool (>0)
 * \return 0 on error, or 1
 * \sa mem_end
 */
int mem_init(unsigned pool_size);
/** Close the service.
 * Must be called after all alloced blocks have been freed. Will
 * check and wine if some blocks were not unregistered.
 * \sa mem_init,mem_unregister
 */
void mem_end(void);
/** To get some bytes from the spool.
 * This is not an error when the declared size of the spool is exceeded.
 * The macro mem_alloc(size) is meant to be used instead.
 * \param size size of the block to allocate
 * \param catcherr if set, abort in case of error
 * \return 0 if malloc fails
 * \sa mem_alloc
 */
void *mem_alloc_d(unsigned size, char const *, size_t, bool catcherr);
/** To be used instead of mem_alloc_d.
 * \sa mem_alloc_d
 */
#define mem_alloc(size) mem_alloc_d(size, __FILE__, __LINE__, false)
#define mem_alloc_catcherr(size) mem_alloc_d(size, __FILE__, __LINE__, true)
/** Resize an allocated block
 * Like the system call, this function returns the new adress of the block, wich
 * datas are unchanged, or NULL if the realloc fails, in wich case the former block
 * still exists.
 * \param block address of the block to realloc
 * \param new_size the new size
 * \return new address or NULL if error
 */
void *mem_realloc(void *block, unsigned new_size);
/** Unregister the memory block so that it can be freed.
 * The registered references to each block are counted.
 * When a block is no more registered it is freed at once.
 * \param block address of the block to unregister. If NULL, no action is taken.
 * \sa mem_register
 */
void mem_unregister(void *block);
/** Register the memory block countaining the adress given.
 * The block won't get freed until we unregister from it
 * \param block address of the block to register
 * \return 0 if no blocs countained that adress, 1 othewise
 */
int mem_register(void *block);
/** Print memory "consumption" */
int mem_consumption(void);
/** Get the size of a previously allocated block
 * \param block address of the block
 * \return the size of this block
 */
unsigned mem_blocksize(const void *block);
#ifndef NDEBUG
/** Print all blocks header and check they are valid */
void mem_print(void);
void mem_whohas(void *addr);
#endif

#endif
// vi:ts=3:sw=3
