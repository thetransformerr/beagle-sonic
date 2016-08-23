.PHONY: all clean

all:
	cd src/firmware/prudaq && $(MAKE)

clean:
	cd src/firmware/prudaq && $(MAKE) clean