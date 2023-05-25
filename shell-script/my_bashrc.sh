# ~/.bashrc: executed by bash(1) for non-login shells.
# see /usr/share/doc/bash/examples/startup-files (in the package bash-doc)
# for examples

# If not running interactively, don't do anything
case $- in
    *i*) ;;
      *) return;;
esac

# don't put duplicate lines or lines starting with space in the history.
# See bash(1) for more options
HISTCONTROL=ignoreboth

# append to the history file, don't overwrite it
shopt -s histappend

# for setting history length see HISTSIZE and HISTFILESIZE in bash(1)
HISTSIZE=1000
HISTFILESIZE=2000

# check the window size after each command and, if necessary,
# update the values of LINES and COLUMNS.
shopt -s checkwinsize

# If set, the pattern "**" used in a pathname expansion context will
# match all files and zero or more directories and subdirectories.
#shopt -s globstar

# make less more friendly for non-text input files, see lesspipe(1)
[ -x /usr/bin/lesspipe ] && eval "$(SHELL=/bin/sh lesspipe)"

# set variable identifying the chroot you work in (used in the prompt below)
if [ -z "${debian_chroot:-}" ] && [ -r /etc/debian_chroot ]; then
    debian_chroot=$(cat /etc/debian_chroot)
fi

# set a fancy prompt (non-color, unless we know we "want" color)
case "$TERM" in
    xterm-color|*-256color) color_prompt=yes;;
esac

# uncomment for a colored prompt, if the terminal has the capability; turned
# off by default to not distract the user: the focus in a terminal window
# should be on the output of commands, not on the prompt
#force_color_prompt=yes

if [ -n "$force_color_prompt" ]; then
    if [ -x /usr/bin/tput ] && tput setaf 1 >&/dev/null; then
	# We have color support; assume it's compliant with Ecma-48
	# (ISO/IEC-6429). (Lack of such support is extremely rare, and such
	# a case would tend to support setf rather than setaf.)
	color_prompt=yes
    else
	color_prompt=
    fi
fi

if [ "$color_prompt" = yes ]; then
    PS1='${debian_chroot:+($debian_chroot)}\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '
else
    PS1='${debian_chroot:+($debian_chroot)}\u@\h:\w\$ '
fi
unset color_prompt force_color_prompt

# If this is an xterm set the title to user@host:dir
case "$TERM" in
xterm*|rxvt*)
    PS1="\[\e]0;${debian_chroot:+($debian_chroot)}\u@\h: \w\a\]$PS1"
    ;;
*)
    ;;
esac

# enable color support of ls and also add handy aliases
if [ -x /usr/bin/dircolors ]; then
    test -r ~/.dircolors && eval "$(dircolors -b ~/.dircolors)" || eval "$(dircolors -b)"
    alias ls='ls --color=auto'
    #alias dir='dir --color=auto'
    #alias vdir='vdir --color=auto'

    alias grep='grep --color=auto'
    alias fgrep='fgrep --color=auto'
    alias egrep='egrep --color=auto'
fi

# colored GCC warnings and errors
#export GCC_COLORS='error=01;31:warning=01;35:note=01;36:caret=01;32:locus=01:quote=01'

# some more ls aliases
alias ll='ls -alF'
alias la='ls -A'
alias l='ls -CF'

# Add an "alert" alias for long running commands.  Use like so:
#   sleep 10; alert
alias alert='notify-send --urgency=low -i "$([ $? = 0 ] && echo terminal || echo error)" "$(history|tail -n1|sed -e '\''s/^\s*[0-9]\+\s*//;s/[;&|]\s*alert$//'\'')"'

# Alias definitions.
# You may want to put all your additions into a separate file like
# ~/.bash_aliases, instead of adding them here directly.
# See /usr/share/doc/bash-doc/examples in the bash-doc package.

if [ -f ~/.bash_aliases ]; then
    . ~/.bash_aliases
fi

# enable programmable completion features (you don't need to enable
# this, if it's already enabled in /etc/bash.bashrc and /etc/profile
# sources /etc/bash.bashrc).
if ! shopt -oq posix; then
  if [ -f /usr/share/bash-completion/bash_completion ]; then
    . /usr/share/bash-completion/bash_completion
  elif [ -f /etc/bash_completion ]; then
    . /etc/bash_completion
  fi
fi

# set the esp-idf env
alias espenv='. $HOME/esp/esp-idf/export.sh'
alias python='python3'

# set the user command 
alias ..="cd .."
alias ..2="cd ../.."
alias ..3="cd ../../.."
alias ..4="cd ../../../.."
alias ..5="cd ../../../../.."

# risc-v toochain
PATH=/opt/riscv/bin:$PATH

# gio open view pdf document
alias openpdf="gio open"

# VSC + VERDI + SCL + SPYGLASS + DC + SYN + SYNPLIFY + IDENTIFY
export DVE_HOME=/home/macro/synopsys/vcs_2018/vcs/O-2018.09-SP2
export VCS_HOME=/home/macro/synopsys/vcs_2018/vcs/O-2018.09-SP2
export LD_LIBRARY_PATH=/home/macro/synopsys/verdi_2018/verdi/Verdi_O-2018.09-SP2/share/PLI/VCS/LINUX64
export VERDI_HOME=/home/macro/synopsys/verdi_2018/verdi/Verdi_O-2018.09-SP2
export SCL_HOME=/home/macro/synopsys/scl_2018/scl/2018.06
export SPYGLASS_HOME=/home/macro/synopsys/spyglass_2016/SpyGlass-L2016.06/SPYGLASS_HOME
export DC_HOME=/home/macro/synopsys/dc_2016/syn/L-2016.03-SP1/
export SYN_HOME=/home/macro/synopsys/syn_2018/syn/O-2018.06-SP1/
export DESIGNWARE_HOME=/home/macro/synopsys/vc_vip_2018/
export PT_HOME=/home/macro/synopsys/pt_2018/pts/O-2018.06-SP1/
export FM_HOME=/home/macro/synopsys/fm_2018/fm/O-2018.06-SP1/
export ICC_HOME=/home/macro/synopsys/icc_2018/icc/O-2018.06-SP1/
export SYNPLIFY_HOME=/home/macro/synopsys/synplify_pro_2018/fpga/N-2018.03-SP1/
export IDENTIFY_HOME=/home/macro/synopsys/synplify_pro_2018/identify/N-2018.03-SP1/
export LC_HOME=/home/macro/synopsys/lib_com_2018/lc/O-2018.06-SP1/

# Altera Quartus primer
export QUARTUS_ROOTDIR=/home/macro/intelFPGA/21.1/quartus
export LD_LIBRARY_PATH=$QUARTUS_ROOTDIT/linux64:$LD_LIBRARY_PATH
PATH=$PATH:$QUARTUS_ROOTDIT/bin
export QSYS_ROOTDIR="/home/macro/intelFPGA/21.1/quartus/sopc_builder/bin"

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

# SPYGLASS
PATH=$PATH:$SPYGLASS_HOME/bin
alias spy='spyglass'

#DC
PATH=$PATH:$DC_HOME/bin
alias dc='dc_shell'
alias dv='design_vision'

#SYN
#PATH=$PATH:$SYN_HOME/bin
#alias dc='dc_shell'
#alias dv='design_vision'

#PT
PATH=$PATH:$PT_HOME/bin
alias pt='primetime'

#FM
PATH=$PATH:$FM_HOME/bin
alias fm='formality'

#ICC2
PATH=$PATH:$ICC_HOME/bin
alias icc='icc_shell'

#VIP
PATH=$PATH:$DESIGNWARE_HOME/bin

#SYNPLIFY PRO
PATH=$PATH:$SYNPLIFY_HOME/bin
alias synplify='synplify_pro'

#IDENTIFY
PATH=$PATH:$IDENTIFY_HOME/bin
alias identify='identify_debugger'

#LC
PATH=$PATH:$LC_HOME/bin
alias lc='lc_shell'

# LICENCE
export LM_LICENSE_FILE=27000@macro-ubuntu
alias lmg_synopsys='lmgrd -c /home/macro/synopsys/scl_2018/scl/2018.06/admin/license/Synopsys.dat'

# Vivado
#source xilinx/Vivado/2019.2/settings64.sh
alias setvivado='source /home/macro/xilinx/Vivado/2019.2/settings64.sh'

#Mentor calibre
#export MGC_HOME=/home/macro/mentor/calibre_2020/aoj_cal_2020.3_16.11
#PATH=$PATH:$MGC_HOME/bin
#PATH=$PATH:$MGC_HOME/lib
#export MGLS_LICENSE_FILE=/home/macro/mentor/calibre_2020/license/license.dat

# UVM LIB
export UVM_1_2_HOME=/home/macro/synopsys/uvm_lib/uvm-1.2
export UVM_1800_2_2020_v_1_1_HOME=/home/macro/synopsys/uvm_lib/1800.2-2020-1.1
export UVM_HOME=/home/macro/synopsys/uvm_lib/uvm-1.2

# Colors
NOCOLOR='\033[0m'
RED='\033[0;31m'
GREEN='\033[0;32m'
ORANGE='\033[0;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
LIGHTGRAY='\033[0;37m'
DARKGRAY='\033[1;30m'
LIGHTRED='\033[1;31m'
LIGHTGREEN='\033[1;32m'
YELLOW='\033[1;33m'
LIGHTBLUE='\033[1;34m'
LIGHTPURPLE='\033[1;35m'
LIGHTCYAN='\033[1;36m'
WHITE='\033[1;37m'

# command 'rm -rf *' never to be executed!
shopt -s extdebug
checkcommand() {
    if [[ $BASH_COMMAND = 'rm -rf *' ]]; then
        echo "[ERROR] The command you have executed is equivalent to ending the life of the computer, we refuse to carry out your command !!!"
        return 1
    fi
        # You can check for other commands here, if you like.
    return 0
}
trap checkcommand DEBUG
set history-size 1000
# display timestamp
export HISTTIMEFORMAT='%F %T '
export HISTIGNORE="pwd:ls:ls -ltr:top:jobs:history"
