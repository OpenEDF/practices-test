/* example1.c - first example in the programming guide
 *
 * This file is Copyright (c) 2014 by Eric S. Raymond
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "mjson.h"

static char color[16], date[16];
static int contributionCount;
//char *test_data = "{\"data\":{\"user\":{\"contributionsCollection\":{\"contributionCalendar\":{\"weeks\":[{\"contributionDays\":[{\"color\":\"#216e39\",\"date\":\"2021-08-25\",\"contributionCount\":13}]}]}}}}}";
char *test_data = "{\"color\":\"#216e39\",\"date\":\"2021-08-25\",\"contributionCount\":13}";
static const struct json_attr_t json_attrs[] = {
    {"color",   t_string, .addr.string = color, .len = sizeof(color)},
    {"date",   t_string, .addr.string = date, .len = sizeof(date)},
    {"count",   t_integer, .addr.integer = &contributionCount,},
    {NULL},
};

int main(int argc, char *argv[])
{
    int status = 0;

    status = json_read_object(test_data, json_attrs, NULL);
    printf("status = %d, color = %s, date = %s, contributionCount = %d\n",
	   status, color, date,  contributionCount);
    if (status != 0)
	puts(json_error_string(status));
}

/* end */
