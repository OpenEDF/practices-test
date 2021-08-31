/**
  ******************************************************************************
  * @file    malloc.h
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    26-August-2021
  * @brief   This file contains all the functions prototypes and variable struct
  *			 for malloc free function.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2020 Macro</center></h2>
  *
  * Licensed emmmmmmmmmm.....
  * 
  * I agree that my code agrees to all  the  restrictions  and  free  of charge 
  * stipulated by the GNU General Public License, because I believe that GNU is  
  * full of energy, appeal, and magic. No one needs my consent to use this code,  
  * if you are using it I found a bug in the  process. I  hope  you can give me
  * feedback. I am very happy to learn from people who are better than me.
  *
  ******************************************************************************
  */
/*         The Embedded Memory Layout
 *      -----------------------------------
 *              Stack (grow down)
 *                    |
 *                   \|/
 *      -----------------------------------
 *            Memory Mapping Segment
 *                    |
 *                   \|/
 *      -----------------------------------
 *                   /|\
 *                    |
 *                   Heap
 *      -----------------------------------
 *
 *                BSS segment
 *      -----------------------------------
 *
 *               Data segment
 *      -----------------------------------
 *
 *              Text segment (ELF)
 *      -----------------------------------
 * */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MALLOC__H__
#define __MALLOC__H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/** @addtogroup malloc_free
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* 
 * enum: @bool_e.
 */
/* The TRUE and FLASE is not impletement in C */
typedef enum { FALSE, TRUE } bool_e;         
/* 
 * struct: @block_t.
 */
/* a block stores data for each malloc call */
typedef struct s_block {
    struct s_block *prev;    /* The previous element for block */
    struct s_block *next;    /* The next element for block */
    void *block_start_addr;  /* The current block start address */
    size_t block_size;       /* The size for allocate */
    bool_e freed_state;      /* The state for block */
} block_t;

/*
 * TODO: Initalization and allocate first block should set tail list.
 * */

/* 
 * struct: @heap_t.
 */
/* heap stores data about one mapped zone */
typedef struct {
    void  *heap_start_addr;      /* The heap start address */
    size_t heap_total_size;      /* The heap total length */
    size_t heap_free_size;       /* The heap free size */  //TODO: There are too many pieces to make it meaningless.
    size_t block_count;          /* The heap block counter */
} heap_t;
         
/* Exported constants --------------------------------------------------------*/
	
/* Exported macro ------------------------------------------------------------*/

/* 
 * macro: @.
 */
/*  */


/* Exported functions --------------------------------------------------------*/
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void *calloc(size_t count, size_t size);
void free(void *ptr);

#ifdef __cplusplus
}
#endif

#endif  /*__MALLOC__H__*/

/**
  * @}
  */

/**
  * @}
  */

/***************************** END OF FILE *************************************/
