:: power on the ubuntu 
set UBUNTU="D:\Ubuntu2004\Ubuntu2004.vmx"
::start vmrun start %UBUNTU% nogui

:: power off the ubuntu 
::vmrun stop %UBUNTU% nogui

:: list the visual machine
::vmrun list

:: start visual machine use vmware
start vmware -x %UBUNTU%

:: start mobaxterm
start MobaXterm
