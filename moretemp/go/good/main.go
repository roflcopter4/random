package main

/*
#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
extern int find_num_cpus(void);
*/
import "C"

import (
	"fmt"
	"github.com/mattn/go-isatty"
	"io/ioutil"
	"os"
	"runtime"
	"strings"
	"sync"
	"time"
	"unsafe"
	// "bytes"
)

type tagdata struct {
	s    string
	kind byte
}

type thread_data struct {
	tags, skip, equiv []string
	// vim_buf           []byte
	vim_buf           *C.char
	lang, order       *string
	start, num        int
}

const (
	SEPCHAR        = ":"
	PATHSEP        = "/"
	REQUIRED_INPUT = 8
	INIT_CAP       = 1024
	TIME_DIV       = 1000000000
)

var errf = fmt.Fprintf
var E = os.Stderr
var NUM_THREADS = int(C.find_num_cpus())

func main() {
	runtime.GOMAXPROCS(NUM_THREADS)
	argc := len(os.Args) - 1

	if isatty.IsTerminal(os.Stdin.Fd()) {
		errf(E, "This program cannot be run manually.\n")
		os.Exit(10)
	} else if argc != REQUIRED_INPUT {
		errf(E, "Wrong number of arguments (got %d, need %d)\n", argc, REQUIRED_INPUT)
		os.Exit(20)
	}

	println("Program id: GO")

	var (
		files  = strings.Split(os.Args[1], SEPCHAR)
		ctlang = os.Args[2]
		vimlang = os.Args[3]
		order = os.Args[4]
		// strip_com = xatoi(os.Args[5]) != 0
		// nchars    = xatoi(os.Args[6])
		skip  = strings.Split(os.Args[7], SEPCHAR)
		equiv = strings.Split(os.Args[8], SEPCHAR)
	)

	// dump_slice(files)
	// dump_slice(skip)
	// dump_slice(equiv)

	tags := make([]string, 0, INIT_CAP)
	files_read := 0

	for i := 0; i < len(files); i += 2 {
		files_read += getlines(&tags, files[i], files[i+1])
	}
	if files_read == 0 {
		println("Error: no tag files were successfully read.")
		os.Exit(1)
	}

	// vim_buf_b := make([]byte, nchars + 1)
	vim_buf_b, err := ioutil.ReadAll(os.Stdin)
	if err != nil {
		println("Error reading stdin:", err)
		println("Managed to read", len(vim_buf_b), "bytes.")
		os.Exit(1)
	} else {
		println("Successfully read", len(vim_buf_b), "bytes.")
	}

	/* start := time.Now()
	if tmp := strip_comments(vim_buf_b, &vimlang); tmp == nil {
		vim_buf_b = append(vim_buf_b, '\000')
	} else {
		vim_buf_b = tmp
		println("len(main): ", len(vim_buf_b))
	}
	errf(E, "removing comments took %fs\n", float64(time.Since(start)) / TIME_DIV) */

	start := time.Now()
	data  := search_tags(tags, skip, equiv, vim_buf_b, &ctlang, &vimlang, &order)
	errf(E, "search_tags took %fs\n", float64(time.Since(start)) / TIME_DIV)

	filt_data := remove_dups(data)
	for _, tmp := range filt_data { fmt.Printf("%c\n%s\n", tmp.kind, tmp.s) }
}

/* ========================================================================== */

func in_order(equiv []string, order *string, kind *byte) bool {
	for _, s := range equiv {
		if len(s) == 0 { continue }
		equiv_a := []byte(s)
		if *kind == equiv_a[0] {
			*kind = equiv_a[1]
			break
		}
	}

	return strings.ContainsRune(*order, rune(*kind))
}

func is_correct_lang(lang, match_lang *string) bool {
	*lang = strings.ToLower(*lang)
	*match_lang = strings.ToLower(*match_lang)

	if *lang == *match_lang { return true }

	return ((*lang == "c" || *lang == "c\\+\\+") &&
		(*match_lang == "c" || *match_lang == "c++"))
}

func skip_tag(skip []string, find *string) bool {
	for _, s := range skip {
		if s == *find { return true }
	}
	return false
}

/* ========================================================================== */

func search_tags(tags, skip, equiv []string,
		 vim_buf []byte,
		 lang, vimlang, order *string) []tagdata {
	out := make([][]tagdata, NUM_THREADS)
	println("There are", len(tags), "tags to search; using", NUM_THREADS, "cpus.")

	var wg sync.WaitGroup
	wg.Add(NUM_THREADS)

	vim_buf_c := c_strip_comments(vim_buf, vimlang)
	defer C.free(unsafe.Pointer(vim_buf_c))

	for i := 0; i < NUM_THREADS; i++ {
		var quot int = len(tags) / NUM_THREADS
		var num int
		if i == (NUM_THREADS - 1) {
			num = len(tags) - ((NUM_THREADS - 1) * quot)
		} else {
			num = quot
		}

		// tdata := thread_data{tags, skip, equiv, vim_buf, lang, order, i * quot, num}
		tdata := thread_data{tags, skip, equiv, vim_buf_c, lang, order, i * quot, num}
		go threaded_search(&out[i], &tdata, &wg)
	}

	wg.Wait()
	alldata := make([]tagdata, 0, len(tags))

	for i := 0; i < NUM_THREADS; i++ {
		alldata = append(alldata, out[i]...)
	}

	return alldata
}


func threaded_search(ret *[]tagdata, data *thread_data, wg *sync.WaitGroup) {
	defer wg.Done()

	*ret = make([]tagdata, 0, data.num)

	for i := data.start; i < (data.start + data.num) && i < len(data.tags); i++ {
		var cur_str string = data.tags[i]

		if len(cur_str) == 0 || cur_str[0] == '!' { continue }
		var (
			name            = ""
			match_lang      = ""
			kind       byte = '\000'
		)

		tokens := strings.Split(cur_str, "\t")

		for i, tok := range tokens {
			if i == 0 {
				name = tok
			} else if len(tok) == 1 {
				kind = tok[0]
			} else if strings.HasPrefix(tok, "language:") && len(tok) > 10 {
				match_lang = tok[9:]
			}
		}

		if kind == '\000' || match_lang == "" { continue }
		tmp := []byte(name)
		tmp = append(tmp, '\000')

		if in_order(data.equiv, data.order, &kind) &&
		   is_correct_lang(data.lang, &match_lang) &&
		   !skip_tag(data.skip, &name) &&
		   C.strstr(data.vim_buf, (*C.char)(unsafe.Pointer(&tmp[0]))) != nil {
			tmpdata := tagdata{name, kind}
			*ret = append(*ret, tmpdata)
		}
		   // C.strstr((*C.char)(unsafe.Pointer(&data.vim_buf[0])),
		   // bytes.Contains(data.vim_buf, []byte(name)) {
	}
}


func remove_dups(data []tagdata) []tagdata {
	start := time.Now()
	quick_sort(data, int64(len(data)))
	errf(E, "Sorting took %fs\n", float64(time.Since(start)) / TIME_DIV)

	start = time.Now()

	ret := make([]tagdata, len(data))
	ret[0] = (data[0])
	x := 1

	for i := 1; i < len(data); i++ {
		if data[i] != data[i-1] {
			ret[x] = data[i]
			x++
		}
	}
	ret = ret[:x]
	errf(E, "Eliminating duplicates took %fs\n", float64(time.Since(start)) / TIME_DIV)

	return ret
}
