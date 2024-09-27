#include "radxa_overlay.h"

static int level = 0;
#define DBG(format,...) \
    do { \
        if (level > 0) { \
            printf(format, ##__VA_ARGS__); \
        } \
    } while (0)

int radxa_display_overlay(const void *blob)
{
	DBG("%s\n",__func__);

	struct blk_desc *dev_desc;
	dev_desc = rockchip_get_bootdev();
	if (!dev_desc) {
		printf("%s: dev_desc is NULL!\n", __func__);
		return -ENODEV;
	}

	printf("%s...%d:%d\n",__func__,dev_desc->if_type,dev_desc->devnum);
	return 0;
}
