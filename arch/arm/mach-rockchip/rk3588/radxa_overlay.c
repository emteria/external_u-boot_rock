#include "radxa_overlay.h"

struct overlay_info overlay_info;

static int level = 0;
#define DBG(format,...) \
    do { \
        if (level > 0) { \
            printf(format, ##__VA_ARGS__); \
        } \
    } while (0)


int radxa_read_screen_info(const void *blob)
{
	u32 size,j = 0;
	u8 *buffer = NULL;
	int ret = 0;

	DBG("%s\n",__func__);
	size = sizeof(overlay_info.screen_info[0])*SCREEN_TYPE + SCREEN_TYPE + 1;
	buffer = (u8*)malloc(size);
	if (buffer == NULL) {
        printf("Memory allocation failed\n");
        return -1;
    }
	ret = vendor_storage_read(VENDOR_RADXA_DISPLAY_ID, buffer, size);
	if (ret < 0) {
			printf("vendor read failed(id=%d)!\n", VENDOR_RADXA_DISPLAY_ID);
			free(buffer);
			return ret;
	}

	overlay_info.screen_num = buffer[j++] - '0'; //ascii to int
	DBG("id=%03d,screen_num =%u,size=%d.\n", VENDOR_RADXA_DISPLAY_ID, overlay_info.screen_num, size);

	for(int i = 0; i < overlay_info.screen_num; i++){
		DBG("buffer %c \n",buffer[j]);
		if(buffer[j] != '-'){
			overlay_info.screen_info[i].type = buffer[j++] - '0';
			overlay_info.screen_info[i].id = buffer[j++] - '0';
			overlay_info.screen_info[i].vop = buffer[j++] - '0';
			overlay_info.screen_info[i].enable = buffer[j++] - '0';
			printf("screen_info type=%d id=%d vop =%d enable=%d \n",\
				overlay_info.screen_info[i].type, \
				overlay_info.screen_info[i].id,	\
				overlay_info.screen_info[i].vop, \
				overlay_info.screen_info[i].enable);
		}else{
			j++;
			i--; //skip '-'
		}
	}
	free(buffer);
	return ret;
}

void radxa_fdt_fixup_logo(const void *blob,const char *vp_to_display,const char *route_logo)
{
	int node;

	node = fdt_path_offset(blob, vp_to_display);
	if (node < 0) {
		printf("%s %s Path not found\n",__func__,vp_to_display);
	} else {
		int phandle = fdt_get_phandle(blob,node);
		node = fdt_path_offset(blob, route_logo);//LOGO
		if (node < 0)
			printf("%s %s Path not found\n",__func__,route_logo);
		else
			fdt_setprop_u32((char*)blob, node, "connect", phandle);
	}

}


void radxa_fdt_status_disabled(const void *blob,char *path)
{
	int nodeoffset;
	int ret;
	nodeoffset = fdt_path_offset(blob, path);
	if (nodeoffset < 0) {
		printf("%s %s Path not found\n",__func__,path);
		return ;
	}
	ret = fdt_status_disabled((char*)blob,nodeoffset);

    DBG("%s..path %s ret = %d \n",__func__,path,ret);
}

void radxa_fdt_status_okay(const void *blob,char *path)
{
	int nodeoffset;
	int ret;
	nodeoffset = fdt_path_offset(blob, path);
	if (nodeoffset < 0) {
		printf("%s %s  %d Path not found\n",__func__,path,nodeoffset);
		return ;
	}
	ret = fdt_status_okay((char*)blob,nodeoffset);

    DBG("%s..path %s ret = %d \n",__func__,path,ret);
}

void radxa_fdt_set_panel_clk(const void *blob,char *path,int clk_rate)
{
	int nodeoffset;

	nodeoffset = fdt_path_offset(blob,path);
	if (nodeoffset < 0) {
		printf("Node %s not found in device tree\n", path);
		return;
	}

	if (fdt_setprop_u32((char*)blob, nodeoffset, DSI_CLK_RATE, clk_rate) < 0) {
		printf("Failed to set property %s\n", DSI_CLK_RATE);
		return;
	}
}


void radxa_fdt_select_panel(const void *blob,struct screen_info screen_info)
{
	int nodeoffset;
	void *fdt = (void*)blob;

	if(screen_info.type == RADXA_CONNECTOR_DSI0 || screen_info.type == RADXA_CONNECTOR_DSI1){
		if (screen_info.type == RADXA_CONNECTOR_DSI0){
			nodeoffset = fdt_path_offset(fdt,DSI0_PANEL_NODE_FDT_PATH);
			if (nodeoffset < 0) {
				printf("Node %s not found in device tree\n", DSI0_PANEL_NODE_FDT_PATH);
				return;
			}

			if(screen_info.id == 0){
				if (fdt_setprop(fdt, nodeoffset, "compatible", DSI_PANEL0, strlen(DSI_PANEL0) + 1) < 0){
					printf("Failed to set compatible DSI_PANEL0\n");
					return;
				}
				radxa_fdt_set_panel_clk(fdt,DSI0_NODE_FDT_PATH,DSI_CLK_480);
			}else if (screen_info.id == 1){
				if (fdt_setprop(fdt, nodeoffset, "compatible", DSI_PANEL1, strlen(DSI_PANEL1) + 1) < 0){
					printf("Failed to set compatible DSI_PANEL1\n");
					return;
				}
				radxa_fdt_set_panel_clk(fdt,DSI0_NODE_FDT_PATH,DSI_CLK_480);
			}else if (screen_info.id == 2){
				if (fdt_setprop(fdt, nodeoffset, "compatible", DSI_PANEL2, strlen(DSI_PANEL2) + 1) < 0){
					printf("Failed to set compatible DSI_PANEL2\n");
					return;
				}
				radxa_fdt_set_panel_clk(fdt,DSI0_NODE_FDT_PATH,DSI_CLK_1000);
			}else if (screen_info.id == 3){
				if (fdt_setprop(fdt, nodeoffset, "compatible", DSI_PANEL3, strlen(DSI_PANEL3) + 1) < 0){
					printf("Failed to set compatible DSI_PANEL3\n");
					return;
				}
				radxa_fdt_set_panel_clk(fdt,DSI0_NODE_FDT_PATH,DSI_CLK_605);
			}else if (screen_info.id == 4){
				if (fdt_setprop(fdt, nodeoffset, "compatible", DSI_PANEL4, strlen(DSI_PANEL4) + 1) < 0){
					printf("Failed to set compatible DSI_PANEL4\n");
					return;
				}
				radxa_fdt_set_panel_clk(fdt,DSI0_NODE_FDT_PATH,DSI_CLK_605);
			}else if (screen_info.id == 5){
				if (fdt_setprop(fdt, nodeoffset, "compatible", DSI_PANEL5, strlen(DSI_PANEL5) + 1) < 0){
					printf("Failed to set compatible DSI_PANEL5\n");
					return;
				}
				radxa_fdt_set_panel_clk(fdt,DSI0_NODE_FDT_PATH,DSI_CLK_605);
			}

			if(screen_info.id < 3){//radxa  panel
				radxa_fdt_status_disabled(fdt,DSI0_I2C6_RP_FT_TOUCH);
				
				radxa_fdt_status_disabled(fdt,DSI0_I2C5_RP_FT_TOUCH);
				if (screen_info.id == 0){
					nodeoffset = fdt_path_offset(fdt,DSI0_I2C6_RADXA_GT_TOUCH);
					fdt_setprop_u32((char*)fdt, nodeoffset, GT_TOUCH_SIZE, GT_TOUCH_9112);

					nodeoffset = fdt_path_offset(fdt,DSI0_I2C5_RADXA_GT_TOUCH);
					fdt_setprop_u32((char*)fdt, nodeoffset, GT_TOUCH_SIZE, GT_TOUCH_9112);
				}else if(screen_info.id == 2){
					nodeoffset = fdt_path_offset(fdt,DSI0_I2C6_RADXA_GT_TOUCH);
					fdt_setprop_u32((char*)fdt, nodeoffset, GT_TOUCH_SIZE, GT_TOUCH_9271);

					nodeoffset = fdt_path_offset(fdt,DSI0_I2C5_RADXA_GT_TOUCH);
					fdt_setprop_u32((char*)fdt, nodeoffset, GT_TOUCH_SIZE, GT_TOUCH_9271);
				}
			}else if(screen_info.id >= 3 && screen_info.id < 6){ //raspberrypi panel
				radxa_fdt_status_disabled(fdt,DSI0_I2C6_RADXA_FT_TOUCH);

				radxa_fdt_status_disabled(fdt,DSI0_I2C5_RADXA_FT_TOUCH);
			}

		} else if(screen_info.type == RADXA_CONNECTOR_DSI1) {
			nodeoffset = fdt_path_offset(fdt,DSI1_PANEL_NODE_FDT_PATH);
			if (nodeoffset < 0) {
				printf("Node %s not found in device tree\n", DSI1_PANEL_NODE_FDT_PATH);
				return;
			}

			if(screen_info.id == 0){
				if (fdt_setprop(fdt, nodeoffset, "compatible", DSI_PANEL0, strlen(DSI_PANEL0) + 1) < 0){
					printf("Failed to set compatible DSI_PANEL0\n");
					return;
				}
				radxa_fdt_set_panel_clk(fdt,DSI1_NODE_FDT_PATH,DSI_CLK_480);
			}else if (screen_info.id == 1){
				if (fdt_setprop(fdt, nodeoffset, "compatible", DSI_PANEL1, strlen(DSI_PANEL1) + 1) < 0){
					printf("Failed to set compatible DSI_PANEL1\n");
					return;
				}
				radxa_fdt_set_panel_clk(fdt,DSI1_NODE_FDT_PATH,DSI_CLK_480);
			}else if (screen_info.id == 2){
				if (fdt_setprop(fdt, nodeoffset, "compatible", DSI_PANEL2, strlen(DSI_PANEL2) + 1) < 0){
					printf("Failed to set compatible DSI_PANEL2\n");
					return;
				}
				radxa_fdt_set_panel_clk(fdt,DSI1_NODE_FDT_PATH,DSI_CLK_1000);
			}else if (screen_info.id == 3){
				if (fdt_setprop(fdt, nodeoffset, "compatible", DSI_PANEL3, strlen(DSI_PANEL3) + 1) < 0){
					printf("Failed to set compatible DSI_PANEL3\n");
					return;
				}
				radxa_fdt_set_panel_clk(fdt,DSI1_NODE_FDT_PATH,DSI_CLK_605);
			}else if (screen_info.id == 4){
				if (fdt_setprop(fdt, nodeoffset, "compatible", DSI_PANEL4, strlen(DSI_PANEL4) + 1) < 0){
					printf("Failed to set compatible DSI_PANEL4\n");
					return;
				}
				radxa_fdt_set_panel_clk(fdt,DSI1_NODE_FDT_PATH,DSI_CLK_605);
			}else if (screen_info.id == 5){
				if (fdt_setprop(fdt, nodeoffset, "compatible", DSI_PANEL5, strlen(DSI_PANEL5) + 1) < 0){
					printf("Failed to set compatible DSI_PANEL5\n");
					return;
				}
				radxa_fdt_set_panel_clk(fdt,DSI1_NODE_FDT_PATH,DSI_CLK_605);
			}

			if(screen_info.id < 3){//radxa  panel
				radxa_fdt_status_disabled(blob,DSI1_I2C8_RP_FT_TOUCH);
				if (screen_info.id == 0){
					nodeoffset = fdt_path_offset(fdt,DSI1_I2C8_RADXA_GT_TOUCH);
					fdt_setprop_u32((char*)fdt, nodeoffset, GT_TOUCH_SIZE, GT_TOUCH_9112);
				}else if(screen_info.id == 2){
					nodeoffset = fdt_path_offset(fdt,DSI1_I2C8_RADXA_GT_TOUCH);
					fdt_setprop_u32((char*)fdt, nodeoffset, GT_TOUCH_SIZE, GT_TOUCH_9271);
				}
			}else if(screen_info.id >= 3 && screen_info.id < 6){ //raspberrypi panel
				radxa_fdt_status_disabled(blob,DSI1_I2C8_RADXA_FT_TOUCH);
			}
		}
	}
}

int radxa_fdt_fixup_screen(const void *blob)
{
	int ret = 0;
	int node;
//	void *fdt = (void*)blob;

	printf("%s\n",__func__);

	fdt_increase_size((void*)blob, SZ_8K);

	for(int i = 0; i < overlay_info.screen_num; i++){
		switch(overlay_info.screen_info[i].type){
			case RADXA_CONNECTOR_HDMI0:
			case RADXA_CONNECTOR_HDMI1:
				if(overlay_info.screen_info[i].type == RADXA_CONNECTOR_HDMI0) { //hdmi0
					node = fdt_path_offset(blob, HDMI0_NODE_FDT_PATH);
					if (node < 0) {
						printf("%s %s Path not found\n",__func__,HDMI0_NODE_FDT_PATH);
					} else {
						if(overlay_info.screen_info[i].enable == FALSE){
							radxa_fdt_status_disabled(blob,HDMI0_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,HDMI0_PHY_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,HDMI0_IN_VP0_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,HDMI0_IN_VP1_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,HDMI0_IN_VP2_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,HDMI0_LOGO_ROUTE_FDT_PATH);
						} else {
							radxa_fdt_status_okay(blob,HDMI0_NODE_FDT_PATH);
							radxa_fdt_status_okay(blob,HDMI0_PHY_NODE_FDT_PATH);
							radxa_fdt_status_okay(blob,HDMI0_LOGO_ROUTE_FDT_PATH);
							if (overlay_info.screen_info[i].vop == VOP0){
								radxa_fdt_status_okay(blob,HDMI0_IN_VP0_NODE_FDT_PATH);
								radxa_fdt_status_disabled(blob,HDMI0_IN_VP1_NODE_FDT_PATH);
								radxa_fdt_status_disabled(blob,HDMI0_IN_VP2_NODE_FDT_PATH);
								radxa_fdt_fixup_logo(blob,VP0_OUT_HDMI0,ROUTE_HDMI0);//logo
							} else if (overlay_info.screen_info[i].vop == VOP1) {
								radxa_fdt_status_disabled(blob,HDMI0_IN_VP0_NODE_FDT_PATH);
								radxa_fdt_status_okay(blob,HDMI0_IN_VP1_NODE_FDT_PATH);
								radxa_fdt_status_disabled(blob,HDMI0_IN_VP2_NODE_FDT_PATH);
								radxa_fdt_fixup_logo(blob,VP1_OUT_HDMI0,ROUTE_HDMI0);
							} else if (overlay_info.screen_info[i].vop == VOP2) {
								radxa_fdt_status_disabled(blob,HDMI0_IN_VP0_NODE_FDT_PATH);
								radxa_fdt_status_disabled(blob,HDMI0_IN_VP1_NODE_FDT_PATH);
								radxa_fdt_status_okay(blob,HDMI0_IN_VP2_NODE_FDT_PATH);
								radxa_fdt_fixup_logo(blob,VP2_OUT_HDMI0,ROUTE_HDMI0);
							}
						}
					}
				} else if (overlay_info.screen_info[i].type == RADXA_CONNECTOR_HDMI1) { //hdmi1
					node = fdt_path_offset(blob, HDMI1_NODE_FDT_PATH);
					if (node < 0) {
						printf("%s %s Path not found\n",__func__,HDMI1_NODE_FDT_PATH);
					} else {
						if(overlay_info.screen_info[i].enable == FALSE){
							radxa_fdt_status_disabled(blob,HDMI1_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,HDMI1_PHY_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,HDMI1_IN_VP0_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,HDMI1_IN_VP1_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,HDMI1_IN_VP2_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,HDMI1_LOGO_ROUTE_FDT_PATH);
						} else {
							radxa_fdt_status_okay(blob,HDMI1_NODE_FDT_PATH);
							radxa_fdt_status_okay(blob,HDMI1_PHY_NODE_FDT_PATH);
							radxa_fdt_status_okay(blob,HDMI1_LOGO_ROUTE_FDT_PATH);
							if (overlay_info.screen_info[i].vop == VOP0){
								radxa_fdt_status_okay(blob,HDMI1_IN_VP0_NODE_FDT_PATH);
								radxa_fdt_status_disabled(blob,HDMI1_IN_VP1_NODE_FDT_PATH);
								radxa_fdt_status_disabled(blob,HDMI1_IN_VP2_NODE_FDT_PATH);
								radxa_fdt_fixup_logo(blob,VP0_OUT_HDMI1,ROUTE_HDMI1);//logo
							} else if (overlay_info.screen_info[i].vop == VOP1) {
								radxa_fdt_status_disabled(blob,HDMI1_IN_VP0_NODE_FDT_PATH);
								radxa_fdt_status_okay(blob,HDMI1_IN_VP1_NODE_FDT_PATH);
								radxa_fdt_status_disabled(blob,HDMI1_IN_VP2_NODE_FDT_PATH);
								radxa_fdt_fixup_logo(blob,VP1_OUT_HDMI1,ROUTE_HDMI1);
							} else if (overlay_info.screen_info[i].vop == VOP2) {
								radxa_fdt_status_disabled(blob,HDMI1_IN_VP0_NODE_FDT_PATH);
								radxa_fdt_status_disabled(blob,HDMI1_IN_VP1_NODE_FDT_PATH);
								radxa_fdt_status_okay(blob,HDMI1_IN_VP2_NODE_FDT_PATH);
								radxa_fdt_fixup_logo(blob,VP2_OUT_HDMI1,ROUTE_HDMI1);
							}
						}
					}
				} else {
					printf("%s Invalid ID \n",__func__);
				}
				break;

			case RADXA_CONNECTOR_DP0:
			case RADXA_CONNECTOR_DP1:
				if(overlay_info.screen_info[i].type == RADXA_CONNECTOR_DP0) { //dp0
					node = fdt_path_offset(blob, DP0_NODE_FDT_PATH);
					if (node < 0) {
						printf("%s %s Path not found\n",__func__,DP0_NODE_FDT_PATH);
					} else {
						if(overlay_info.screen_info[i].enable == FALSE){
							radxa_fdt_status_disabled(blob,DP0_NODE_FDT_PATH);
							//radxa_fdt_status_disabled(blob,DP0_USBDP_PHY_NODE_FDT_PATH);//phy usb/dp share
							radxa_fdt_status_disabled(blob,DP0_IN_VP0_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,DP0_IN_VP1_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,DP0_IN_VP2_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,DP0_LOGO_ROUTE_FDT_PATH);
						} else {
							radxa_fdt_status_okay(blob,DP0_NODE_FDT_PATH);
							//radxa_fdt_status_okay(blob,DP0_USBDP_PHY_NODE_FDT_PATH);//phy usb/dp share
							radxa_fdt_status_okay(blob,DP0_LOGO_ROUTE_FDT_PATH);
							if (overlay_info.screen_info[i].vop == VOP0){
								radxa_fdt_status_okay(blob,DP0_IN_VP0_NODE_FDT_PATH);
								radxa_fdt_status_disabled(blob,DP0_IN_VP1_NODE_FDT_PATH);
								radxa_fdt_status_disabled(blob,DP0_IN_VP2_NODE_FDT_PATH);
								radxa_fdt_fixup_logo(blob,VP0_OUT_DP0,ROUTE_DP0);//logo
							} else if (overlay_info.screen_info[i].vop == VOP1) {
								radxa_fdt_status_disabled(blob,DP0_IN_VP0_NODE_FDT_PATH);
								radxa_fdt_status_okay(blob,DP0_IN_VP1_NODE_FDT_PATH);
								radxa_fdt_status_disabled(blob,DP0_IN_VP2_NODE_FDT_PATH);
								radxa_fdt_fixup_logo(blob,VP1_OUT_DP0,ROUTE_DP0);
							} else if (overlay_info.screen_info[i].vop == VOP2) {
								radxa_fdt_status_disabled(blob,DP0_IN_VP0_NODE_FDT_PATH);
								radxa_fdt_status_disabled(blob,DP0_IN_VP1_NODE_FDT_PATH);
								radxa_fdt_status_okay(blob,DP0_IN_VP2_NODE_FDT_PATH);
								radxa_fdt_fixup_logo(blob,VP2_OUT_DP0,ROUTE_DP0);
							}
						}
					}
				} else if(overlay_info.screen_info[i].type == RADXA_CONNECTOR_DP1) { //dp1
					node = fdt_path_offset(blob, DP1_NODE_FDT_PATH);
					if (node < 0) {
						printf("%s %s Path not found\n",__func__,DP1_NODE_FDT_PATH);
					} else {
						if(overlay_info.screen_info[i].enable == FALSE){
							radxa_fdt_status_disabled(blob,DP1_NODE_FDT_PATH);
							//radxa_fdt_status_disabled(blob,DP1_USBDP_PHY_NODE_FDT_PATH);//phy usb/dp share
							radxa_fdt_status_disabled(blob,DP1_IN_VP0_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,DP1_IN_VP1_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,DP1_IN_VP2_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,DP1_LOGO_ROUTE_FDT_PATH);
						} else {
							radxa_fdt_status_okay(blob,DP1_NODE_FDT_PATH);
							//radxa_fdt_status_okay(blob,DP1_USBDP_PHY_NODE_FDT_PATH);//phy usb/dp share
							radxa_fdt_status_okay(blob,DP1_LOGO_ROUTE_FDT_PATH);
							if (overlay_info.screen_info[i].vop == VOP0){
								radxa_fdt_status_okay(blob,DP1_IN_VP0_NODE_FDT_PATH);
								radxa_fdt_status_disabled(blob,DP1_IN_VP1_NODE_FDT_PATH);
								radxa_fdt_status_disabled(blob,DP1_IN_VP2_NODE_FDT_PATH);
								radxa_fdt_fixup_logo(blob,VP0_OUT_DP1,ROUTE_DP0);//logo
							} else if (overlay_info.screen_info[i].vop == VOP1) {
								radxa_fdt_status_disabled(blob,DP1_IN_VP0_NODE_FDT_PATH);
								radxa_fdt_status_okay(blob,DP1_IN_VP1_NODE_FDT_PATH);
								radxa_fdt_status_disabled(blob,DP1_IN_VP2_NODE_FDT_PATH);
								radxa_fdt_fixup_logo(blob,VP1_OUT_DP1,ROUTE_DP1);
							} else if (overlay_info.screen_info[i].vop == VOP2) {
								radxa_fdt_status_disabled(blob,DP1_IN_VP0_NODE_FDT_PATH);
								radxa_fdt_status_disabled(blob,DP1_IN_VP1_NODE_FDT_PATH);
								radxa_fdt_status_okay(blob,DP1_IN_VP2_NODE_FDT_PATH);
								radxa_fdt_fixup_logo(blob,VP2_OUT_DP1,ROUTE_DP1);
							}
						}
					}
				} else {
					printf("%s Invalid ID \n",__func__);
				}
				break;

			case RADXA_CONNECTOR_EDP0:
			case RADXA_CONNECTOR_EDP1:
				if(overlay_info.screen_info[i].type == RADXA_CONNECTOR_EDP0) { //edp0
					node = fdt_path_offset(blob, EDP0_NODE_FDT_PATH);
					if (node < 0) {
						printf("%s %s Path not found\n",__func__,EDP0_NODE_FDT_PATH);
					} else {
						if(overlay_info.screen_info[i].enable == FALSE){
							radxa_fdt_status_disabled(blob,EDP0_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,EDP0_PHY_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,EDP0_IN_VP0_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,EDP0_IN_VP1_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,EDP0_IN_VP2_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,EDP0_LOGO_ROUTE_FDT_PATH);
						} else {
							radxa_fdt_status_okay(blob,EDP0_NODE_FDT_PATH);
							radxa_fdt_status_okay(blob,EDP0_PHY_NODE_FDT_PATH);
							radxa_fdt_status_okay(blob,EDP0_LOGO_ROUTE_FDT_PATH);
							if (overlay_info.screen_info[i].vop == VOP0){
								radxa_fdt_status_okay(blob,EDP0_IN_VP0_NODE_FDT_PATH);
								radxa_fdt_status_disabled(blob,EDP0_IN_VP1_NODE_FDT_PATH);
								radxa_fdt_status_disabled(blob,EDP0_IN_VP2_NODE_FDT_PATH);
								radxa_fdt_fixup_logo(blob,VP0_OUT_EDP0,ROUTE_EDP0);
							} else if (overlay_info.screen_info[i].vop == VOP1) {
								radxa_fdt_status_disabled(blob,EDP0_IN_VP0_NODE_FDT_PATH);
								radxa_fdt_status_okay(blob,EDP0_IN_VP1_NODE_FDT_PATH);
								radxa_fdt_status_disabled(blob,EDP0_IN_VP2_NODE_FDT_PATH);
								radxa_fdt_fixup_logo(blob,VP1_OUT_EDP0,ROUTE_EDP0);
							} else if (overlay_info.screen_info[i].vop == VOP2) {
								radxa_fdt_status_disabled(blob,EDP0_IN_VP0_NODE_FDT_PATH);
								radxa_fdt_status_disabled(blob,EDP0_IN_VP1_NODE_FDT_PATH);
								radxa_fdt_status_okay(blob,EDP0_IN_VP2_NODE_FDT_PATH);
								radxa_fdt_fixup_logo(blob,VP2_OUT_EDP0,ROUTE_EDP0);
							}
						}
					}
				} else if(overlay_info.screen_info[i].type == RADXA_CONNECTOR_EDP1) { //edp1
					node = fdt_path_offset(blob, EDP1_NODE_FDT_PATH);
					if (node < 0) {
						printf("%s %s Path not found\n",__func__,EDP1_NODE_FDT_PATH);
					} else {
						if(overlay_info.screen_info[i].enable == FALSE){
							radxa_fdt_status_disabled(blob,EDP1_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,EDP1_PHY_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,EDP1_IN_VP0_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,EDP1_IN_VP1_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,EDP1_IN_VP2_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,EDP1_LOGO_ROUTE_FDT_PATH);
						} else {
							radxa_fdt_status_okay(blob,EDP1_NODE_FDT_PATH);
							radxa_fdt_status_okay(blob,EDP1_PHY_NODE_FDT_PATH);
							radxa_fdt_status_okay(blob,EDP1_LOGO_ROUTE_FDT_PATH);
							if (overlay_info.screen_info[i].vop == VOP0){
								radxa_fdt_status_okay(blob,EDP1_IN_VP0_NODE_FDT_PATH);
								radxa_fdt_status_disabled(blob,EDP1_IN_VP1_NODE_FDT_PATH);
								radxa_fdt_status_disabled(blob,EDP1_IN_VP2_NODE_FDT_PATH);
								radxa_fdt_fixup_logo(blob,VP0_OUT_EDP1,ROUTE_EDP1);
							} else if (overlay_info.screen_info[i].vop == VOP1) {
								radxa_fdt_status_disabled(blob,EDP1_IN_VP0_NODE_FDT_PATH);
								radxa_fdt_status_okay(blob,EDP1_IN_VP1_NODE_FDT_PATH);
								radxa_fdt_status_disabled(blob,EDP1_IN_VP2_NODE_FDT_PATH);
								radxa_fdt_fixup_logo(blob,VP1_OUT_EDP1,ROUTE_EDP1);
							} else if (overlay_info.screen_info[i].vop == VOP2) {
								radxa_fdt_status_disabled(blob,EDP1_IN_VP0_NODE_FDT_PATH);
								radxa_fdt_status_disabled(blob,EDP1_IN_VP1_NODE_FDT_PATH);
								radxa_fdt_status_okay(blob,EDP1_IN_VP2_NODE_FDT_PATH);
								radxa_fdt_fixup_logo(blob,VP2_OUT_EDP1,ROUTE_EDP1);
							}
						}
					}
				} else {
					printf("%s Invalid ID \n",__func__);
				}
				break;

			case RADXA_CONNECTOR_DSI0:
			case RADXA_CONNECTOR_DSI1:
				if(overlay_info.screen_info[i].type == RADXA_CONNECTOR_DSI0) { //dsi0
					node = fdt_path_offset(blob, DSI0_NODE_FDT_PATH);
					if (node < 0) {
						printf("%s %s Path not found\n",__func__,DSI0_NODE_FDT_PATH);
					} else {
						if(overlay_info.screen_info[i].enable == FALSE){
							radxa_fdt_status_disabled(blob,DSI0_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,DSI0_PHY_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,DSI0_IN_VP2_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,DSI0_IN_VP3_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,DSI0_LOGO_ROUTE_FDT_PATH);
						} else {
							radxa_fdt_status_okay(blob,DSI0_NODE_FDT_PATH);
							radxa_fdt_status_okay(blob,DSI0_PHY_NODE_FDT_PATH);
							radxa_fdt_status_okay(blob,DSI0_LOGO_ROUTE_FDT_PATH);
							if (overlay_info.screen_info[i].vop == VOP2){
								radxa_fdt_status_okay(blob,DSI0_IN_VP2_NODE_FDT_PATH);
								radxa_fdt_status_disabled(blob,DSI0_IN_VP3_NODE_FDT_PATH);
								radxa_fdt_fixup_logo(blob,VP2_OUT_DSI0,ROUTE_DSI0);
							} else if (overlay_info.screen_info[i].vop == VOP3) {
								radxa_fdt_status_disabled(blob,DSI0_IN_VP2_NODE_FDT_PATH);
								radxa_fdt_status_okay(blob,DSI0_IN_VP3_NODE_FDT_PATH);
								radxa_fdt_fixup_logo(blob,VP3_OUT_DSI0,ROUTE_DSI0);
							}
							radxa_fdt_select_panel(blob,overlay_info.screen_info[i]);
						}
					}
				} else if(overlay_info.screen_info[i].type == RADXA_CONNECTOR_DSI1) { //dsi1
					node = fdt_path_offset(blob, DSI1_NODE_FDT_PATH);
					if (node < 0) {
						printf("%s %s Path not found\n",__func__,DSI1_NODE_FDT_PATH);
					} else {
						if(overlay_info.screen_info[i].enable == FALSE){
							radxa_fdt_status_disabled(blob,DSI1_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,DSI1_PHY_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,DSI1_IN_VP2_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,DSI1_IN_VP3_NODE_FDT_PATH);
							radxa_fdt_status_disabled(blob,DSI1_LOGO_ROUTE_FDT_PATH);
						} else {
							radxa_fdt_status_okay(blob,DSI1_NODE_FDT_PATH);
							radxa_fdt_status_okay(blob,DSI1_PHY_NODE_FDT_PATH);
							radxa_fdt_status_okay(blob,DSI1_LOGO_ROUTE_FDT_PATH);
							if (overlay_info.screen_info[i].vop == VOP2){
								radxa_fdt_status_okay(blob,DSI1_IN_VP2_NODE_FDT_PATH);
								radxa_fdt_status_disabled(blob,DSI1_IN_VP3_NODE_FDT_PATH);
								radxa_fdt_fixup_logo(blob,VP2_OUT_DSI1,ROUTE_DSI1);
							} else if (overlay_info.screen_info[i].vop == VOP3) {
								radxa_fdt_status_disabled(blob,DSI1_IN_VP2_NODE_FDT_PATH);
								radxa_fdt_status_okay(blob,DSI1_IN_VP3_NODE_FDT_PATH);
								radxa_fdt_fixup_logo(blob,VP3_OUT_DSI1,ROUTE_DSI1);
							}
							radxa_fdt_select_panel(blob,overlay_info.screen_info[i]);
						}
					}
				} else {
					printf("%s Invalid ID \n",__func__);
				}
				break;

			case RADXA_CONNECTOR_LVDS:
				printf("%s lvds is not implemented \n",__func__);
				break;

			default:
				printf("%s Invalid connector   type \n",__func__);
				break;
		}
	}
	return ret;
}
int radxa_display_overlay(const void *blob)
{
	int ret;
	DBG("%s\n",__func__);

	struct blk_desc *dev_desc;
	dev_desc = rockchip_get_bootdev();
	if (!dev_desc) {
		printf("%s: dev_desc is NULL!\n", __func__);
		return -ENODEV;
	}

	printf("%s...%d:%d\n",__func__,dev_desc->if_type,dev_desc->devnum);

	ret = radxa_read_screen_info(blob);
	if(ret < 0)
		 printf("radxa_read_screen_info failed\n");
	else
		radxa_fdt_fixup_screen(blob);

	return 0;
}
