CC=gcc
CFLAGS = -O2 -Wall -Wextra -Werror
INCLUDES=-I../../share
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)

OUTNAME=adc-controller.out

DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) $(INCLUDES) -c
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

.PHONY: clean

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(OUTNAME) main.c

%.o : %.c
%.o : %.c $(DEPDIR)/%.d
			$(COMPILE.c) $<
			$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

clean:
	rm -rf $(OBJECTS) .d $(OUTNAME)

-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SOURCES)))
