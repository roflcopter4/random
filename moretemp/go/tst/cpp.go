package main

//go:generate

import (
	"fmt"
	"os"
)

func main() {
	var (e error
	     fp *os.File
	     i int)

	if fp, e = os.Open("/home/bml/shauto"); e != nil {
		os.Exit(1)
	}

	st,_ := fp.Stat()
	size := st.Size()
	filebuf := make([]byte, size)

	if i, e = fp.Read(filebuf); e != nil {
		println(e)
	} else {
		fmt.Printf("Read %d bytes\n", i)
	}

	str := string(filebuf)
	print(str)
}
