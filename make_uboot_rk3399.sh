echo "start patch"
git apply uboot-restore-boot.patch && \
echo "end patch"
./make.sh rk3399 && \
echo "start checkout patch"
git checkout arch/arm/mach-rockchip/boot_mode.c drivers/video/drm/rockchip_panel.c
echo "end checkout patch"