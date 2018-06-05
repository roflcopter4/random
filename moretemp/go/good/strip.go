package main

/*
#define _GNU_SOURCE
#include <stdlib.h>
extern void strip_comments(char **buffer, size_t len, const char *lang);
extern int find_num_cpus(void);
*/
import "C"

import (
	"os"
	"fmt"
	"time"
	"unsafe"
)

const (
	__C = iota
	__CPP
	__CS
	__GO
	__JAVA
	__PYTHON
)

const (
	C_LIKE = iota
	PYTHON
)

type com struct {
	basic_type uint
	delim      byte
}

type lang_types struct {
	lang    string
	lang_id uint
	basic_type int
	comment byte
}

var languages = []lang_types{
	{ "c",      __C,      C_LIKE, '\000' },
	{ "cpp",    __CPP,    C_LIKE, '\000' },
	{ "cs",     __CS,     C_LIKE, '\000' },
	{ "go",     __GO,     C_LIKE, '\000' },
	{ "java",   __JAVA,   C_LIKE, '\000' },
	{ "python", __PYTHON, PYTHON, '#'    },
}


func c_strip_comments (vim_buf []byte, lang *string) (*C.char) {
	start := time.Now()

	vim_buf_c := C.CString(string(vim_buf))
	lang_c    := C.CString(*lang)
	C.strip_comments(&vim_buf_c, C.size_t(len(vim_buf)), lang_c)
	C.free(unsafe.Pointer(lang_c))

	errf(E, "search_tags took %fs\n", float64(time.Since(start)) / TIME_DIV)
	return vim_buf_c
}


func strip_comments(buffer []byte, lang *string) []byte {
	var ctype *lang_types = nil
	for i := range languages {
		if *lang == languages[i].lang {
			ctype = &languages[i]
			break
		}
	}
	if ctype != nil {
		switch ctype.basic_type {
		case C_LIKE: return handle_cstyle(buffer)
		case PYTHON: return handle_python(buffer)
		}
	}

	println("Language not recognized.")
	return nil
}


/*============================================================================*/
/* C style languages */

const ( NONE = iota; BLOCK; LINE; )

func handle_cstyle(pos []byte) []byte {
	var (
		comment = NONE
		single, double, slash, escape, skip bool
		space int
	)
	buf := make([]byte, len(pos) + 1)

	logf, e := os.Create("/home/bml/anotherlog.log")
	defer logf.Close()
	if e != nil { panic("I fucked up") }
	fmt.Fprintln(logf, "FUCK YOU")

	if len(pos) == 0 {
		println("Error: Empty vim buffer")
		return nil
	}

	println("Stripping C comments")

	QUOTE := func () bool {
		return single || double
	}

	check_quote := func (tocheck, other *bool) {
		if !*other && comment == NONE {
			if *tocheck {
				if !escape {
					*tocheck = false
					skip = true
				}
			} else {
				*tocheck = true
			}
		}
		slash = false
	}

	buf[0] = ' '
	x := 1

	for i := 0; i < len(pos); i++ {
		var c, cm string

		switch pos[i] {
			case '\n': c = "\\n"
			case '\t': c = "\\t"
			default:   c = string(pos[i])
		}

		switch comment {
			case NONE:  cm = "NONE"
			case LINE:  cm = "LINE"
			case BLOCK: cm = "BLOCK"
		}

		switch pos[i] {
		case '/':
			if comment == BLOCK && pos[i - 1] == '*' {
				comment = NONE
				slash = false
				skip = true
			} else if !double {
				if slash && comment == NONE {
					comment = LINE
					x--
				} else {
					slash = true
				}
			}
		case '*':
			if !double && slash {
				if comment == NONE {
					comment = BLOCK
					x--
				}
				slash = false
			}
		case '\n':
			if !escape {
				slash, double = false, false
				if comment == LINE {
					comment = NONE
				}
				if buf[x - 1] == '\n' {
					skip = true
				}
			}
		case '\\': ;
		case '"':  check_quote(&double, &single)
		case '\'': check_quote(&single, &double)
		default:   slash = false
		}

		escape = (pos[i] == '\\' && !escape)

		if (pos[i] == ' ' || pos[i] == '\t') {
			skip = (skip || buf[x - 1] == '\n')
			if skip { space = 0 } else { space++ }
		} else { space = 0 }

		fmt.Fprintf(logf, "pos: %2s, i: %5d, x: %5d, comment: %5s, single: %5t, double: %5t, slash: %5t, escape: %5t, skip: %5t, space: %d\n",
				c, i, x, cm, single, double, slash, escape, skip, space)

		if skip {
			skip = false
		} else if comment == NONE && !QUOTE() && space < 2 {
			buf[x] = pos[i]
			x++
		}
	}

	buf[x] = '\000'
	fmt.Fprintf(logf, "len: %d, x: %d\n", len(buf), x)
	logf.Write(buf)
	return buf[:x+1]
}

/*============================================================================*/
/* Python */

const (
	NO_DOCSTRING = iota
	SINGLE_DOCSTRING
	DOUBLE_DOCSTRING
)

type py_quote struct {
	q     bool
	count int
	delim byte
	kind  int
}

func handle_python(pos []byte) []byte {
	var (
		in_docstring          int = NO_DOCSTRING
		comment, escape, skip bool
		space                 int
	)
	single := py_quote{ false, 0, '\'', SINGLE_DOCSTRING }
	double := py_quote{ false, 0, '"',  DOUBLE_DOCSTRING }

	buf := make([]byte, len(pos)+1)

	if len(pos) == 0 {
		println("Error: Empty vim buffer")
		return nil
	}

	println("Stripping python comments")

	buf[0] = ' '
	x := 1

	logf, e := os.Create("/home/bml/anotherlog.log")
	if e != nil { panic("I fucked up") }
	fmt.Fprintln(logf, "FUCK YOU")

	// These two are direct translations of what were simple macros in the C
	// version of this messy function.
	var QUOTE = func () bool {
		return single.q || double.q || in_docstring != NO_DOCSTRING
	}

	check_docstring := func (tocheck, other *py_quote, i int) {
		if in_docstring != NO_DOCSTRING {
			if tocheck.count == 3 || pos[i-1] == tocheck.delim {
				tocheck.count--
			} else {
				tocheck.count = 3
			}

			if tocheck.count == 0 { in_docstring = NO_DOCSTRING }
			if in_docstring == NO_DOCSTRING { skip = true }
		} else {
			if (tocheck.count == 0 && !(tocheck.q || other.q)) || pos[i-1] == tocheck.delim {
				tocheck.count++
			}

			if tocheck.count == 3 { in_docstring = tocheck.kind }

			if in_docstring != NO_DOCSTRING {
				tocheck.q, other.q = false, false
				skip = true
			} else if !other.q && !comment {
				if tocheck.q {
					if !escape {
						tocheck.q = false
						skip = true
					}
				} else {
					tocheck.q = true
				}
			}
		}
	}

	for i := 0; i < len(pos); i++ {
		if !comment && !QUOTE() && !escape && pos[i] == '#' {
			comment = true
			space = 0
			continue
		}
		if comment && pos[i] != '\n' { continue }

		switch pos[i] {
		case '\n':
			if buf[x-1] == '\n' { skip = true }
			comment = false
			space = 0

		case '"':
			if in_docstring != SINGLE_DOCSTRING {
				check_docstring(&double, &single, i)
			}
			space = 0

		case '\'':
			if in_docstring != DOUBLE_DOCSTRING {
				check_docstring(&single, &double, i)
			}
			space = 0

		case '\t':
			fallthrough
		case ' ':
			if buf[x-1] == '\n' {
				skip = true
			} else {
				space++
			}

		default:
			space = 0
		}

		switch in_docstring {
		case SINGLE_DOCSTRING:
			if single.count < 3 && pos[i] != single.delim { single.count = 3 }
			double.count = 0
		case DOUBLE_DOCSTRING:
			if double.count < 3 && pos[i] != double.delim { double.count = 3 }
			single.count = 0
		case NO_DOCSTRING:
			if single.count > 0 && pos[i] != single.delim { single.count = 0 }
			if double.count > 0 && pos[i] != double.delim { double.count = 0 }
		}

		if pos[i] == '\\' { escape = !escape } else { escape = false }

		if skip {
			skip = false
		} else if !QUOTE() && space < 2 {
			buf[x] = pos[i]
			x++
		}
	}

	buf[x] = '\000'
	logf.Write(buf)
	logf.Close()
	return buf
}
