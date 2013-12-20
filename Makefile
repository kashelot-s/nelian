SOLUTION=ttyDump

#CFLAGS += -Wall -Wextra
CFLAGS += -I./src
LDFLAGS +=  `pkg-config --cflags --libs plplotd`

ifdef DEBUG
	CFLAGS += -O0 -ggdb
endif

SRC = $(wildcard src/*.c)

all: $(SRC)
	gcc  $(CFLAGS) -o $(SOLUTION) $(LDFLAGS) $^
