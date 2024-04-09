echo "start patch"
git apply uboot-restore-boot.patch && \
git apply support_rk817_bigBattery.patch && \
echo "end patch"
./make.sh rk3568 && \
echo "start checkout patch"
git checkout arch/arm/mach-rockchip/boot_mode.c drivers/video/drm/rockchip_panel.c
git checkout configs/rk3568_defconfig drivers/power/charge_animation.c drivers/power/fuel_gauge/fg_rk817.c drivers/power/fuel_gauge/fuel_gauge_uclass.c include/power/fuel_gauge.h include/power/rk8xx_pmic.h
echo "end checkout patch"