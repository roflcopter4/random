import sys


class Day:
    def __init__(self, name, tide_list):
        self.name = name
        self.tides = [float(i[0]) for i in tide_list]
        self.times = [float(i[1]) for i in tide_list]
        tmp = []
        for i in range(len(self.times)):
            tmp.append([self.times[i], self.tides[i]])

        self.stuff = tmp

    def print_self(self):
        print(self.name)
        print(self.stuff)

    def print_max_min(self):
        min_tide = min(self.tides)
        max_tide = max(self.tides)
        print("%s: min tide: %f, max tide: %f" %
              (self.name, min_tide, max_tide))

    def find_highest_tide(self):
        highest = max(self.tides)
        index = self.tides.index(highest)
        return self.stuff[index]

    def find_lowest_tide(self):
        lowest = min(self.tides)
        index = self.tides.index(lowest)
        return self.stuff[index]


def get_averages(day_list):
    high_tides = []
    low_tides = []
    for day in day_list:
        high_tides.append(day.find_highest_tide())
        low_tides.append(day.find_lowest_tide())

    high_average = sum([i[1] for i in high_tides]) / len(high_tides)
    low_average = sum([i[1] for i in low_tides]) / len(low_tides)

    return low_average, high_average


filename = 'waves.txt'
try:
    with open(filename, 'r') as fp:
        results = [i.strip('meters\n').split(',', 1) for i in fp]
except IOError as e:
    print(e)
    sys.exit(1)

results.sort(key=lambda x: x[0])
days = []
day_list = []
i = 0
prev_day = results[0][0]
try:
    while True:
        day = results[i][0]
        tide_list = []
        while day == prev_day:
            tide_list.append(results[i][1].split(','))
            i += 1
            day = results[i][0]

        day_list.append(Day(prev_day, tide_list))
        prev_day = day
except IndexError:
    pass

for day in day_list:
    day.print_max_min()
    day.print_self()

low_average, high_average = get_averages(day_list)

print("The lowest tides appeared on average %f hours after midnight" % low_average)
print("The highest tides appeared on average %f hours after midnight" % high_average)
