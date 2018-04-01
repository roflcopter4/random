"""File class for xtar"""

import re
import subprocess
import sys
from os.path import dirname, realpath, basename


###############################################################################


def check_short_tar(ext):
    if ext in ("tgz"):
        ret = "gz"
    elif ext in ("tbz", "tb2", "tbz2"):
        ret = "bz2"
    elif ext in ("txz"):
        ret = "xz"
    elif ext in ("tZ", "taz", "taZ"):
        ret = "Z"
    elif ext in ("tlz"):
        ret = "lzma"
    else:
        return False, ext

    return True, ret


def backticks(command):
    proc = subprocess.Popen(command, stdout=subprocess.PIPE)
    out, err = proc.communicate()
    return out.decode().rstrip()


###############################################################################


class xtar_file:

    def __init__(self, filename):
        self.filename = basename(filename)
        self.fullpath = realpath(filename)
        self.basepath = dirname(self.fullpath)
        self.bname = None

        self.extention = None
        self.ext_type = None
        self.ext_tar = None
        self.ext_cmd = None

        self.mime_raw = None
        self.mime_type = None
        self.mime_tar = None
        self.mime_cmd = None

        self.likely_type = None
        self.likely_tar = None
        self.likely_cmd = None

        self.extention_analysis()
        self.mimetype_analysis()

    def extention_analysis(self):
        self.extention = re.sub(".*\.(.*)", r"\1", self.filename)

        if re.search("\.tar\..*", self.filename) is not None:
            self.ext_tar = True
            self.bname = re.sub("(.*)\.tar\..*", r"\1", self.filename)
        else:
            self.ext_tar, self.extention = check_short_tar(self.extention)
            self.bname = re.sub("(.*)\..*", r"\1", self.filename)

    def mimetype_analysis(self):
        app = backticks(["file", "--mime-type", self.fullpath]).lower()

        if not re.search("application", app):
            print("YOU WHORE")
            sys.exit(10)

        app = re.sub(".*?x-(.*)", r"\1", app)

    #
    #
    def __str__(self):
        buf = "DUMPING XTAR_FILE\n"
        buf += "filename: %s\n" % str(self.filename)
        buf += "fullpath: %s\n" % str(self.fullpath)
        buf += "basepath: %s\n" % str(self.basepath)
        buf += "bname: %s\n" % str(self.bname)
        buf += "extention: %s\n" % str(self.extention)
        buf += "ext_type: %s\n" % str(self.ext_type)
        buf += "ext_tar: %s\n" % str(self.ext_tar)
        buf += "ext_cmd: %s\n" % str(self.ext_cmd)
        buf += "mime_raw: %s\n" % str(self.mime_raw)
        buf += "mime_type: %s\n" % str(self.mime_type)
        buf += "mime_tar: %s\n" % str(self.mime_tar)
        buf += "mime_cmd: %s\n" % str(self.mime_cmd)
        buf += "likely_type: %s\n" % str(self.likely_type)
        buf += "likely_tar: %s\n" % str(self.likely_tar)
        buf += "likely_cmd: %s" % str(self.likely_cmd)

        return buf
