
default: build

build: gmux_backlight

gmux_backlight: gmux_backlight.c
	gcc -Wall -o gmux_backlight gmux_backlight.c

install: gmux_backlight
	install -m 6755 -D gmux_backlight $(DESTDIR)/usr/bin/gmux_backlight

remove:
	rm /usr/bin/gmux_backlight
