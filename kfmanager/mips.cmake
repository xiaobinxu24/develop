# this one is important
SET(CMAKE_SYSTEM_NAME Linux)
option(MIPS "For mips version" ON)
option(MTK "For mtk version" ON)
option(GOOGLETEST "If need googletest" OFF)

#SET(cm_include "/home/v/jkdata/work/op-3-3-8/staging_dir/target-mipsel_r2_uClibc-0.9.33.2/usr/include")
#SET(cm_include "/home/v/jkdata/work/chaos_calmer/staging_dir/target-mipsel_mips32r2_uClibc-0.9.33.2/usr/include")
SET(cm_include "/home/v/jkdata/work/chaos_calmer/staging_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/usr/include")
SET(cm_link "/home/v/jkdata/work/chaos_calmer/staging_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/usr/lib" )

# specify the cross compiler
#SET(CMAKE_C_COMPILER   /opt/OpenWrt-Toolchain-ralink-for-mipsel_r2-gcc-4.6-linaro_uClibc-0.9.33.2/toolchain-mipsel_r2_gcc-4.6-linaro_uClibc-0.9.33.2/bin/mipsel-openwrt-linux-gcc)
#SET(CMAKE_CXX_COMPILER   /opt/OpenWrt-Toolchain-ralink-for-mipsel_r2-gcc-4.6-linaro_uClibc-0.9.33.2/toolchain-mipsel_r2_gcc-4.6-linaro_uClibc-0.9.33.2/bin/mipsel-openwrt-linux-g++)
#SET(CMAKE_C_COMPILER   /media/v/docs/work/op-3-3-8/staging_dir/toolchain-mipsel_r2_gcc-4.6-linaro_uClibc-0.9.33.2/bin/mipsel-openwrt-linux-gcc)
#SET(CMAKE_CXX_COMPILER   /media/v/docs/work/op-3-3-8/staging_dir/toolchain-mipsel_r2_gcc-4.6-linaro_uClibc-0.9.33.2/bin/mipsel-openwrt-linux-g++)
SET(CMAKE_C_COMPILER /home/v/jkdata/work/chaos_calmer/staging_dir/toolchain-mipsel_24kec+dsp_gcc-4.8-linaro_uClibc-0.9.33.2/bin/mipsel-openwrt-linux-uclibc-gcc)
SET(CMAKE_CXX_COMPILER /home/v/jkdata/work/chaos_calmer/staging_dir/toolchain-mipsel_24kec+dsp_gcc-4.8-linaro_uClibc-0.9.33.2/bin/mipsel-openwrt-linux-uclibc-g++)

# where is the target environment 
#SET(CMAKE_FIND_ROOT_PATH  /pathto/crosstool-linux-gcc-4.5.2-gclibc-2.9-oabi/arm-unknown-linux-gnu /pathto/crosstool-linux-gcc-4.5.2-gclibc-2.9-oabi/arm-unknown-linux-gnu/arm-unknown-linux-gnu)
#SET(CMAKE_FIND_ROOT_PATH   /opt/OpenWrt-Toolchain-ralink-for-mipsel_r2-gcc-4.6-linaro_uClibc-0.9.33.2/toolchain-mipsel_r2_gcc-4.6-linaro_uClibc-0.9.33.2)
#SET(CMAKE_FIND_ROOT_PATH /media/v/docs/work/proj/work/op-3-3-8/staging_dir/target-mipsel_r2_uClibc-0.9.33.2)

# search for programs in the build host directories
#SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
#SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
