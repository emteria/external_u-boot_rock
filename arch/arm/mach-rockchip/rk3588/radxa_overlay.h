#include <common.h>
#include <malloc.h>
#include <asm/arch/vendor.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/libfdt.h>
#include <fdt_support.h>
#include <boot_rkimg.h>

DECLARE_GLOBAL_DATA_PTR;

#define TRUE	1
#define FALSE	0

//crtc
#define VOP0	0
#define VOP1	1
#define VOP2	2
#define VOP3	3

//connector<-->crtc
#define VP0_OUT_DP0         "/vop@fdd90000/ports/port@0/endpoint@0"  //dp0_in_vp0
#define VP0_OUT_EDP0        "/vop@fdd90000/ports/port@0/endpoint@1"  //edp0_in_vp0
#define VP0_OUT_HDMI0       "/vop@fdd90000/ports/port@0/endpoint@2"  //hdmi0_in_vp0
#define VP0_OUT_DP1         "/vop@fdd90000/ports/port@0/endpoint@3"  //dp1_in_vp0
#define VP0_OUT_EDP1        "/vop@fdd90000/ports/port@0/endpoint@4"  //edp1_in_vp0
#define VP0_OUT_HDMI1       "/vop@fdd90000/ports/port@0/endpoint@5"  //hdmi1_in_vp0

#define VP1_OUT_DP0         "/vop@fdd90000/ports/port@1/endpoint@0"  //dp0_in_vp1
#define VP1_OUT_EDP0        "/vop@fdd90000/ports/port@1/endpoint@1"  //edp0_in_vp1
#define VP1_OUT_HDMI0       "/vop@fdd90000/ports/port@1/endpoint@2"  //hdmi0_in_vp1
#define VP1_OUT_DP1         "/vop@fdd90000/ports/port@1/endpoint@3"  //dp1_in_vp1
#define VP1_OUT_EDP1        "/vop@fdd90000/ports/port@1/endpoint@4"  //edp1_in_vp1
#define VP1_OUT_HDMI1       "/vop@fdd90000/ports/port@1/endpoint@5"  //hdmi1_in_vp1

#define VP2_OUT_DP0         "/vop@fdd90000/ports/port@2/endpoint@0"  //dp0_in_vp2
#define VP2_OUT_EDP0        "/vop@fdd90000/ports/port@2/endpoint@1"  //edp0_in_vp2
#define VP2_OUT_HDMI0       "/vop@fdd90000/ports/port@2/endpoint@2"  //hdmi0_in_vp2
#define VP2_OUT_DSI0        "/vop@fdd90000/ports/port@2/endpoint@3"  //dsi0_in_vp2
#define VP2_OUT_DSI1        "/vop@fdd90000/ports/port@2/endpoint@4"  //dsi1_in_vp2
#define VP2_OUT_DP1         "/vop@fdd90000/ports/port@2/endpoint@5"  //dp1_in_vp2
#define VP2_OUT_EDP1        "/vop@fdd90000/ports/port@2/endpoint@6"  //edp1_in_vp2
#define VP2_OUT_HDMI1       "/vop@fdd90000/ports/port@2/endpoint@7"  //hdmi1_in_vp2

#define VP3_OUT_DSI0        "/vop@fdd90000/ports/port@3/endpoint@0"  //dsi0_in_vp3
#define VP3_OUT_DSI1        "/vop@fdd90000/ports/port@3/endpoint@1"  //dsi1_in_vp3
#define VP3_OUT_RGB         "/vop@fdd90000/ports/port@3/endpoint@2"  //rgb_in_vp3

//route logo
#define ROUTE_DP0           "/display-subsystem/route/route-dp0"
#define ROUTE_DP1           "/display-subsystem/route/route-dp1"
#define ROUTE_HDMI0         "/display-subsystem/route/route-hdmi0"
#define ROUTE_HDMI1         "/display-subsystem/route/route-hdmi1"
#define ROUTE_EDP0          "/display-subsystem/route/route-edp0"
#define ROUTE_EDP1          "/display-subsystem/route/route-edp1"
#define ROUTE_DSI0          "/display-subsystem/route/route-dsi0"
#define ROUTE_DSI1          "/display-subsystem/route/route-dsi1"

//connector   type
#define RADXA_CONNECTOR_HDMI0    0
#define RADXA_CONNECTOR_HDMI1    1
#define RADXA_CONNECTOR_DP0      2
#define RADXA_CONNECTOR_DP1      3
#define RADXA_CONNECTOR_EDP0     4
#define RADXA_CONNECTOR_EDP1     5
#define RADXA_CONNECTOR_DSI0     6
#define RADXA_CONNECTOR_DSI1     7
#define RADXA_CONNECTOR_LVDS     8
#define RADXA_CONNECTOR_RGB      9

//connector sub id
#define RADXA_CONNECTOR_SB0    0
#define RADXA_CONNECTOR_SB1    1
#define RADXA_CONNECTOR_SB2    2
#define RADXA_CONNECTOR_SB3    3
#define RADXA_CONNECTOR_SB4    4
#define RADXA_CONNECTOR_SB5    5

//hdmi path
#define HDMI0                           0
#define HDMI0_NODE_FDT_PATH             "/hdmi@fde80000"
#define HDMI0_PHY_NODE_FDT_PATH         "/hdmiphy@fed60000"   //phy hdmi/edp share
#define HDMI0_IN_VP0_NODE_FDT_PATH      "/hdmi@fde80000/ports/port@0/endpoint@0"
#define HDMI0_IN_VP1_NODE_FDT_PATH      "/hdmi@fde80000/ports/port@0/endpoint@1"
#define HDMI0_IN_VP2_NODE_FDT_PATH      "/hdmi@fde80000/ports/port@0/endpoint@2"
#define HDMI0_LOGO_ROUTE_FDT_PATH       "/display-subsystem/route/route-hdmi0"

#define HDMI1                           1
#define HDMI1_NODE_FDT_PATH             "/hdmi@fdea0000"
#define HDMI1_PHY_NODE_FDT_PATH         "/hdmiphy@fed70000"
#define HDMI1_IN_VP0_NODE_FDT_PATH      "/hdmi@fdea0000/ports/port@0/endpoint@0"
#define HDMI1_IN_VP1_NODE_FDT_PATH      "/hdmi@fdea0000/ports/port@0/endpoint@1"
#define HDMI1_IN_VP2_NODE_FDT_PATH      "/hdmi@fdea0000/ports/port@0/endpoint@2"
#define HDMI1_LOGO_ROUTE_FDT_PATH       "/display-subsystem/route/route-hdmi1"

//dp path
#define DP0                             0
#define DP0_NODE_FDT_PATH               "/dp@fde50000"
#define DP0_USBDP_PHY_NODE_FDT_PATH     "/phy@fed80000"  //phy usb/dp share
#define DP0_USBDP_PHY_DP_NODE_FDT_PATH  "/phy@fed80000/dp-port"
#define DP0_IN_VP0_NODE_FDT_PATH        "/dp@fde50000/ports/port@0/endpoint@0"
#define DP0_IN_VP1_NODE_FDT_PATH        "/dp@fde50000/ports/port@0/endpoint@1"
#define DP0_IN_VP2_NODE_FDT_PATH        "/dp@fde50000/ports/port@0/endpoint@2"
#define DP0_LOGO_ROUTE_FDT_PATH         "/display-subsystem/route/route-dp0"

#define DP1                             1
#define DP1_NODE_FDT_PATH               "/dp@fde60000"
#define DP1_USBDP_PHY_NODE_FDT_PATH     "/phy@fed90000"  //phy usb/dp share
#define DP1_USBDP_PHY_DP_NODE_FDT_PATH  "/phy@fed90000/dp-port"
#define DP1_IN_VP0_NODE_FDT_PATH        "/dp@fde60000/ports/port@0/endpoint@0"
#define DP1_IN_VP1_NODE_FDT_PATH        "/dp@fde60000/ports/port@0/endpoint@1"
#define DP1_IN_VP2_NODE_FDT_PATH        "/dp@fde60000/ports/port@0/endpoint@2"
#define DP1_LOGO_ROUTE_FDT_PATH         "/display-subsystem/route/route-dp1"

//edp path
#define EDP0                            0
#define EDP0_NODE_FDT_PATH              "/edp@fdec0000"
#define EDP0_PHY_NODE_FDT_PATH          "/phy@fed60000"
#define EDP0_IN_VP0_NODE_FDT_PATH       "/edp@fdec0000/ports/port@0/endpoint@0"
#define EDP0_IN_VP1_NODE_FDT_PATH       "/edp@fdec0000/ports/port@0/endpoint@1"
#define EDP0_IN_VP2_NODE_FDT_PATH       "/edp@fdec0000/ports/port@0/endpoint@2"
#define EDP0_LOGO_ROUTE_FDT_PATH        "/display-subsystem/route/route-edp0"

#define EDP1                            1
#define EDP1_NODE_FDT_PATH              "/edp@fded0000"
#define EDP1_PHY_NODE_FDT_PATH          "/phy@fed70000"
#define EDP1_IN_VP0_NODE_FDT_PATH       "/edp@fded0000/ports/port@0/endpoint@0"
#define EDP1_IN_VP1_NODE_FDT_PATH       "/edp@fded0000/ports/port@0/endpoint@1"
#define EDP1_IN_VP2_NODE_FDT_PATH       "/edp@fded0000/ports/port@0/endpoint@2"
#define EDP1_LOGO_ROUTE_FDT_PATH        "/display-subsystem/route/route-edp1"

//dsi path
#define DSI0                            0
#define DSI0_NODE_FDT_PATH              "/dsi@fde20000"
#define DSI0_PHY_NODE_FDT_PATH          "/phy@feda0000"
#define DSI0_PANEL_NODE_FDT_PATH        "/dsi@fde20000/panel@0"
#define DSI0_IN_VP2_NODE_FDT_PATH       "/dsi@fde20000/ports/port@0/endpoint@0"
#define DSI0_IN_VP3_NODE_FDT_PATH       "/dsi@fde20000/ports/port@0/endpoint@1"
#define DSI0_LOGO_ROUTE_FDT_PATH        "/display-subsystem/route/route-dsi0"

#define DSI1                            1
#define DSI1_NODE_FDT_PATH              "/dsi@fde30000"
#define DSI1_PHY_NODE_FDT_PATH          "/phy@fedb0000"
#define DSI1_PANEL_NODE_FDT_PATH        "/dsi@fde30000/panel@0"
#define DSI1_IN_VP2_NODE_FDT_PATH       "/dsi@fde30000/ports/port@0/endpoint@0"
#define DSI1_IN_VP3_NODE_FDT_PATH       "/dsi@fde30000/ports/port@0/endpoint@1"
#define DSI1_LOGO_ROUTE_FDT_PATH        "/display-subsystem/route/route-dsi1"

#define DSI_CLK_RATE                    "rockchip,lane-rate"
#define DSI_CLK_200                     200
#define DSI_CLK_480                     480
#define DSI_CLK_1000                    1000

//panel
#define DSI_PANEL0                      "radxa,display-8hd"
#define DSI_PANEL1                      "radxa,display-10hd"
#define DSI_PANEL2                      "radxa,display-10fhd"
#define DSI_PANEL3                      "raspits,tc358762-7inch"
#define DSI_PANEL4                      "raspits,tc358762-5inch"

/*
 * When the SD card is started, this node will affect the kernel vendor driver's use of the emmc device,
 * causing the SD card overlay data to be unable to be saved.
*/
#define EMMC_PATH		"/mmc@fe2e0000"

#define SCREEN_TYPE         10

//display item id
#define VENDOR_RADXA_DISPLAY_ID         32

struct screen_info {
    u8 type; /*connector*/
    u8 id;   /*panel*/
    u8 vop;   /*vop0~3*/
    u8 enable;
};

struct overlay_info {
    u32 screen_num;
    struct screen_info screen_info[SCREEN_TYPE];
};

int radxa_display_overlay(const void *blob);

