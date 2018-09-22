
BIN = xbps-query-custom

OBJS =  main.o list.o show-deps.o show-info-files.o ownedby.o search.o
OBJS += ../lib64/xbps-install-util.o

CPPFLAGS = -I../include -L../lib64
CPPFLAGS += -D_unused="__attribute__((__unused__))"
CPPFLAGS += -DNDEBUG

CFLAGS = -O2

LDFLAGS = -lxbps


.PHONY: all
all: $(BIN)

%.o: %.c
	@printf " [CC]\t\t$@\n"
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

.PHONY: $(BIN)
$(BIN): $(OBJS)
	@printf " [CCLD]\t\t$@\n"
	@$(CC) $^ $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $@

.PHONY: clean
clean:
	-rm -f $(BIN)
	-rm -f *.o
