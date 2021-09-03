@echo off

REM Path to the POF binary file
set pof_file="hawk_top.pof"
REM Index of the target device in the JTAG chain
set device_index=1

REM Set the window title
title JTAG erasing
set QUARTUS_ROOTDIR=D:\intelFPGA\18.0\qprogrammer

REM Silently check if the "quartus_pgm" program is reachable
%QUARTUS_ROOTDIR%\bin64\quartus_pgm --version
if %ErrorLevel% neq 0 goto error_quartus_pgm
%QUARTUS_ROOTDIR%\bin64\quartus_pgm --auto

echo.
echo Erasing and blank-checking the device...
echo.
REM Erase (R) + Blank-check (B) device at index %device_index%
%QUARTUS_ROOTDIR%\bin64\quartus_pgm --mode=JTAG --operation=R;%pof_file%@%device_index%
%QUARTUS_ROOTDIR%\bin64\quartus_pgm --mode=JTAG --operation=B;%pof_file%@%device_index%
echo.
if %ErrorLevel% neq 0 goto error

:pass
echo Operation is terminated.
echo Please reboot the board.
echo.
pause
goto eof

:error
echo Erase operation has FAILED.
echo.
pause
goto eof

:error_quartus_pgm
echo ERROR: PROGRAMMER IS NOT AVAILABLE!
echo The file "quartus_pgm.exe" has not been found on your computer.
echo As a consequence, none of the programming/erasing operations are available.
echo This script will exit.
echo.
pause
goto eof

:eof
