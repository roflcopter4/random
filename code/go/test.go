package main

/*
#cgo LDFLAGS: -lbsd
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <bsd/bsd.h>
#include <stdarg.h>

char *
wrapper(const char *fmt, ...)
{
        va_list *ap;
        va_start(ap, fmt);
        char *buf;
        vasprintf(buf, fmt, ap);
        va_end(ap);
        return buf;
}

*/
// import "C"

import (
    // "os"
    "fmt"
    // "unsafe"
)


const (
        x = 12
        y = 32
);


type arse struct {
        a, b, c int;
};


func main() {
        me := arse{1, 2 ,3}
        you := &me

        fmt.Printf("That is: %d - %d - %d\n", me.a, me.b, me.c)
        fmt.Printf("What is: %d - %d - %d\n", you.a, you.b, you.c)
        println("Also ->", x)
}
