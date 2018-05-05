#!/bin/sh

# Check that we got enough arguments at that the first one is actually executable.
[ $# -lt 2 ] && exit 1
[ "$(command -v $1)" ] || exit 2

#####################################################
# Below is rather specific for checkbashisms, to the point where this whole script is dubious.

# Ditch gvfs stuff - non portable
#[ "$(echo "$2" | grep 'gvfs')" ] && exit 3

# Portable command to collect output.
out="$($1 "$2" 2>&1)"

# Initial if is portable, lines following it are not
if [ "$out" ] \
&& [ -z "$(echo "$out" | grep -e 'appears to be a shell wrapper' -e 'command="`')" ]; then
    out="$(echo "$out" | perl -pe 's/^(possible.*)/\033[1;31m$1\033[0m/g')"

    printf  -- '\n\n\n\033[1;33m-----   %s   -----\033[0m\n\n' "${2}"

    #if [ "$(echo "$out" | grep -e '==' -e 'bash arrays' -e '')" ]; then 
    if ! [ "$(echo "$out" | grep 'type')" ]; then 
        perl -pi -e 's,^#\!/bin/sh,#!/bin/bash,' "$2"
        printf "\033[1;32mFixing ${2} because of obvious violations.\033[0m\n"
    else
        #printf "   NOT FIXED\n\n"
        echo "$out"
    fi
fi
