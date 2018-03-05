import sys

try:
    a = int(input())
    b = int(input())
except ValueError as err:
    print("Error: %s", err, file=sys.stderr)
    sys.exit(1)

if a < b:
    print("%d is less than %d." % (a, b))
elif a == b:
    print("%d is equal to %d." % (a, b))
else:
    print("%d is greater than %d." % (a, b))
