package main

/*
#cgo CFLAGS: -O3 -march=native
#cgo LDFLAGS: -lz -llzma
#include <malloc.h>
extern char * gz_getlines (const char *filename);
extern char * xz_getlines (const char *filename); */
import "C"

import (
	"fmt"
	"os"
	"unsafe"
	"strings"
)

const (
	GZIP = 1
	LZMA = 2
)

func getlines(tags *[]string,
	      comp_type, filename string) int {
	switch (comp_type) {
	case "none": plain_getlines(tags, filename)
	case "gzip": c_getlines(tags, filename, GZIP)
	case "lzma": c_getlines(tags, filename, LZMA)
	default:
		println("Error: invalid compression type: ", comp_type)
		return 0
	}

	return 1
}


func strsep(tags *[]string, buf *string) {
	slc := strings.Split(*buf, "\n")
	*tags = append(*tags, slc...)
}


func plain_getlines(tags *[]string, filename string) {
	fp      := xopen(filename)
	st, _   := fp.Stat()
	filebuf := make([]byte, st.Size())

	if i, e := fp.Read(filebuf); e != nil {
		fmt.Fprintf(os.Stderr, "Failed to read file \"%s\": %s\n", filename, e)
	} else {
		println("Read", i, "bytes")
	}
	filestr := string(filebuf)

	strsep(tags, &filestr)
}


func c_getlines(tags *[]string, filename string, f int) {
	var (
		cstr =C.CString(filename)
		filebuf *C.char
		filestr string
	)

	switch f {
	case GZIP: filebuf = C.gz_getlines(cstr)
	case LZMA: filebuf = C.xz_getlines(cstr)
	}

	filestr = C.GoString(filebuf)
	println("Read", len(filestr), "chars.")

	strsep(tags, &filestr)
	C.free(unsafe.Pointer(cstr))
	C.free(unsafe.Pointer(filebuf))
}
