#!/bin/sh

DEBUG=false

ShowUsage() {
    echo "Usage: $(basename "$0")  [local dir] <system dir>"
    cat << 'EOF'
Will remove fonts from local dir found in system dir. Specifying the system
dir is optional; '/usr/share/fonts' is the default.
EOF
    exit 1
}

if [ $# -eq 0 ] || [ "$1" = '-h' ] || [ "$1" = '--help' ]; then
    ShowUsage
elif [ $# -eq 1 ]; then
    locDir="$(realpath "$1")"
    sysDir=/usr/share/fonts
elif [ $# -eq 2 ]; then
    locDir="$(realpath "$1")"
    sysDir="$(realpath "$2")"
else
    echo 'Too many paramaters.'
    ShowUsage
fi

for file1 in "${locDir}"/*; do
    file1_base="$(basename "$file1")"
    if [ -d "${file1}" ]; then
        echo "File 1 ->${file1} is a dir!"
        for file2 in "${file1}"/*; do
            file2_base="$(basename "${file2}")"
            [ "${file2_base}" = 'fonts.scale' ] || [ "${file2_base}" = 'fonts.scale' ] && continue
            if [ -f "${sysDir}/${file1_base}/${file2_base}" ]; then
                printf "%-80s\t%s\n" "${file1}/${file2_base}" "is a dublicate, deleting"
                ! ${DEBUG} && rm -f "${file2}"
            fi
        done

    else
        [ "${file1_base}" = 'fonts.scale' ] || [ "${file1_base}" = 'fonts.scale' ] && continue
        if [ -f "${sysDir}/${file1_base}" ]; then
            printf "%-80s\t%s\n" "${file1}" "is a dublicate, deleting"
            ! ${DEBUG} && rm -f "${file1}"

        else
            for sysFile1 in "${sysDir}"/*; do
                if [ -d "$sysFile1" ]; then
                    if [ -f "${sysFile1}/${file1_base}" ]; then
                        printf "%-80s\t%s\n" "${file1}" "is a dublicate, deleting"
                        ! ${DEBUG} && rm -f "${file1}"
                    fi
                fi
            done
        fi
    fi
done
