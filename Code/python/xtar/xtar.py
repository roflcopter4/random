#!/usr/bin/env python3

import sys
import getopt
from shutil import which
from os.path import basename

import xtar_main

DEFAULT_TAR = "bsdtar"

###############################################################################


def main(files, options):
    xtar = xtar_main.xtar(options, len(files))

    counter = 1
    for archive in files:
        if (counter > 1):
            print("\n")
        print("----- Processing file '%s' -----" % archive)

        xtar.init_archive(archive)


###############################################################################


def get_options():
    kwargs = {
        "TAR": find_tar(DEFAULT_TAR),
        "odir": None,
        "verbose": False,
        "combine": False
    }
    options = ["hVvt:bgco:",
               ["help", "version", "verbose", "tar=", "bsdtar", "gtar",
                "combine", "out="]]

    try:
        opts, args = getopt.getopt(sys.argv[1:], *options)
    except getopt.GetoptError as e:
        print(e, '\n')
        show_usage(2)
    else:
        for opt, optarg in opts:
            opt = opt.lstrip('-')
            if opt in ('h', "help"):
                show_usage(0)

            elif opt in ('V', "version"):
                print("xtar version 4.1.0 (python)")

            elif opt in ('v', "verbose"):
                warn("Warning: verbose doesn't do anything yet")
                kwargs["verbose"] = True

            elif opt in ('t', "tar"):
                kwargs["TAR"] = find_tar(optarg)

            elif opt in ('b', "bsdtar"):
                kwargs["TAR"] = find_tar("bsdtar")

            elif opt in ('g', "gtar"):
                kwargs["TAR"] = find_tar("gtar")

            elif opt in ('c', "combine"):
                kwargs["combine"] = True
                warn("Fatal error: Combine not implemented")
                sys.exit(5)

            elif opt in ('o', "out"):
                kwargs["odir"] = optarg

        if not args:
            print("Error: No input files.\n")
            show_usage(1)

        return args, kwargs


def find_tar(binary):
    if which(binary):
        return binary
    else:
        return "tar"


def warn(string):
    print(string, file=sys.stderr)


# -----------------------------------------------------------------------------
###############################################################################
# -----------------------------------------------------------------------------


def show_usage(status):
    if status == 0:
        print("Usage: %s [options] archive(s)..." % basename(sys.argv[0]))
        print("""
Extract an archive safely to a unique directory, ensuring no irritating
single sub-directories or lots of loose files are created. See the manual for
more detailed information.

OPTIONS
 -h --help      Show this usage information.
 -V --version   Show version.
 -v --verbose   Verbose mode. Display progress information if possible.
 -o --out [DIR] Explicitly specify output directory. If it already exists,
                time_t will be appended to it. When used with multiple
                archives it will function as a top directory with each archive
                extracted to sub-directories, unless -c is supplied, whereupon
                all archives are combined into it.
 -c --combine   Combine multiple archives. When -o is not supplied, a directory
                name is generated from the name of the first supplied archive.
                *** NOT IMPLEMENTED ***
 -T --tar [ARG] Explicity specify the tar binary to use.
 -b --bsdtar    Use bsdtar over 'tar' if it exists, otherwise fall back to tar.
 -g --gtar      Use gtar if it exists, otherwise fall back to tar.""")
    else:
        print("Usage: %s [options] archive(s)..." % basename(sys.argv[0]),
              file=sys.stderr)
    sys.exit(status)


if __name__ == "__main__":
    main(*get_options())
