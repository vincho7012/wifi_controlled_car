CC = xtensa-lx106-elf-gcc
CFLAGS = -I. -mlongcalls
LDLIBS = -nostdlib -Wl,--start-group -lmain -lnet80211 -lwpa -llwip -lpp -lphy -lc -Wl,--end-group -lgcc
LDFLAGS = -Teagle.app.v6.ld

wifi-0x00000.bin: wifi
	esptool.py elf2image $^

wifi: wifi.o

wifi.o: wifi.c

flash: wifi-0x00000.bin
	esptool.py -b 921600 write_flash -fm dio -fs 32m 0 wifi-0x00000.bin 0x10000 wifi-0x10000.bin 0x3fc000 esp_init_data_default.bin

clean:
	rm -f wifi wifi.o wifi-0x00000.bin wifi-0x10000.bin
