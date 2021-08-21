#!/bin/bash
# 1. set the function for print

# 
# function bref: print help infomation
# return: None
#
function print_help()
{
    echo " [cmd]                 [descript]"
    echo "  help        print all command and description."
    echo "  connect     test the gerrit connect."
    echo "  project     list the all project in gerrit."
    echo "  apropos     list the all aproops capabilities in gerrit."
    echo "  groups      list the all groups in gerrit."
    echo "  owngroups   list the all groups in gerrit for own."
    echo "  version     print the gerrit version."
    echo "  submit      list the sunmit information."
    echo "  reviewers   set reviewers."
}

# 2. get the current change id and set env
USER='yang.zhixiong'
SERVER='192.168.100.51'
CUR_ChangeId=$(git log -1 | grep Change-Id | sed s/'    Change-Id: '//)
CLIARGUM=$1
REVIEWERS='-a huang.shuoshuo@wuqi-tech.com -a chen.stephen@wuqi-tech.com -a yuan.sam@wuqi-tech.com
           -a mao.zhiqiang@wuqi-tech.com -a ge.yiqiao@wuqi-tech.com -a qu.chengkang@wuqi-tech.com'

# 3. multiway branch
case "$CLIARGUM" in
    "help")
        print_help
    ;;
    "connect")    
        ssh -p 29419 $USER@$SERVER
    ;;
    "project")
        ssh -p 29419 $USER@$SERVER gerrit ls-projects
    ;;
    "apropos")
        ssh -p 29419 $USER@$SERVER gerrit apropos capabilities
    ;;
    "groups")
        ssh -p 29419 $USER@$SERVER gerrit ls-groups
    ;;
    "owngroups")
        ssh -p 29419 $USER@$SERVER gerrit ls-groups --owned
    ;;
    "version")
        ssh -p 29419 $USER@$SERVER gerrit version
    ;;
    "submit")
        ssh -p 29419 $USER@$SERVER gerrit test-submit rule $CUR_ChangeId --no-filters
    ;;
    "reviewers")
        ssh -p 29419 $USER@$SERVER gerrit set-reviewers $REVIEWERS $CUR_ChangeId
    ;;
    *)
        echo "Input Argument Invalid: $CLIARGUM".
        echo "Please The Check And Reply."
        print_help
    ;;
esac
################################### end ##################################################
