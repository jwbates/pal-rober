#ifndef _COMMON_H
#define _COMMON_H

#include "/Users/jwbates/Projects/pal/src/include/color.h"

#define MASTER_ADDRESS     10
#define SLAVE_ADDRESS      9

#define REQUEST_SIZE              32

#define START_CAPTURE_REQUEST     0x01

#define BYTES_PER_PIXEL   2

#define IMAGE_WIDTH       320
#define IMAGE_HEIGHT      240

#define IMAGE_BYTES       IMAGE_WIDTH*IMAGE_HEIGHT*BYTES_PER_PIXEL

#define CLUSTER_COUNT     6

#endif /* _COMMON_H */
