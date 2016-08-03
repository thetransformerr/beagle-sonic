.PHONY: all clean

all:
	cd src/firmware/ths1206/pru0 && $(MAKE)
	cd src/firmware/ths1206/pru1 && $(MAKE)
	cd src/software/adc-controller && $(MAKE)

clean:
	cd src/firmware/ths1206/pru0 && $(MAKE) clean
	cd src/firmware/ths1206/pru1 && $(MAKE) clean
	cd src/software/adc-controller && $(MAKE) clean