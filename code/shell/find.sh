#!/bin/sh

#str_tool="path"
#str_usage="[-s|--suppress] [-r|--reverse] [-d|--dirname] [-b|--basename] [-m|--magic] [-p|--path <path>] <str> [<str> ...]"
#gen_tmpfile=yes
#arg_spec="1+"
#opt_spec="s.r.d.b.m.p:"
#opt_alias="s:suppress,r:reverse,d:dirname,b:basename,m:magic,p:path"
#opt_r=no
#opt_d=no
#opt_b=no
#opt_m=no

if [ ".$ZSH_VERSION" != . ] && (emulate sh) >/dev/null 2>&1; then
    #   reconfigure zsh(1)
    emulate sh
elif [ ".$BASH_VERSION" != . ] && (set -o posix) >/dev/null 2>&1; then
    #   reconfigure bash(1)
    set -o posix
fi

namelist="$*"
opt_p="$PATH"
opt_s='no'
paths="`echo "$opt_p" | sed -e 's/^:/.:/' -e 's/::/:.:/g' -e 's/:$/:./' -e 's/:/ /g'`"

case "$1" in
    -s)
        opt_s='yes'
        shift
        ;;
esac


#   STANDARD SITUATION
#   iterate over names
for name in $namelist; do
    #   iterate over paths
    for path in $paths; do
        path="`echo "$path" | sed -e 's;/*$;;'`"
        if [ -x "$path/$name" ] && [ ! -d "$path/$name" ]; then
            if [ ".$opt_s" != .yes ]; then
                echo "$path/$name"
            fi
            exit 0
        fi
    done
done

exit 1
