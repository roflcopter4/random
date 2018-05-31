package main

// #cgo LDFLAGS: D:/Go/USER/lol/lol/contrib/liblzma.lib contrib/libpthread.lib contrib/libz.lib

// #cgo LDFLAGS: -llzma -lpthread -lz
// #cgo CFLAGS: -march=native -Wall -Wextra -Wpedantic -Wformat -g -O3
// #include <stdlib.h>
// extern int cuntbag(int argc, char *argv[]);
import "C"

import (
	"fmt"
	"os"
	"strconv"
	// "strings"
	"unsafe"
)

const (
	/* SEPCHAR        = ";"
	PATHSEP        = '\\' */
	SEPCHAR        = ":"
	PATHSEP        = "/"
	FILE1          = "/home/bml/.vim/dein/repos/github.com/c0r73x/neotags.nvim/neotags_bin/src/neotags.c"
	REQUIRED_INPUT = 8
)

func main() {
	argc := len(os.Args) - 1

	if argc != REQUIRED_INPUT {
		fmt.Printf("Wrong number of arguments (got %d, need %d)\n", argc, REQUIRED_INPUT)
		os.Exit(1)
	}

	/* var (
		files     = strings.Split(os.Args[1], SEPCHAR)
		ctlang    = os.Args[2]
		vimlang   = os.Args[3]
		order     = os.Args[4]
		strip_com = xatoi(os.Args[5]) != 0
		nchars    = xatoi(os.Args[6])
		skip      = strings.Split(os.Args[7], SEPCHAR)
		equiv     = strings.Split(os.Args[8], SEPCHAR)
	) */

	// println("Uhm ", files, ctlang, vimlang, order, strip_com, nchars, skip, equiv)
	// for i := 0; i < len(files)-1; i++ {
	//         println(files[i])
	// }

	args2 := os.Args
	args2 = append(args2, FILE1)

	argv := make([]*C.char, len(os.Args)+1)
	for i, s := range args2 {
		cs := C.CString(s)
		defer C.free(unsafe.Pointer(cs))
		argv[i] = cs
	}

	C.cuntbag(C.int(10), &argv[0])
}

func xatoi(arg string) (ret int) {
	ret, err := strconv.Atoi(arg)
	if err != nil {
		println("Error, invalid integer: ", arg)
		os.Exit(1)
	}
	return
}
