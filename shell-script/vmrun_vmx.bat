:: power on the ubuntu 
set UBUNTU="D:\Ubuntu2004\Ubuntu2004.vmx"
:: vmrun start %UBUNTU% nogui

:: power off the ubuntu 
::vmrun stop %UBUNTU% nogui

:: list the visual machine
::vmrun list

:: start visual machine use vmware
vmware -x %UBUNTU%

:: start mobaxterm
:: MobaXterm &
