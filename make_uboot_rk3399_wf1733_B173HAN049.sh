echo "start patch"
git apply uboot-restore-boot.patch && \
cp drivers/video/drm/analogix_dp.c-B173HAN049 drivers/video/drm/analogix_dp.c && \
echo "end patch"
./make.sh rk3399 && \
echo "start checkout patch"
git checkout arch/arm/mach-rockchip/boot_mode.c drivers/video/drm/rockchip_panel.c && \
cp drivers/video/drm/analogix_dp.c-normal drivers/video/drm/analogix_dp.c
echo "end checkout patch"
