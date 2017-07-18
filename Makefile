.PHONY: tof clean

tof:
	$(MAKE) -C pru-tof_method/firmware/prudaq

clean:
	$(MAKE) -C pru-tof_method/firmware/prudaq clean
