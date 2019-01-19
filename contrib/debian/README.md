
Debian
====================
This directory contains files used to package ccd/cc-qt
for Debian-based Linux systems. If you compile ccd/cc-qt yourself, there are some useful files here.

## cc: URI support ##


cc-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install cc-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your ccqt binary to `/usr/bin`
and the `../../share/pixmaps/cc128.png` to `/usr/share/pixmaps`

cc-qt.protocol (KDE)

