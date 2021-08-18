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
