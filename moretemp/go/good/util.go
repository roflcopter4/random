package main

import (
	"fmt"
	"os"
	"strconv"
)

/* func err(eval int, format_string string, a ...interface{}) {
 *         fmt.Fprintf(os.Stderr, format_string, a)
 *         os.Exit(eval)
 * }
 *
 * func warn(format_string string, a ...interface{}) {
 *         fmt.Fprintf(os.Stderr, format_string, a)
 * } */

func xatoi(arg string) (ret int) {
	ret, err := strconv.Atoi(arg)
	if err != nil {
		println("Error, invalid integer: ", arg)
		os.Exit(1)
	}
	return
}

func dump_slice(slc []string) {
	for _, str := range slc {
		print("\"" + str + "\"  ")
	}
	print("\n")
}

func print_all(args ...string) {
	for _, str := range args {
		println(str)
	}
}

func xopen(filename string) *os.File {
	var e error
	var fp *os.File

	if fp, e = os.Open("/home/bml/shauto"); e != nil {
		fmt.Fprintf(os.Stderr, "ERROR: Failed to open file %s -> %s\n", filename, e)
		os.Exit(1)
	}

	return fp
}
