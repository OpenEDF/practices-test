########## START ##############
#!/bin/sh
#
# Setup a work space called `debug-riscv` with two windows
# first window has 4 panes. 
#
session="riscv-debug"
openocd_cfg_file='../work/openocd-cfg/ftdi_openocd.cfg'
root_passwd='1'
gdbinit_file='../work/hawk_rom_lib_push/hawk_prebuild/tools/debug_scripts/gdbinit'
# set up tmux
tmux start-server

# create a new tmux session, starting htop from a saved session in the new window
#          |++++++++++++++++++++++++++++++++++++++++++++|
#          |                     |                      |
#          |                     |                      |
#          |         0           |         2            |
#          |                     |                      |
#          |++++++++++++++++++++++++++++++++++++++++++++|
#          |                     |                      |
#          |         1           |         3            |
#          |                     |                      |
#          |                     |                      |
#          |++++++++++++++++++++++++++++++++++++++++++++|
#

# 0. new the first pane debug
tmux new-session -d -s $session -n "debug"

# 1. spilt pane 0 virtical left 0 and right 1. (50% + 50%)
tmux selectp -t 0
tmux splitw -h -p 50

# 2. spilt pane 0 horizontal up 0 and down 1. (25% + 75%)
tmux selectp -t 0
tmux splitw -v -p 75

# 3. spilt pane 2 horizontal up 2 and down 3. (50% + 50%)
tmux selectp -t 2
tmux splitw -v -p 50

# 4. select pane 0 and run shell command 'htop'.
tmux selectp -t 0
tmux send-keys "htop" C-m

# 5. select pane 2 and run shell command 'sudo openocd -f cfgfile' and input the passwd.
tmux selectp -t 2
tmux send-keys "sudo openocd -f $openocd_cfg_file" C-m
tmux send-keys "$root_passwd" C-m

# 6. select pane 3 and run shell command 'sudo minicom' and input the passwd.
tmux selectp -t 3
tmux send-keys "sudo minicom" C-m
tmux send-keys "$root_passwd" C-m

# 7. return the pane 1 and run debug
tmux selectp -t 1
tmux send-keys "riscv64-unknown-elf-gdb -x $gdbinit_file" C-m

# 8. don't close the session when finsh the setup 
tmux attach-session -t $session

########## END #################
