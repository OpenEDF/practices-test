#/usr/bin/bash
# A completion function for the scons builtin

# hawk project name
project=('unit_test   unit_test_fpga    unit_test_os    unit_test_os_fpga
	   	  demo        demo_fpga basp    bsp_fpga        smoke_test 
	   	  smorom_lib  rom               rom_fpga	    sbl    
		  sbl_fpga    fw_updater        fw_updater_fpga wifi_dtest	
		  wifi_dtest_phy_v2	wifi_dtest_phy_v2_fpga
		  phy_v2_test		phy_v2_test_fpga
		  rom_lib 			rom_lib_test')

# auto completion function and programmable generated
# $1 is name of the command 
# s2 is the word being completed
# s3 is the word preceding the word being completed

_scons_completions ()
{
	local cur
	cur=${COMP_WORDS[3]}
	COMPREPLY=($(compgen -W "$project" $cur))
}

complete -F _scons_completions  scons

# add command:'. $(find name sconscompletion.sh)' to root .basrc
