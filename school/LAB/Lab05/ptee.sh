#!/bin/sh

ShowUsage() {
    cat << EOF
Usage: `basename $0` base_cmd 'pipe_process1' 'pipe_process2' ...
Splits stdout to several different commands, making sure they terminate
sequentially and thus not mix their outputs. Basically a thin wrapper
around tee(1).
EOF
    exit $1
}

if [ $# -eq 0 ]; then
    ShowUsage 0
elif [ $# -lt 2 ]; then
    echo "Not enough commands."
    ShowUsage 1
fi

first=true
tmp="$(mktemp -u)"
cat > "$tmp"

for cmd in "$@"; do
    if $first; then 
        first=false 
    else
        printf "\n\n"
    fi
    printf "$cmd\n\n"

    (cat "$tmp" | $cmd)
done

rm "$tmp"

#CMD="$1"
#shift
#pipes=( )

#num=${#@}
#(( num = num - 1 ))

#for proc in {0..$num}; do
    #tmp="$(mktemp -u)"
    #mkfifo "$tmp"
    #pipes=( $pipes "$tmp" )
#done

#echo "Executing ${=CMD}"

#setopt multios
#${=CMD} >/dev/tty >${pipes[1]} >${pipes[2]} &
##${=CMD} | tee /dev/tty >${pipes[2]} &

#echo "Executing ${=1} < ${pipes[1]}"

#${=1} < ${pipes[1]} &

#echo "Executing ${=2} < ${pipes[2]}"

#${=2} < ${pipes[2]} &

#for pip in $pipes; do
    #rm -f "$pip"
#done
