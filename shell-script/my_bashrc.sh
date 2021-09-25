#scons completion
#. $(find -name sconscompletion.sh)
. /usr/share/bash-completion/scons_user

# spike and pk
#alias pk='/opt/riscv/riscv64-unknown-elf/bin/pk'
export PATH=$PATH:/opt/riscv/spike/bin

alias cd1..='cd ..'
alias cd2..='cd ../..'
alias cd3..='cd ../../..'
alias cd4..='cd ../../../..'
alias cd5..='cd ../../../../..'
alias cd6..='cd ../../../../../..'
alias cd7..='cd ../../../../../../'

# git psuh hawk
alias push_hawk='git push origin HEAD:refs/for/hawk_v3_dev'
alias clone_hawk='echo ssh://yang.zhixiong@192.168.100.51:29419/hawk_open_core.git'
HAWK_REP='ssh://yang.zhixiong@192.168.100.51:29419/hawk_open_core.git'

# system-c support
SYSTEMC_HOME=/usr/local/systemc233
SYSTEMC_INCLUDE=$SYSTEMC_HOME/include
SYSTEMC_LIBDIR=$SYSTEMC_HOME/lib-linux64

# query the weather
curl https://wttr.in/shanghai-pudong

# gio open view pdf document
alias openpdf="gio open"

# VSC + VERDI + SCL
export DVE_HOME=/home/macro/synopsys/vcs_2018/vcs/O-2018.09-SP2
export VCS_HOME=/home/macro/synopsys/vcs_2018/vcs/O-2018.09-SP2
export LD_LIBRARY_PATH=/home/macro/synopsys/verdi_2018/verdi/Verdi_O-2018.09-SP2/share/PLI/VCS/LINUX64
export VERDI_HOME=/home/macro/synopsys/verdi_2018/verdi/Verdi_O-2018.09-SP2
export SCL_HOME=/home/macro/synopsys/scl_2018/scl/2018.06

# DVE
PATH=$PATH:$VCS_HOME/gui/dve/bin
alias dve='dve -full64'

# VCS
PATH=$PATH:$VCS_HOME/bin
alias vcs='vcs'

# VERDI
PATH=$PATH:$VERDI_HOME/bin
alias verdi="verdi"

# SCL
PATH=$PATH:$SCL_HOME/linux64/bin
export VCS_ARCH_OVERRIDE=linux

# LICENCE
export LM_LICENSE_FILE=27000@macro-ubuntu
alias lmg_synopsys='lmgrd -c /home/macro/synopsys/scl_2018/scl/2018.06/admin/license/Synopsys.dat'
