SOLUTION=ttyDump

#CFLAGS += -Wall -Wextra
CFLAGS += -I./src
CFLAGS +=  `pkg-config --cflags plplotd`
LDFLAGS +=  `pkg-config --libs plplotd`

ifdef DEBUG
	CFLAGS += -O0 -ggdb
endif

SRC = $(wildcard src/*.c)

all: $(SRC)
	gcc  $(CFLAGS) -o $(SOLUTION) $(LDFLAGS) $^

clean:
	-rm -rf $(SOLUTION)
	-rm -rf *.raw_data
