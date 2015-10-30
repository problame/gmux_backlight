
default: build

docs: gmux_backlight.1

build: gmux_backlight

gmux_backlight: gmux_backlight.c
	gcc -Wall -o gmux_backlight gmux_backlight.c

gmux_backlight.1: gmux_backlight.1.txt LICENSE
	a2x --doctype manpage --format manpage gmux_backlight.1.txt

install: gmux_backlight gmux_backlight.1
	install -m 6755 -D gmux_backlight $(DESTDIR)/usr/bin/gmux_backlight
	install -m 755 -D gmux_backlight.1 $(DESTDIR)/usr/share/man/man1/gmux_backlight.1

remove:
	rm -f $(DESTDIR)/usr/bin/gmux_backlight
	rm -f $(DESTDIR)/usr/share/man/man1/gmux_backlight.1

