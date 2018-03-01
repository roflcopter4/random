#!/usr/bin/env python3
#
# I really should have actually read the instructions that clearly stated the
# required lengths of the columns of the chart before writing the program.
# I didn't, but this 'program' generates the correct output anyway so nuts
# to it all.
#
# Vim also whines about leaving extra spaces before assignment operators. I
# removed them, but why on earth is that considered bad style?

from sys import exit, stdout
from datetime import date

# Evil global magic variable. Even more evil because all books are overdue.
today = date(2018, 1, 19)

# ============================================================================
# CLASSES
# ============================================================================

# Make the class names relatively long, with "Library" in front to avoid any
# later confusion that might arise from variables with names like "member"
# (lowercase) should the class be called Member (uppercase).


class LibraryMember:
    requiredInput = 3
    separator = ","

    def __init__(self, phoneNumber, name, address):
        self.name = name
        self.address = address  # Not used
        self.phoneNumber = phoneNumber
        self.books = None
        self.totalDue = 0.0

    def process_books(self, book_list):
        """Takes a list of LibraryBook objects and finds with which of them
        this member shares a phone number. Assigns those to that member's list
        and sums up the total overdue fee, if any, for the member.
        """
        self.books = []
        self.totalDue = 0.0
        for book in book_list:
            if self.phoneNumber == book.phoneNumber:
                self.books.append(book)
                if book.overdueFee:
                    self.totalDue += book.overdueFee


class LibraryBook:
    requiredInput = 4
    separator = ";"
    price_per_day = 0.25  # In dollars
    days_before_sale = 90  # After this many days, the book must be purchased.

    def __init__(self, ID, value, returnDate, phoneNumber):
        self.ID = ID
        self.value = value
        # self.returnDate = date(*map(int, returnDate.split("/")))
        self.returnDate = date(*[int(num) for num in returnDate.split("/")])
        self.phoneNumber = phoneNumber
        self.overdueFee = 0.0
        self.overdueDays = 0

        self.check_overdue()

    def check_overdue(self):
        """Does what it says on the tin. If the book is overdue, calculate the
        total running cost from the number of overdue days and, if applicable,
        the total cost of the book.
        """
        # today = date.today()
        diff = today - self.returnDate
        if diff.days > 0:
            self.overdueDays = diff.days
            self.overdueFee = self.price_per_day * self.overdueDays
            if self.overdueDays > self.days_before_sale:
                self.overdueFee += float(self.value)
        else:
            self.overdueDays = 0


class OutputChart:
    """Creates the entire output chart from a list of members with books
    properly assigned to them. Also contains a method for writing the chart.
    Most of the longer or arbitrary literals are defined here at the top.
    """
    sec1_width = 14
    sec2_padding = 2
    headers = [" Phone Number", " Name", " Due", " Total Dues"]

    def __init__(self, member_list):
        self.member_list = []
        # Remove all members without any overdue books or fees.
        for member in member_list:
            if member.totalDue > 0.0:
                self.member_list.append(member)
        self.totalDue = 0.0

        # If nobody is left in the list, just set the chart to None.
        if len(self.member_list) == 0:
            self.chart = None
        else:
            self.secWidths = [None]*3
            self.hBar = ""
            # Calculate the sizes of each section, then create the chart.
            self.update_values()
            self.chart = self.create_chart()

    def update_values(self):
        """I didn't carefully read the instructions and thought I needed to
        dynamically generate the size of the chart, which this method does.
        It also sorts the members and adds the total amount due.
        """
        # Sort the list by phone number
        self.member_list = sorted(self.member_list,
                                  key=lambda member: member.phoneNumber)
        self.totalDue = sum([member.totalDue for member in self.member_list])

        # Calculate the widths of the two potentially variable sections
        self.secWidths[0] = self.sec1_width
        self.secWidths[1] = max([len(member.name) for member
                                 in self.member_list]) + self.sec2_padding
        self.secWidths[2] = len("$ %.2f" % self.totalDue)

        # Generate the dashed horizontal bar from the widths
        self.hBar = "+"
        for width in self.secWidths:
            self.hBar += width*"-" + "+"

    def create_chart(self):
        """This method is a wasteland of literals and assumptions. It creates
        the output chart dynamically.
        """
        # Having 'self' all over the place makes this mess even messier.
        # I decided to take advantage of the fact that python mostly
        # defines by reference and avoid the problem. I am not sure whether
        # or not this is considered poor style.
        hBar = self.hBar
        secWidths = self.secWidths
        member_list = self.member_list
        headers = self.headers
        totalDue = self.totalDue

        chart = [hBar]
        buf = "|"
        for index in range(0, 3):
            buf += headers[index].ljust(secWidths[index]) + "|"
        chart.append(buf)
        chart.append(hBar)

        for member in member_list:
            # Format and print the phone number and member name.
            num  = member.phoneNumber
            buf  = "|(%s) %s %s|" % (num[:3], num[3:6], num[6:])
            buf += "%s|" % member.name.ljust(secWidths[1])

            # Format and print the amount oweing; justify only the number.
            # Justify to width - 1 because the dollar sign takes one space.
            dueStr = "%.2f" % member.totalDue
            buf += "$%s|" % dueStr.rjust(secWidths[2] - 1)

            # Simply print the books. No fancy formatting at all.
            for book in member.books:
                buf += "[%s](%d); " % (book.ID, book.overdueDays)

            chart.append(buf)

        chart.append(hBar)

        # Print the summary line. No need to justify the third section because
        # the size of the section was calculated from the length of the number.
        # Just add 3 spaces in place of the '|', '$', and ' ' on other lines.
        buf = "|%s|" % headers[3].ljust(secWidths[0])
        buf += "$ ".rjust(secWidths[1])
        buf += " "*3 + "%.2f|" % totalDue
        chart.append(buf)
        chart.append(hBar)

        return chart

    def write_chart(self, filename=None, mode='w'):
        """Writes the whole chart either to a given file or to stdout."""
        if self.chart is not None:
            chart_string = "\n".join(self.chart) + "\n"
        else:
            chart_string = "No members with overdue books!\n"
        if filename is not None:
            try:
                with open(filename, mode) as fp:
                    fp.write(chart_string)
            except IOError:
                print("Error writing to file %s, exiting." % filename)
                exit()
        else:
            stdout.write(chart_string)


# =============================================================================
# FUNCTIONS
# =============================================================================


def parse_file(filename, Class):
    """Parses structured data from given file line by line by splitting it
    with the separator character specified in the given Class, into the number
    of expected variables specified by the Class. An instance of the Class is
    created from each line and a list of them is returned.
    """
    try:
        with open(filename) as fp:
            obj_list = []
            for line in fp.readlines():
                line = line.rstrip("\n")
                data = line.split(Class.separator)

                # Double check that everything was found
                if len(data) != Class.requiredInput:
                    print("ERROR: Data is invalid.\n" + data)
                    exit()
                obj_list.append(Class(*data))
    except IOError:
        print("File '%s' not found, exiting" % filename)
        exit()

    return obj_list


# =============================================================================
# MAIN
# =============================================================================


member_list = parse_file("members.txt", LibraryMember)
book_list = parse_file("books.txt", LibraryBook)

# Assign books to the member in possession of them.
for member in member_list:
    member.process_books(book_list)

# Generate and write the chart to stdout and the file 'summary.txt'.
output = OutputChart(member_list)
output.write_chart()
output.write_chart("summary.txt")
