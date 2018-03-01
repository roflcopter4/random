#!/bin/sh

./calc | sed 's/=/\./' | column -t -s'.' -o'.' -R2 | sed 's/\./=/'
