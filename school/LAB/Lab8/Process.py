#!/usr/bin/env python3

import sys
FILE = 'input.txt'

##############################################################################
# Classes


class Data:
    def __init__(self, data):
        self.grades = data
        self.maxi = max(self.grades)
        self.mini = min(self.grades)
        self.mean = sum(self.grades) / len(self.grades)

        self.sections = self.make_sections()

    def make_sections(self):
        array = [[]]
        secmax = 9
        ind = 0
        for grade in self.grades:
            if grade > secmax:
                secmax = (secmax + 10) if (secmax != 99) else (100)
                array.append([])
                ind += 1
            array[ind].append(grade)

        return array

    def __str__(self):
        s = self
        buf = "Average: %s\nMax: %s\nMin: %s\n" % (s.mean, s.maxi, s.mini)
        for sec in self.sections:
            buf += str(sec) + "\n"
        return buf[:-1]


##############################################################################


class HTML:
    TAB = 4 * ' '

    def __init__(self):
        self.output = "<html>\n\n"
        self.stack = []
        self.level_stack = []
        self.level = 0

    def __add_content(self, content):
        stuff = content.split("\n")
        for line in stuff:
            line.rstrip()
            self.output += (self.TAB * self.level) + line + '\n'

    def __check_stack(self):
        if self.stack:
            self.__add_content(self.stack.pop())
            self.stack.append('')

    def __do_add(self, sec):
        self.stack.append(sec)
        self.stack.append('')
        self.level_stack.append(self.level)

    def add(self, lst):
        self.__check_stack()
        for sec in lst:
            self.__do_add(sec)
            self.output += (self.TAB * self.level) + "<%s>\n" % sec
            self.level += 1

    def css(self, sec, fmt):
        self.__check_stack()
        self.__do_add(sec)
        self.output += (self.TAB * self.level) + "<%s %s>\n" % (sec, fmt)
        self.level += 1

    def end(self, num=None):
        num = (1) if (num is None) else (num)
        for _ in range(num):
            content = self.stack.pop()
            section = self.stack.pop()

            if content:
                self.__add_content(content)
            self.level = self.level_stack.pop()

            self.output += (self.TAB * self.level) + "</%s>\n" % section
            if self.level_stack == 0:
                self.output += '\n'

    def lit(self, content):
        self.stack[-1] += content

    def para(self, content):
        content = "\n".join([i + "</br>" for i in content.splitlines()])

        if content[-1] != '\n':
            content += '\n'
        self.stack[-1] += "<p>\n%s</p>\n" % content

    def finish(self):
        self.output += "</html>\n"


##############################################################################
# Functions


def get_data():
    try:
        with open(FILE, 'r') as file_handle:
            data = [i.rstrip().split() for i in file_handle]
            # data = list(filter(lambda x: len(x), data))
            data = list(filter(len, data))
            return sorted([int(i[1]) for i in data])
    except FileNotFoundError as err:
        print(err)
        sys.exit()


##############################################################################
# Main

DATA = Data(get_data())
# print(str(DATA))

RANGES = ["[%2d-%-3d]" % (i, i+9) for i in range(0, 81, 10)]
RANGES.append("[90-100]")

H = HTML()

H.add(['body', 'h1'])
H.lit("This took so ridiculously long it's seriously embarrassing.")
H.end(2)

H.add(['body'])
H.para("Class average: %d\nHighest grade: %d\nLowest grade: %d\n"
       % (DATA.mean, DATA.maxi, DATA.mini))

H.add(['table', 'tr'])
width = 13

for sect in DATA.sections:
    length = len(sect) * 20
    H.css('td', 'valign="bottom"')
    H.lit("""\
<div style="width:62px;height: %dpx ;background:blue; border:1px solid red; writing-mode:tb-rl">
</div>""" % length)
    H.end()

    H.css('td', 'valign="bottom"')
    H.lit("""<div style="width:%dpx;height: %dpx ;background:white;">\n</div>""" % (width, length))
    H.end()
    width += 1

H.end(2)
H.css('font', "face=courier size='3'")
H.lit(" ".join(RANGES))
H.end(2)

H.finish()
print(H.output, end="")
