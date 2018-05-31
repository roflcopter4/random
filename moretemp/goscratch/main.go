package main

import (
        "fmt"
        "os"
        "strconv"
        "strings"
)

const (
        SEPCHAR = ";"
        PATHSEP = '\\'
        FILE1 = "D:\\cygwin64_new\\home\\Brendan\\.vim\\dein\\repos\\github.com\\c0r73x\\neotags.nvim\\neotags_bin\\src\\neotags.c"
        REQUIRED_INPUT = 8
)


func main() {
        var files, skip, equiv []string
        var ctlang, vimlang, order string
        var strip_com bool
        var nchars int
        i, argc := 1, len(os.Args) - 1

        if argc != REQUIRED_INPUT {
                fmt.Printf("Wrong number of arguments (got %d, need %d)\n",
                           argc, REQUIRED_INPUT)
                os.Exit(1)
        }

        files     = strings.Split(os.Args[i], SEPCHAR); i++
        ctlang    = os.Args[i]; i++
        vimlang   = os.Args[i]; i++
        order     = os.Args[i]; i++
        strip_com = xatoi(os.Args[i]) != 0; i++
        nchars    = xatoi(os.Args[i]); i++
        skip      = strings.Split(os.Args[i], SEPCHAR); i++
        equiv     = strings.Split(os.Args[i], SEPCHAR); i++

        println("Uhm ", files, ctlang, vimlang, order, strip_com, nchars, skip, equiv)

        for i := 0; i < len(files) - 1; i++ {
                println(files[i])
        }
}


func xatoi(arg string) (ret int) {
        ret, err := strconv.Atoi(arg)
        if err != nil {
                println("Error, invalid integer: ", arg)
                os.Exit(1)
        }
        return
}
