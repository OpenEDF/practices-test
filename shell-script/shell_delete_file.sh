########## START ##############
#/bin/bash
# deleted the all *.o file.
#1. deleted app floder
prefix="rom_lib/rom_lib_fpga/core0/"
del_file="
    app 
    driver/hw/*.o 
    driver/non_os/efuse/*.o
    driver/non_os/irq/*.o
    driver/non_os/soc/*.o
    driver/non_os/timer/*.o
    lib/boot_reason/src/*.o
    lib/cpu_usage/src/*.o
    lib/iot_debug/src/*.o
    lib/pkt/*.o
    lib/riscv/src/*.o
    lib/utils/src
    lib/iot_libc/src/ctype
    lib/iot_libc/src/search 
    lib/iot_libc/src/stdio
    lib/iot_libc/src/stdlib
    lib/iot_libc/src/string
    lib/iot_libc/src/*.o
    os/freertos_10_2_1/freertos
    os/os_shim/*.o
    os/freertos_10_2_1/os_shim/*.o
    os/freertos_10_2_1/portable
    phy/hawk_phy_v2/phy
    phy/hawk_phy_v2/phy_math
    phy/hawk_phy_v2/phy_std
    wifi/wifi_core/wifi_core/wdrv"

#2. for loop deleted
for file in ${del_file}
do
    del=$prefix$file
    rm -rf $del
    echo "[OK] Remove the $del"
done

#3.finish
echo "All done."

#4. check the file and floder
echo "Show the all file."
tree $prefix

########## END #################
