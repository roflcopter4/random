class CircularQueue:

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

    def dequeue(self):
        """Removes and returns the front - most item in the queue. Returns
        nothing if the queue is empty.
        """
        if self.__count == 0:
            raise Exception('Error: Queue is empty')
        item = self.__items[self.__head]
        self.__items[self.__head] = None
        self.__count -= 1
        self.__head = (self.__head + 1) % self.__capacity
        return item

    def peek(self):
        """Returns the front-most item, and DOES NOT change the queue. """
        if self.__count == 0:
            raise Exception('Error: Queue is empty')
        return self.__items[self.__head]

    def isEmpty(self):
        """Returns True if the queue is empty, and False otherwise:"""
        return self.__count == 0

    def isFull(self):
        """Returns True if the queue is full, and False otherwise:"""
        return self.__count == self.__capacity

    def size(self):
        """Returns the number of items in the queue:"""
        return self.__count

    def capacity(self):
        """Returns the capacity of the queue:"""
        return self.__capacity

    def clear(self):
        """Removes all items from the queue, and sets the size to 0 clear()
        should not change the capacity
        """
        self.__items = []
        self.__count = 0
        self.__head = 0
        self.__tail = 0

    def __str__(self):
        """Returns a string representation of the queue:"""
        str_exp = "["
        i = self.__head
        for j in range(self.__count):
            str_exp += str(self.__items[i]) + " "
            i = (i + 1) % self.__capacity
        return str_exp + "]"

    def __repr__(self):
        """Returns a string representation of the object CircularQueue"""
        s = self
        # return str(s.__items) + " H=" + str(s.__head) + " T = " + str(
        #     s.__tail) + " (" + str(s.__count)+"/" + str(s.__capacity) + ")"
        return "%s H=%d T=%d (%d/%d)" \
            % (str(s.__items), s.__head, s.__tail, s.__count, s.__capacity)
