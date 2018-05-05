GO       = go
GC       = gccgo
OPTFLAGS = -Og -g -march=native
WARNS    = -Wall -Wextra -Wpedantic
ALLFLAGS = ${OPTFLAGS} ${WARNS}
GOFLAGS  = -compiler=${GC}
TARGET   = arse

.if "${GC}" == "gccgo"
    GOFLAGS += -gccgoflags='${ALLFLAGS}'
.else
    GOFLAGS += -gcflags='${ALLFLAGS}'
.endif

.if ${TARGET}
    GOFLAGS += -o ${TARGET}
.endif

###############################################################################

all:
	${GO} build ${GOFLAGS}

clean:
	${GO} clean

remake: clean .WAIT all

.PHONY: clean remake
