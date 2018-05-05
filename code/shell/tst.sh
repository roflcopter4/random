#!/bin/sh

msgprefix=$(basename "$0")
dst="$1"
shift

for src in "$@"; do
    #   determine if one of the paths is an absolute path,
    #   because then we _have_ to use an absolute symlink
    oneisabs=0
    srcisabs=0
    dstisabs=0
    case $src in
        /*)
            oneisabs=1
            srcisabs=1
            ;;
    esac
    case $dst in
        /*)
            oneisabs=1
            dstisabs=1
            ;;
    esac

    #   split source and destination into dir and base name
    if [ -d "$src" ]; then
        srcdir=$(echo "$src" | sed -e 's;/*$;;')
        srcbase=""
    else
        srcdir=$(echo "$src" | sed -e 's;^[^/]*$;;' -e 's;^\(.*/\)[^/]*$;\1;' -e 's;\(.\)/$;\1;')
        srcbase=$(echo "$src" | sed -e 's;.*/\([^/]*\)$;\1;')
    fi
    if [ -d "$dst" ]; then
        dstdir=$(echo "$dst" | sed -e 's;/*$;;')
        dstbase=""
    else
        dstdir=$(echo "$dst" | sed -e 's;^[^/]*$;;' -e 's;^\(.*/\)[^/]*$;\1;' -e 's;\(.\)/$;\1;')
        dstbase=$(echo "$dst" | sed -e 's;.*/\([^/]*\)$;\1;')
    fi

    #   consistency check
    if [ ".$dstdir" != . ]; then
        if [ ! -d "$dstdir" ]; then
            echo "$msgprefix:Error: destination directory not found: $dstdir" 1>&2
            shtool_exit 1
        fi
    fi

    #   make sure the source is reachable from the destination
    if [ $dstisabs = 1 ]; then
        if [ $srcisabs = 0 ]; then
            if [ ".$srcdir" = . ]; then
                srcdir="$(pwd | sed -e 's;/*$;;')"
                srcisabs=1
                oneisabs=1
            elif [ -d "$srcdir" ]; then
                srcdir="$(
                    cd "$srcdir" || exit 100
                    pwd | sed -e 's;/*$;;'
                )"
                srcisabs=1
                oneisabs=1
            fi
        fi
    fi

    #   split away a common prefix
    prefix=""
    if [ ".$srcdir" = ".$dstdir" ] && [ ".$srcdir" != . ]; then
        prefix="$srcdir/"
        srcdir=""
        dstdir=""
    else
        while [ ".$srcdir" != . ] && [ ".$dstdir" != . ]; do
            presrc=$(echo $srcdir | sed -e 's;^\([^/]*\)/.*;\1;')
            predst=$(echo $dstdir | sed -e 's;^\([^/]*\)/.*;\1;')
            if [ ".$presrc" != ".$predst" ]; then
                break
            fi
            prefix="$prefix$presrc/"
            srcdir=$(echo $srcdir | sed -e 's;^[^/]*/*;;')
            dstdir=$(echo $dstdir | sed -e 's;^[^/]*/*;;')
        done
    fi

    #   destination prefix is just the common prefix
    dstpre="$prefix"

    #   determine source prefix which is the reverse directory
    #   step-up corresponding to the destination directory
    srcpre=""
    allow_relative_srcpre=no
    if [ ".$prefix" != . ] && [ ".$prefix" != ./ ]; then
        allow_relative_srcpre=yes
    fi
    if [ $oneisabs = 0 ]; then
        allow_relative_srcpre=yes
    fi
    # if [ ".$opt_s" != .yes ]; then
    #     allow_relative_srcpre=no
    # fi
    if [ ".$allow_relative_srcpre" = .yes ]; then
        pl="$dstdir/"
        OIFS="$IFS"
        IFS='/'
        for pe in $pl; do
            [ ".$pe" = . ] && continue
            [ ".$pe" = .. ] && continue
            srcpre="../$srcpre"
        done
        IFS="$OIFS"
    else
        if [ $srcisabs = 1 ]; then
            srcpre="$prefix"
        fi
    fi

    #   determine destination symlink name
    if [ ".$dstbase" = . ]; then
        if [ ".$srcbase" != . ]; then
            dstbase="$srcbase"
        else
            dstbase=$(echo "$prefix$srcdir" | sed -e 's;/*$;;' -e 's;.*/\([^/]*\)$;\1;')
        fi
    fi

    echo "'$srcpre$srcdir$srcbase'  -->  '$dstpre$dstdir$dstbase'"

    #   now finalize source and destination directory paths
    srcdir=$(echo "$srcdir" | sed -e 's;\([^/]\)$;\1/;')
    dstdir=$(echo "$dstdir" | sed -e 's;\([^/]\)$;\1/;')

    # #   run the final link command
    # if [ ".$opt_t" = .yes ]; then
    #     echo "ln$lnopt $srcpre$srcdir$srcbase $dstpre$dstdir$dstbase"
    # fi
    # eval "ln$lnopt $srcpre$srcdir$srcbase $dstpre$dstdir$dstbase"
done
