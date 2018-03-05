class CircularQueue:

    # Constructor, which creates a new empty queue:
    def __init__(self, capacity):
        if not isinstance(capacity, int) or capacity <= 0:
            raise Exception('Capacity Error')
        self.__items = []
        self.__capacity = capacity
        self.__count = 0
        self.__head = 0
        self.__tail = 0

    def enqueue(self, item):
        if self.__count == self.__capacity:
            raise Exception('Error: Queue is full')
        if len(self.__items) < self.__capacity:
            self.__items.append(item)
        else:
            self.__items[self.__tail] = item
            self.__count += 1
            self.__tail = (self.__tail + 1) % self.__capacity

    # Removes and returns the front - most item in the queue.
    # Returns nothing if the queue is empty.
    def dequeue(self):
        if self.__count == 0:
            raise Exception('Error: Queue is empty')
        item = self.__items[self.__head]
        self.__items[self.__head] = None
        self.__count -= 1
        self.__head = (self.__head + 1) % self.__capacity
        return item

    # Returns the front-most item in the queue, and DOES NOT change the queue.
    def peek(self):
        if self.__count == 0:
            raise Exception('Error: Queue is empty')
        return self.__items[self.__head]

    # Returns True if the queue is empty, and False otherwise:
    def isEmpty(self):
        return self.__count == 0

    # Returns True if the queue is full, and False otherwise:
    def isFull(self):
        return self.__count == self.__capacity

    # Returns the number of items in the queue:
    def size(self):
        return self.__count

    # Returns the capacity of the queue:
    def capacity(self):
        return self.__capacity

    # Removes all items from the queue, and sets the size to 0
    # clear() should not change the capacity
    def clear(self):
        self.__items = []
        self.__count = 0
        self.__head = 0
        self.__tail = 0

    # Returns a string representation of the queue:
    def __str__(self):
        str_exp = "]"
        i = self.__head
        for j in range(self.__count):
            str_exp += str(self.__items[i]) + " "
            i = (i+1) % self.__capacity
        return str_exp + "]"

    # Returns a string representation of the object CircularQueue
    def __repr__(self):
        return str(self.__items) + " H=" + str(self.__head) + " T = " + str(
            self.__tail) + " (" + str(self.__count)+"/" + \
            str(self.__capacity)+")"
