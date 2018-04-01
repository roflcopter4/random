"""Main package file for xtar"""

from pprint import pprint
from os import getcwd

from xtar_file import xtar_file


class xtar:
    cmd_color = "bMagenta"

    def __init__(self, options, NumArchives):
        self.options = options
        self.NumArchives = NumArchives
        self.CWD = getcwd()
        self.counter = 0
        self.archive = None
        self.out = None
        self.tmpdir = None

    def init_archive(self, filename):
        self.archive = xtar_file(filename)
        print(str(self.archive))
