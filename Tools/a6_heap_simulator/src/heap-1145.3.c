#include <arm.h>
#include <heap-1145.3.h>

extern unsigned int gHeapAvailable;
extern free_block_t **gHeapBins;

/*
 * Getting the next block
 */

heap_block_t *next_block(heap_block_t *heap_block)
{
  return &heap_block[heap_block->cur_size];
}

/*
 * Getting the previous block
 */

heap_block_t *prev_block(heap_block_t *heap_block)
{
  return &heap_block[-(heap_block->prev_size >> 2)];
}

/*
 * Quantifying size, i.e. converting bytes into block count
 */

unsigned int quantify_size(unsigned int size)
{
  return size / HEAP_BLOCKSIZE;
}

/*
 * Finding a suitable bin for size
 */

unsigned int compute_bin(unsigned int size)
{
  return NUM_OF_BINS - __clz(quantify_size(size));
}

/*
 * Extends size to fill a whole number of blocks
 */

unsigned int required_size(int size)
{
  unsigned int result;

  result = (size + (2 * HEAP_BLOCKSIZE - 1)) & ~(HEAP_BLOCKSIZE - 1);

  if (result < sizeof(free_block_t))
    result = sizeof(free_block_t);

  return result;
}

/*
 * Converting a free block to an allocated one,
 * and optionally splitting it
 */

void assign_chunk(heap_block_t *block, unsigned int size)
{
  free_block_t *new_block;
  heap_block_t *next_block_;
  unsigned int quantified_size;
  unsigned int suitable_bin_rel;
  free_block_t **suitable_bin_ptr;
  free_block_t *suitable_bin_element;

  /*
   * If the block cannot hold its own size + size of free_block_t,
   * then we are not splitting anything
   */

  if (size + sizeof(free_block_t) > block->cur_size * HEAP_BLOCKSIZE)
  {
    /*
     * Getting next block and unsetting FLAG_PREV_FREE to it
     */

    next_block_ = next_block(block);
    next_block_->prev_size &= ~(FLAG_PREV_FREE);
  }

  /*
   * ...otherwise splitting it in two
   */

  else
  {
    /*
     * Calculating the new block's position
     */

    new_block = (free_block_t *)((void *)block + size);

    /*
     * Getting a next already created block
     */

    next_block_ = next_block(block);

    /*
     * Clearing the new block's prev_size
     */

    new_block->block.prev_size = (new_block->block.prev_size & ~(FLAG_PREV_FREE)) | FLAG_THIS_FREE;

    /*
     * Quantifying size of the current block,
     * i.e. converting bytes to block counts
     */

    quantified_size = quantify_size(size);

    /*
     * Setting the new block's prev_size and cur_size
     */

    new_block->block.prev_size = (new_block->block.prev_size & (FLAG_PREV_FREE | FLAG_THIS_FREE)) | quantified_size << 2;
    new_block->block.cur_size = block->cur_size - (quantified_size & 0x3FFFFFFF);

    /*
     * Setting the current block's cur_size
     */

    block->cur_size = quantified_size;

    /*
     * Setting the next block's prev_size
     */

    next_block_->prev_size = (next_block_->prev_size & (FLAG_PREV_FREE | FLAG_THIS_FREE)) | new_block->block.cur_size << 2;

    /*
     * Finding a suitable bin for the new block
     * and reassigning the linked list
     */

    suitable_bin_rel = sizeof(free_block_t *) * compute_bin(new_block->block.cur_size * HEAP_BLOCKSIZE);
    suitable_bin_ptr = (free_block_t **)((void *)gHeapBins + suitable_bin_rel);
    suitable_bin_element = *suitable_bin_ptr;

    new_block->free_list.bkd = (free_list_t *)suitable_bin_ptr;
    new_block->free_list.fwd = suitable_bin_element;

    if (suitable_bin_element)
      suitable_bin_element->free_list.bkd = &new_block->free_list;

    *suitable_bin_ptr = new_block;
  }

  /*
   * Clearing FLAG_THIS_FREE from the current block
   */

  block->prev_size &= ~(FLAG_THIS_FREE);
}

/*
 * Allocating some memory
 */

void *heap_malloc(unsigned int size)
{
  unsigned int required_size_;
  unsigned int suitable_bin;
  free_block_t **suitable_bin_ptr;
  free_block_t *i;
  free_block_t *next_free_block;

  /*
   * Not allowing zero-sized buffers
   */

  if (!size)
panic_cause:
    panic("_malloc", "_malloc must allocate at least one byte");

  /*
   * Disabling interrupts to avoid race conditions
   */

  enter_critical_section();

  /*
   * Normalizing size and findning a minimal suitable bin
   */

  required_size_ = required_size(size);
  suitable_bin = compute_bin(required_size_);
  suitable_bin_ptr = (free_block_t **)(gHeapBins + suitable_bin);

  /*
   * Panicking if we got out of bins
   */

next_bin:
  if (suitable_bin > NUM_OF_BINS - 1)
    goto panic_cause;

  /*
   * Finding a suitable block by iterating through bins
   */

  for (i = *suitable_bin_ptr; ; i = i->free_list.fwd)
  {
    if (!i)
    {
      ++suitable_bin;
      ++suitable_bin_ptr;
      goto next_bin;
    }

    if (required_size_ <= i->block.cur_size * HEAP_BLOCKSIZE)
      break;
  }

  /*
   * Reassigning the linked list
   */

  next_free_block = i->free_list.fwd;

  i->free_list.bkd->fwd = next_free_block;

  if (next_free_block)
    next_free_block->free_list.bkd = i->free_list.bkd;

  /*
   * Converting the free block we just found
   * to an occupied one
   */

  assign_chunk((heap_block_t *)i, required_size_);

  /*
   * Decrementing gHeapAvailable
   */

  gHeapAvailable -= i->block.cur_size * HEAP_BLOCKSIZE;

  /*
   * Reenabling interupts
   */

  exit_critical_section();

  /*
   * Returning actual buffer to a client
   */

  return (void *)i + sizeof(heap_block_t);
}

/*
 * Freeing some memory
 */

void heap_free(void *buffer)
{
  heap_block_t *cur_block;
  heap_block_t *next_block_;
  heap_block_t *prev_block_;
  char old_flags;
  char new_flags;
  free_block_t *next_block_as_free;
  free_block_t *prev_block_as_free;
  free_block_t *prev_block_fwd;
  free_block_t *next_block_fwd;
  heap_block_t *next_block_after_free;
  unsigned int suitable_bin_rel;
  free_block_t **suitable_bin_ptr;
  free_block_t *suitable_bin_start;

  /*
   * Only working with non-NULL buffers obviously
   */

  if (buffer)
  {
    /*
     * Disabling interrupts to avoid race conditions
     */

    enter_critical_section();

    /*
     * Getting the allocation's metadata
     */

    cur_block = (heap_block_t *)buffer - 1;

    /*
     * Incrementing gHeapAvailable
     */

    gHeapAvailable += cur_block->cur_size * HEAP_BLOCKSIZE;

    /*
     * Getting next/previous blocks and also current flags
     */

    next_block_ = next_block(cur_block);
    prev_block_ = prev_block(cur_block);
    old_flags = *((char *)cur_block);

    /*
     * Metadata sanity checks
     */

    if ( next_block_ == cur_block
      || prev_block_ == cur_block
      || prev_block(next_block_) != cur_block
      || next_block(prev_block_) != cur_block
      || old_flags & FLAG_THIS_FREE)
    {
      panic("_free", "heap overflow");
    }

    /*
     * Setting FLAG_THIS_FREE to true
     */

    new_flags = old_flags | FLAG_THIS_FREE;
    *((char *)cur_block) = new_flags;

    /*
     * Getting next/previous blocks yet again.
     * This time will treat them as free ones
     */

    next_block_as_free = (free_block_t *)next_block(cur_block);
    prev_block_as_free = (free_block_t *)prev_block(cur_block);

    /*
     * If the previous block is free,
     * glueing our current one with it
     */

    if (new_flags & FLAG_PREV_FREE)
    {
      /*
       * Reassigning the linked list
       */

      prev_block_fwd = prev_block_as_free->free_list.fwd;
      prev_block_as_free->free_list.bkd->fwd = prev_block_fwd;

      if (prev_block_fwd)
        prev_block_fwd->free_list.bkd = prev_block_as_free->free_list.bkd;

      /*
       * Increasing size of the previous with ours
       */

      prev_block_as_free->block.cur_size += cur_block->cur_size;

      /*
       * Clearing the current block's metadata
       */

      cur_block->cur_size = 0;
      cur_block->prev_size &= (FLAG_THIS_FREE | FLAG_PREV_FREE);

      /*
       * From now on, working with the previous block
       */

      cur_block = (heap_block_t *)prev_block_as_free;
    }

    /*
     * If the next block is free,
     * glueing our current one with it
     */

    if (next_block_as_free->block.prev_size & FLAG_THIS_FREE)
    {
      /*
       * Reassigning the linked list
       */

      next_block_fwd = next_block_as_free->free_list.fwd;
      next_block_as_free->free_list.bkd->fwd = next_block_fwd;

      if (next_block_fwd)
        next_block_fwd->free_list.bkd = next_block_as_free->free_list.bkd;

      /*
       * Increasing size of ours with the next's
       */

      cur_block->cur_size += next_block_as_free->block.cur_size;

      /*
       * Clearing the next block's metadata
       */

      next_block_as_free->block.cur_size = 0;
      next_block_as_free->block.prev_size &= (FLAG_THIS_FREE | FLAG_PREV_FREE);
    }

    /*
     * Getting next block yet again
     * after the possible procedures above
     */

    next_block_after_free = next_block(cur_block);

    /*
     * Setting flags to the current and the next blocks 
     */

    cur_block->prev_size |= FLAG_THIS_FREE;
    next_block_after_free->prev_size |= FLAG_PREV_FREE;

    /*
     * Setting prev_size to the next block
     */

    next_block_after_free->prev_size = (next_block_after_free->prev_size & (FLAG_THIS_FREE | FLAG_PREV_FREE))
                                                                        | cur_block->cur_size << 2;

    /*
     * Finding a suitable bin and reassigning the linked list
     */

    suitable_bin_rel = sizeof(free_block_t *) * compute_bin(cur_block->cur_size * HEAP_BLOCKSIZE);
    suitable_bin_ptr = (free_block_t **)((void *)gHeapBins + suitable_bin_rel);
    suitable_bin_start = *suitable_bin_ptr;

    ((free_block_t *)cur_block)->free_list.bkd = (free_list_t *)suitable_bin_ptr;
    ((free_block_t *)cur_block)->free_list.fwd = suitable_bin_start;

    if (suitable_bin_start)
      suitable_bin_start->free_list.bkd = &(((free_block_t *)cur_block)->free_list);

    *suitable_bin_ptr = (free_block_t *)cur_block;

    /*
     * All done, reenabling interrupts
     */

    exit_critical_section();
  }
}
