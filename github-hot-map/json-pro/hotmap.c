/* example1.c - first example in the programming guide
 *
 * This file is Copyright (c) 2014 by Eric S. Raymond
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "mjson.h"

#define HOTMAP_ELEM_SIZE    16
#define HOTMAP_DAY_NUMS     7
#define TEMP_BUFFER_SIZE    512

typedef struct {
    uint8_t color[HOTMAP_ELEM_SIZE];
    uint8_t date[HOTMAP_ELEM_SIZE];
    uint32_t contributionCount;
} hotmap_t;

typedef struct {
    uint32_t hotmap_nums;
    hotmap_t hotmap_data[HOTMAP_DAY_NUMS];
} hotmap_list_t;

char hotmapbuffer[TEMP_BUFFER_SIZE];
char *buffer_index= NULL;

uint32_t json_object_list_read(char *buf, hotmap_list_t *hotmap_list)
{
    /* mapping the hotmap_t json data */
    const struct json_attr_t json_attrs_subobject[] = {
        {"color",              t_string,  STRUCTOBJECT(hotmap_t, color), .len = sizeof(hotmap_list->hotmap_data[0].color)},
        {"date",               t_string,  STRUCTOBJECT(hotmap_t, date),  .len = sizeof(hotmap_list->hotmap_data[0].date)},
        {"contributionCount",  t_integer, STRUCTOBJECT(hotmap_t, contributionCount)},
        {NULL},
    };

    /* maping the hotmap_list json data */
    const struct json_attr_t json_attrs_objects[] = {
        {"class",             t_check, .dflt.check = "OBJECTS"},
        {"contributionDays",  t_array, STRUCTARRAY(hotmap_list->hotmap_data, json_attrs_subobject, &(hotmap_list->hotmap_nums))},
        {NULL},
    };

    memset(hotmap_list, '\0', sizeof(*hotmap_list));
    return json_read_object(buf, json_attrs_objects, NULL);

}

int main(int argc, char *argv[])
{
    int status = 0;

    /* Allocate space for object list */ 
    hotmap_list_t *hotmap_list = malloc(sizeof(hotmap_list_t));

    /* memcpy the data from orignal to buffer */
    buffer_index = argv[1] + 77;
    printf("1: %s\n", buffer_index);
    memcpy(hotmapbuffer, buffer_index, 457);
    printf("2: %s\n", hotmapbuffer);

    /* Call object list parsing function */
    status = json_object_list_read(hotmapbuffer, hotmap_list);

    if (status == 0) 
    {
        for (uint32_t index = 0; index < hotmap_list->hotmap_nums; index++) {
            printf(" Object:             %d\n", index);
            printf("  color:             %s\n",  hotmap_list->hotmap_data[index].color);
            printf("  date:              %s\n",  hotmap_list->hotmap_data[index].date);
            printf("  contributionCount: %d\n",  hotmap_list->hotmap_data[index].contributionCount);
        }
    } 
    else 
    {
        puts(json_error_string(status));
    }
    
    return status;
}

/* end */
