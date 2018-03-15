class Tree:
    def __init__(self, name, height, age):
        self.name = name
        self.height = height
        self.age = age

    def by_age(self):
        return self.age

    def by_height(self):
        return self.height

    def __str__(self):
        return "[%s, %dcm, %dys]" % (self.name, self.height, self.age)


trees = [
    #    name        height (cm)  age
    Tree('Oak',      170,         62),
    Tree('Elm',      468,         27),
    Tree('Birch',    278,         45),
    Tree('Juniper',  132,         31)
]

trees.sort(key=lambda x: x.by_age(), reverse=True)
print([str(i) for i in trees])
