#!/bin/jsh

[ "$ZSH_VERSION" ] && set -o shwordsplit >/dev/null 2>&1

# Only shows long usage info if either called with no paramaters or with 0.
# Exits after showing info with either the number or with 0.
ShowUsage() {
    THIS=`basename "$0"`
    [ "$1" ] && [ "$1" -ne 0 ] && out=2 || out=1
    echo "Usage: ${THIS} [options] archive(s)..." >&$out
    [ "$out" -eq 2 ] && exit "$1"
    echo; cat <<'EOF'
Extract an archive safely to a unique directory, ensuring no irritating
single sub-directories or lots of loose files are created. See the manual for
more detailed information. Long options are only available if getopt(1) is GNU
getopt. Otherwise the shell builtin getopts is used, which is more primative.

--- OPTIONS ---
-h --help      Show this usage information.
-V --version   Show version.
-v --verbose   Verbose mode. Display progress information if possible.
-o [dir]  --top [DIR]
               Explicitly specify output directory. If it already exists,
                 time_t will be appended to it. When used with multiple
                 archives it will function as a top directory with each archive
                 extracted to sub-directories, unless -c is supplied, whereupon
                 all archives are combined into it.
-c --combine   Combine multiple archives. When -o is not supplied, a directory
                 name is generated from the name of the first supplied archive.

-- TAR --
-b             Use bsdtar over 'tar' if it exists, otherwise fall back to tar.
-g             Use gtar if it exists, otherwise fall back to tar.
-t             Use 'tar' by default.
--tar ARG      Explicity specify the location/name of 'tar'.

-- EXTRACTION --
-7 --use7zip   Use 7zip for extractions if it is installed. Tar archives are
                 subsequently piped to tar because 7zip cannot properly handle
                 UN*X file permissions.
-T --usetar    Simply rely on 'tar -xf' to handle the extraction. Will fail if
                 tar fails to identify the archive.
-A             If the archive contains only one file that is a directory, use
                 it as the top dir even if it has a name like 'usr'.
-f --force     Force: try to extract an unknown archive by trial and error.
EOF
    exit 0
}


# =================================================================================================
# Subroutines


cmd_exists() {
    if [ -z "$ExistChecker" ]; then
        FindExistChecker
    fi
    case "$ExistChecker" in
        'command')
            command -v "$1" >/dev/null 2>&1
            return $?
            ;;
        'type')
            type "$1" >/dev/null 2>&1
            return $?
            ;;
        'shell')
            $ExistCheckerSh -c "command -v $1 >/dev/null 2>&1" >/dev/null 2>&1
            return $?
            ;;
        'csh')
            csh -c "which $1 >& /dev/null" >/dev/null 2>&1
            return $?
            ;;
        'which')
            which "$1" >/dev/null 2>&1
            return $?
            ;;
    esac

    return 255
}


# Yes, all of the redirections are actually necessary. Most shells don't need
# them, but the most ancient and terrible of shells will insist on echoing
# errors unless you put the command in a subshell and redirect that too.
FindExistChecker() {
    if (command -v ls >/dev/null 2>&1) >/dev/null 2>&1; then
        ExistChecker='command'
    elif (type ls >/dev/null 2>&1) >/dev/null 2>&1; then
        ExistChecker='type'
    else
        for shell in 'sh' 'bash' 'dash' 'ksh' 'mksh' 'pdksh' 'ash' 'busybox ash' 'zsh'; do
            if ($shell -c 'command -v ls >/dev/null 2>&1' >/dev/null 2>&1) >/dev/null 2>&1; then
                ExistChecker='shell'
                ExistCheckerSh="$shell"
                break
            fi
        done
    fi
    if [ -z "$ExistChecker" ]; then
        if (csh -c 'which ls >& /dev/null' >/dev/null 2>&1) >/dev/null 2>&1; then
            ExistChecker='csh'
        elif (which ls >/dev/null 2>&1) >/dev/null 2>&1; then
            ExistChecker='which'
        fi
    fi

    if [ -z "$ExistChecker" ]; then
        cat <<__EOF__
Your shell does not support "command -v", you do not have 'which' installed,
and you have no other shells installed (in a sane location) that support any
equivalent. Amazing. I could try to revert to something like testing whether
'cmd --version' succeeds, but instead, out of spite, this is a fatal error.
__EOF__
        exit 255
    fi
    $debug && echo "Using ${ExistChecker}" >&2
    $debug && [ -n "$ExistCheckerSh" ] && echo "Shell = ${ExistCheckerSh}" >&2
}


# =================================================================================================


# NOTE: 'is_tar' is also true for cpio files - tar can deal with both.
# EXPORTS: bpath fname bname ext bext is_tar
get_ext() {
    bpath=`realpath \`dirname "${Archive}"\``
    ext=`echo "${fname}" | grep -o '\.tar\..*'`
    [ "${ext}" ] || ext=`echo "${fname}" | grep -o '\.cpio\..*'`
    if [ "${ext}" ]; then
        ext=`echo "${ext}" | sed 's/^\.\(.*\)/\1/'`
        bext=`echo "${ext}" | sed 's/.*\.\(.*\)$/\1/'`
        bname=`basename "$fname" ".$ext"`
        is_tar=true
    else
        ext=`echo "${fname}" | sed 's/.*\.\(.*\)$/\1/'`
        bname=`basename "${fname}" ".${ext}"`
        check_short_tar "$ext"  # assigns is_tar
    fi
}


# Simple check whether the extension is a stupid DOS style short form.
# EXPORTS: is_tar
check_short_tar() {
    case "$1" in
        tgz)
            bext='gz'
            is_tar=true
            ;;
        tbz|tb2|tbz2)
            bext='bz2'
            is_tar=true
            ;;
        txz)
            bext='xz'
            is_tar=true
            ;;
        tZ|taz|taZ)
            bext='Z'
            is_tar=true
            ;;
        tlz)
            bext='lzma'
            is_tar=true
            ;;
        *)
            is_tar=false
            ;;
    esac
}


# Determine the output directory name.
# EXPORTS: odir
get_odir() {
    modpath=
    if [ "${odir_param}" ]; then
        modpath="${bpath}/${odir_param}"
        if [ "${num_archives}" -eq 1 ] || [ "${combine}" ]; then
            odir="${modpath}"
            oname="${odir_param}"
        else
            odir="${modpath}/${bname}"
            oname="${bname}"
        fi
    else
        modpath="${bpath}"
        odir="${modpath}/${bname}"
        oname="${bname}"
    fi
    [ -r "$odir" ] && odir=`handle_conflict "$modpath" "$oname"`
}


handle_conflict() {
    _bpath="$1"
    _oname="$2"
    res_count=1
    newdir="${_bpath}/${_oname}-${TimeStamp}"
    while [ -r "$newdir" ]; do
        newdir="${_bpath}/${_oname}-${TimeStamp}-${res_count}"
        res_count=`expr $res_count '+' '1'`
    done
    echo "$newdir"
}


# =================================================================================================


# In one test I had a file starting with '-' that broke everything. I'm now so
# paranoid that I put '--' after every standard command. It's probably though
# since I don't plan on doing the same for all of the extraction commands
do_extract() {
    modpath= tmp= odir2= oname= BOMB=
    mkdir -p "$odir" && cd "$odir" || exit 20

    # ${A}: The actual full path of the source file, used in the final command.
    # ${B}: The path of the source file relative to the extraction directory.
    #       Only used when echoing the command. Full paths are just safer.
    A="${bpath}/${fname}"
    B="../${fname}"
    echo "mkdir && cd -> `basename "$odir"`"

    # No lazy shortcuts, only an if...else statement is safe here.
    if $is_tar; then
        extract_tar
    else
        extract_else
    fi
    ret=$?
    if [ $ret -ne 0 ]; then
        handle_failure && printf '\nSuccess!\n' >&2 || return $?
    fi

    while [ -z "$combine" ] && [ "`env ls -1 -A "$odir" | wc -l`" -eq 1 ]
    do
        lonefile=`env ls -1 -A "$odir"`
        if (echo "$lonefile" | grep -q '^\.'); then
            newname=`echo "$lonefile" | sed 's/^\.\(.*\)/\1/'`
            mv "${odir}/${lonefile}" "${odir}/${newname}"
            lonefile="${newname}"
        fi

        if [ "$lonefile" = "$bname" ]; then
            tmp=`handle_conflict "$bpath" "$bname"`
            mv "${odir}/${lonefile}" "$tmp"
            cd "$bpath" && rmdir "$odir"
            mv "$tmp" "$odir"

        else
            if [ -z "$SetUsUpTheBomb" ] && [ -d "${odir}/${lonefile}" ]; then
                for d in 'usr' 'bin' 'share' 'lib' 'lib64' 'lib32'; do
                    [ "${d}" = "$lonefile" ] && BOMB='YES' && break
                done
            fi

            if [ -z "$BOMB" ]; then
                if [ "$num_archives" -eq 1 ]; then
                    oname="${odir_param:-${lonefile}}"
                    odir2="${bpath}/${oname}"
                    [ -r "$odir2" ] && odir2=`handle_conflict "$bpath" "$oname"`
                else
                    modpath="${bpath}${odir_param:+/}${odir_param}"
                    odir2="${modpath}/${lonefile}"
                    [ -r "$odir2" ] && odir2=`handle_conflict "$modpath" "$lonefile"`
                fi
                mv "${odir}/${lonefile}" "$odir2"
                cd "$odir2" && rmdir "$odir"
                odir="$odir2"
                odir2=
            fi
        fi
    done

    echo "Extracted to:  `basename "${odir}"`"
    cd "$bpath" || exit 20
}


# Extract a .tar.* archive.
extract_tar() {
    cmd=
    cmdT1=
    cmdT2=
    if [ "x${using}" = 'xtar' ]; then
        cmd="${TAR} -xf"
        echo "${cmd} '${B}'"
        eval "${cmd} '${A}'"
    else
        [ "x${using}" = 'x7zip' ] && bext='7z'
        determine_decompressor "$bext" || return $?
        if [ "$cmdT1" = 'SPECIAL' ]; then
            handle_special_cases "$bext"
        else
            echo "${cmd}${cmdT1} '${B}'${cmdT2} | ${TAR} -xf -"
            eval "${cmd}${cmdT1} '${A}'${cmdT2} | ${TAR} -xf -"
        fi
    fi
}


# Extract anything else.
extract_else() {
    cmd=
    cmdE1=
    cmdE2=
    to_stdout=
    vredir=
    [ "x${using}" = 'x7zip' ] && ext='7z'
    determine_decompressor "$ext" || return $?

    # Stream decompressors will place the output file in the same directory as
    # the archive, so they have to send it to stdout in order for this to work.
    if [ "$to_stdout" ]; then
        echo "${cmd}${cmdE1} '${B}'${cmdE2} > ${bname}"
        eval "${cmd}${cmdE1} '${A}'${cmdE2} > '${odir}/${bname}'"

    # The only way to make certain commands shut the hell up.
    elif [ "$vredir" ]; then
        echo "${cmd}${cmdE1} '${B}'${cmdE2} >/dev/null 2>&1"
        eval "${cmd}${cmdE1} '${A}'${cmdE2} >/dev/null 2>&1"

    else
        echo "${cmd}${cmdE1} '${B}'${cmdE2}"
        eval "${cmd}${cmdE1} '${A}'${cmdE2}"
    fi
}


# NOTE: If a space is required between the end of cmd and the start of the
#       extra option, it must be added to the beginning of that option. The
#       same goes for option 2 which occurs after the source file. This is
#       simply to avoid there being a space when options do not appear, and
#       also to allow the chaining of options where possible. Admittedly this
#       is a little kludgy.
# XXX:  I didn't originally realize that stream decompressors wouldn't output
#       to the working directory, so many commands had different options for T1
#       and E1. After I did realize this, most then had the same value for
#       both. I removed the two variables completely for those cases. Before
#       this nearly all commands used both variables.
# EXPORTS: cmd cmdT1 cmdT2 cmdE1 to_stdout
determine_decompressor() {
    #echo $1
    cmd_exists 'uncompress' && case "$1" in
        z|Z)
            cmd='uncompress -c'
            to_stdout='YES'
            return 0;;
    esac

    cmd_exists 'gzip' && case "$1" in
        gz|z|Z)
            cmd='gzip -dc'
            to_stdout='YES'
            return 0;;
    esac

    cmd_exists 'bzip2' && case "$1" in
        bz|bz2)
            cmd='bzip2 -dc'
            to_stdout='YES'
            return 0;;
    esac

    cmd_exists 'xz' && case "$1" in
        xz|lzma|lz)
            cmd='xz -dc'
            to_stdout='YES'
            return 0;;
    esac

    cmd_exists 'lz4' && case "$1" in
        lz4)
            cmd='lz4 -dc'
            to_stdout='YES'
            return 0;;
    esac

    case "$1" in
        tar|cpio)
            cmd="${TAR} -xf"
            cmdT2=' -'  # NOTE: <-- Space required.
            return 0;;
    esac

    cmd_exists '7z' && [ -z "$no7z" ] && case "$1" in
        7z|gz|bz|bz2|xz|lzma|lz|lz4|zip|cpio|rar|\
        Z|z|jar|deb|rpm|a|ar|iso|img)
            cmd="7z${v7z}"
            cmdT1=' -so x'
            cmdE1=' x'
            return 0;;
    esac

    cmd_exists 'zpaq' && case "$1" in
        zpaq)
            cmd='zpaq x'
            cmdT1='SPECIAL'
            $verb || vredir='YES'
            return 0;;
    esac

    cmd_exists 'zip' && case "$1" in
        zip)
            cmd="unzip${vzip}"
            cmdT1=' -p'
            return 0;;
    esac

    cmd_exists 'arc' && case "$1" in
        arc)
            cmd='arc'
            cmdT1=' p'
            cmdE1=' x'
            return 0;;
    esac

    cmd_exists 'unace' && case "$1" in
        ace|winace)
            cmd='unace x'
            return 0;;
    esac

    cmd_exists 'unrar' && case "$1" in
        rar)
            cmd='unrar x'
            return 0;;
    esac

    if [ -z "${cmd}" ]; then
        [ -z "${FORCE}" ] && cat <<EOF >&2
ERROR: File "${fname}"
       Either format is unrecognized or no known extraction utilities were
       found. Double check whether the required program is installed and is in
       your \$PATH. If unsure, consider attempting to 'force' extract the
       archive with the '-f' flag. If that fails too then this script
       unfortunately cannot extact your file.
EOF
        return 120
    fi
}


# Handle formats whose extractor will not write to the standard output.
handle_special_cases() {
    case "$1" in
        zpaq)
            if [ "$vredir" ]; then
                echo "zpac x '${B}' >/dev/null 2>&1"
                eval "zpaq x '${A}' >/dev/null 2>&1"
            else
                echo "zpac x '${B}'"
                eval "zpaq x '${A}'"
            fi
            for f in .*; do
                if (echo "${f}" | grep -q '.tar$'); then
                    echo "tar -xf && rm -> '${f}'"
                    eval "tar -xf '${f}'"
                    rm -f "${f}"
                    break
                fi
            done
            ;;
        *)
            exit 5
            ;;
    esac
}


handle_failure() {
    # Unzip likes to return failure for no good reason.
    if [ "$ext" = 'zip' ] || [ "$bext" = 'zip' ]; then
        return 0
    elif [ "$FORCE" ]; then
        index=1
        printf '\nAttempting to force extract\n\n' >&2
        while true; do
            if cmd_exists 'patool' && [ $index -eq 1 ]; then
                echo "Trying patool" >&2
                eval "patool extract '${A}'" && return

            elif cmd_exists 'atool' && [ $index -eq 2 ]; then
                echo "Trying atool" >&2
                eval "atool -x '${A}'" && return

            elif cmd_exists '7z' && [ $index -eq 3 ]; then
                echo "Trying 7zip" >&2
                eval "7z x${v7z} '${A}'" && return

            elif cmd_exists 'zpaq' && [ $index -eq 4 ]; then
                echo 'Trying zpaq' >&2
                eval "zpaq x '${A}'" && return

            elif [ $index -eq 5 ]; then
                echo "Trying tar" >&2
                eval "tar -xf '${A}'" && return

            else
                echo 'Total failure' >&2
                break
            fi
            index=`expr $index '+' '1'`
            echo
        done
    fi
    cd "${bpath}" || exit 20
    [ -z "$combine" ] && rmdir "${odir}"
    return 30
}


# =================================================================================================
# Main Code

odir= is_tar= odir_param= combine= prefer= using= SetUsUpTheBomb=
FORCE= no7z= GnuGetoptCMD= ExistChecker= ExistCheckerSh=

VER='xtar version 2.0'
OPTSTRING='hVvo:cbgt7TAf'
LONGOPTS='help,version,verbose,top:,combine,tar:,use7zip,usetar,force'

TimeStamp=`date +%s`
first=true
GnuGetopt=false
debug=false

# Verbiage - defaults to 'shut the hell up'.
verb=false
v7z=' -bso0 -bsp0'
vzip=' -qq'

# Some color literals
NORMAL='\033[0m'
YELLOW='\033[33m'

[ $# -eq 0 ] && ShowUsage 1  # Exit if no paramaters

# I've never heard of GNU Getopt being named ggetopt, but in the tiny chance
# that some UN*X somewhere has done that we should test for it.
for TST in 'getopt' '/usr/bin/getopt' '/usr/local/bin/getopt' 'ggetopt'; do
    ${TST} -T >/dev/null 2>&1
    if [ $? -eq 4 ]; then
        GnuGetopt=true
        GnuGetoptCMD="$TST"
        break
    fi
done

if $GnuGetopt; then
    TEMP=`${GnuGetoptCMD} -n "${0}" -o "${OPTSTRING}N" \
            --longoptions "${LONGOPTS}" -- "$@"` || ShowUsage 2
    eval set -- "$TEMP"

else
    # For portability we're stuck with the POSIX builtin getopts rather than the
    # superior GNU getopt command. It's still much better than BSD's getopt.
    # Nonetheless, we should handle at least a few attempts at GNU style options.
    case "$1" in
        --help)
            ShowUsage 0  # Exits
            ;;
        --version)
            echo "${VER}" && exit 0
            ;;
    esac
fi

while
    if $GnuGetopt; then
        ARG="$1"
        OPTARG="$2"
        true
    else
        getopts "${OPTSTRING}ND" ARG
    fi
do
    case "$ARG" in
        h|-h|--help)
            ShowUsage 0  # Exits
            ;;
        V|-V|--version)
            echo "${VER}" && exit 0
            ;;
        v|-v|--verbose)
            verb=true
            v7z=
            vzip=
            $GnuGetopt && shift
            ;;
        o|-o|--top)
            odir_param="$OPTARG"
            $GnuGetopt && shift 2
            ;;
        c|-c|--combine)
            combine='YES'
            $GnuGetopt && shift
            ;;
        b|-b)
            prefer='bsdtar'
            $GnuGetopt && shift
            ;;
        g|-g)
            prefer='gtar'
            $GnuGetopt && shift
            ;;
        t|-t)
            prefer='tar'
            $GnuGetopt && shift
            ;;
        --tar)
            prefer="$OPTARG"
            shift 2
            ;;
        7|-7|--use7zip)
            using='7zip'
            $GnuGetopt && shift
            ;;
        T|-T|--usetar)
            using='tar'
            $GnuGetopt && shift
            ;;
        A|-A)
            SetUsUpTheBomb='MakeYourTime'
            $GnuGetopt && shift
            ;;
        f|-f|--force)
            FORCE='YES'
            $GnuGetopt && shift
            ;;
        N|-N)
            # UNDOCUMENTED - Avoid 7zip for testing backup commands.
            no7z='YES'
            $GnuGetopt && shift
            ;;
        D|-D)
            debug=true
            ;;
        --)
            shift; break
            ;;
        *)
            ShowUsage 2  # Exits
            ;;
    esac
done
$GnuGetopt || shift `expr $OPTIND '-' '1'`


[ $# -eq 0 ] && echo 'No archive names provided.' >&2 && ShowUsage 3
num_archives=$#


case "$prefer" in
    bsdtar)
        cmd_exists bsdtar && TAR='bsdtar' || TAR='tar'
        ;;
    gtar)
        cmd_exists gtar && TAR='gtar' || TAR='tar'
        ;;
    tar|*)
        TAR='tar'
        ;;
esac


for Archive in "$@"; do
    fname=`basename "$Archive"`
    $first && first=false || echo
    printf -- "${YELLOW}===============================================================================${NORMAL}\n"
    printf -- "${YELLOW}-----  Processing ${fname}  -----${NORMAL}\n"

    [ -f "$Archive" ] || {
        echo "ERROR: File '${Archive}' either doesn't exist or is a directory. Skipping." >&2
        continue
    }

    get_ext
    if ([ -n "$combine" ] && [ -z "$odir" ]) || [ -z "$combine" ]; then
        get_odir
    fi
    do_extract
done
