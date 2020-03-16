The implementation is currently known to misbehave with the following
applications.

## kate

When launching kate with a single file `./runoptimized kate README.md` brings
up the window fine. But on exit the program hangs with what is presumably a
race condition:

```
dbus[26895]: type unknown not a basic type
```

By contrast, launching with multiple files: `./runoptimized kate README.md
Makefile` hits what is presumably a race condition when loading the fonts, and
the window comes up with two open tabs but the text is not being rendered. The
error is:

```
Fontconfig error: "/etc/fonts/fonts.conf", line 1: not well-formed (invalid token)
Fontconfig error: Cannot load config file from /etc/fonts/fonts.conf
Fontconfig warning: no <cachedir> elements found. Check configuration.
Fontconfig warning: adding <cachedir>/var/cache/fontconfig</cachedir>
Fontconfig warning: adding <cachedir prefix="xdg">fontconfig</cachedir>
```

## vlc

Running the program without arguments seems to succeed. Trying to play back a
video consistently produces a crash:

```
#0  __memmove_avx_unaligned_erms () at ../sysdeps/x86_64/multiarch/memmove-vec-unaligned-erms.S:371
#1  0x00007ff8478e142f in memmove (__len=<optimised out>, __src=<optimised out>, __dest=<optimised out>)
    at /usr/include/x86_64-linux-gnu/bits/string_fortified.h:40
#2  QListData::prepend (this=this@entry=0x7ff849c7f768) at tools/qlist.cpp:210
#3  0x00007ff847a034e8 in QList<QString>::prepend (t=..., this=0x7ff849c7f768)
    at ../../include/QtCore/../../src/corelib/tools/qlist.h:636
#4  QStandardPaths::standardLocations (type=QStandardPaths::GenericDataLocation) at io/qstandardpaths_unix.cpp:350
#5  0x00007ff8479b114e in QStandardPaths::locateAll (type=<optimised out>, fileName=..., options=...) at io/qstandardpaths.cpp:417
#6  0x00007ff841309900 in ?? () from /usr/lib/x86_64-linux-gnu/qt5/plugins/platformthemes/KDEPlasmaPlatformTheme.so
#7  0x00007ff84130b93f in ?? () from /usr/lib/x86_64-linux-gnu/qt5/plugins/platformthemes/KDEPlasmaPlatformTheme.so
#8  0x00007ff841306f47 in ?? () from /usr/lib/x86_64-linux-gnu/qt5/plugins/platformthemes/KDEPlasmaPlatformTheme.so
#9  0x00007ff841306fb0 in ?? () from /usr/lib/x86_64-linux-gnu/qt5/plugins/platformthemes/KDEPlasmaPlatformTheme.so
#10 0x00007ff84131c856 in ?? () from /usr/lib/x86_64-linux-gnu/qt5/plugins/platformthemes/KDEPlasmaPlatformTheme.so
#11 0x00007ff84801a79a in QPlatformThemeFactory::create(QString const&, QString const&) () from /usr/lib/x86_64-linux-gnu/libQt5Gui.so.5
#12 0x00007ff848028a20 in QGuiApplicationPrivate::createPlatformIntegration() () from /usr/lib/x86_64-linux-gnu/libQt5Gui.so.5
#13 0x00007ff84802945d in QGuiApplicationPrivate::createEventDispatcher() () from /usr/lib/x86_64-linux-gnu/libQt5Gui.so.5
#14 0x00007ff847a74795 in QCoreApplicationPrivate::init (this=0x55e4e033e060) at kernel/qcoreapplication.cpp:823
#15 0x00007ff84802aeef in QGuiApplicationPrivate::init() () from /usr/lib/x86_64-linux-gnu/libQt5Gui.so.5
#16 0x00007ff8489f4659 in QApplicationPrivate::init (this=0x55e4e033e060) at kernel/qapplication.cpp:576
#17 0x00007ff8493b48d6 in ?? () from /usr/lib/x86_64-linux-gnu/vlc/plugins/gui/libqt_plugin.so
#18 0x00007ff8493b5936 in ?? () from /usr/lib/x86_64-linux-gnu/vlc/plugins/gui/libqt_plugin.so
#19 0x00007ff8672916db in start_thread (arg=0x7ff849c80700) at pthread_create.c:463
#20 0x00007ff866db688f in clone () at ../sysdeps/unix/sysv/linux/x86_64/clone.S:95
```

## mpv

The program crashes consistently, even just trying to make it display the help:

```
$ ./runoptimized mpv
./runoptimized: line 15: 27148 Segmentation fault      (core dumped) $@
```

The stack trace points to something in the lua library:

```
#1  0x00005563b348ae09 in  ()
#2  0x00005563b33e5406 in  ()
#3  0x00005563b33e64bb in  ()
#4  0x00005563b33c4e16 in  ()
#5  0x00005563b33c535f in  ()
#6  0x00005563b34155ef in  ()
#7  0x00007fc216336985 in  () at /usr/lib/x86_64-linux-gnu/liblua5.2.so.0
#8  0x00007fc216336ca2 in  () at /usr/lib/x86_64-linux-gnu/liblua5.2.so.0
#9  0x00007fc216335c9c in  () at /usr/lib/x86_64-linux-gnu/liblua5.2.so.0
#10 0x00007fc216335ddf in  () at /usr/lib/x86_64-linux-gnu/liblua5.2.so.0
#11 0x00007fc216335f42 in  () at /usr/lib/x86_64-linux-gnu/liblua5.2.so.0
#12 0x00007fc216341453 in  () at /usr/lib/x86_64-linux-gnu/liblua5.2.so.0
#13 0x00007fc216342c33 in  () at /usr/lib/x86_64-linux-gnu/liblua5.2.so.0
#14 0x00007fc216336cae in  () at /usr/lib/x86_64-linux-gnu/liblua5.2.so.0
#15 0x00007fc216332ceb in lua_callk () at /usr/lib/x86_64-linux-gnu/liblua5.2.so.0
#16 0x00007fc21634d57c in  () at /usr/lib/x86_64-linux-gnu/liblua5.2.so.0
#17 0x00007fc21634d5fc in  () at /usr/lib/x86_64-linux-gnu/liblua5.2.so.0
#18 0x00007fc21634d98a in  () at /usr/lib/x86_64-linux-gnu/liblua5.2.so.0
#19 0x00007fc216336985 in  () at /usr/lib/x86_64-linux-gnu/liblua5.2.so.0
#20 0x00007fc216342545 in  () at /usr/lib/x86_64-linux-gnu/liblua5.2.so.0
#21 0x00007fc216336cae in  () at /usr/lib/x86_64-linux-gnu/liblua5.2.so.0
#22 0x00007fc216332ceb in lua_callk () at /usr/lib/x86_64-linux-gnu/liblua5.2.so.0
#23 0x00005563b341576f in  ()
#24 0x00007fc216336985 in  () at /usr/lib/x86_64-linux-gnu/liblua5.2.so.0
#25 0x00007fc216336ca2 in  () at /usr/lib/x86_64-linux-gnu/liblua5.2.so.0
#26 0x00007fc2163362df in  () at /usr/lib/x86_64-linux-gnu/liblua5.2.so.0
#27 0x00007fc216336f11 in  () at /usr/lib/x86_64-linux-gnu/liblua5.2.so.0
#28 0x00007fc216332db1 in lua_pcallk () at /usr/lib/x86_64-linux-gnu/liblua5.2.so.0
#29 0x00005563b34172d4 in  ()
#30 0x00007fc216336985 in  () at /usr/lib/x86_64-linux-gnu/liblua5.2.so.0
#31 0x00007fc216336ca2 in  () at /usr/lib/x86_64-linux-gnu/liblua5.2.so.0
#32 0x00007fc2163362df in  () at /usr/lib/x86_64-linux-gnu/liblua5.2.so.0
#33 0x00007fc216336f11 in  () at /usr/lib/x86_64-linux-gnu/liblua5.2.so.0
#34 0x00007fc216332db1 in lua_pcallk () at /usr/lib/x86_64-linux-gnu/liblua5.2.so.0
#35 0x00005563b34177f2 in  ()
#36 0x00005563b341ced5 in  ()
#37 0x00007fc218cb56db in start_thread (arg=0x7fc1f09d3700) at pthread_create.c:463
#38 0x00007fc21557a88f in clone () at ../sysdeps/unix/sysv/linux/x86_64/clone.S:95
```
