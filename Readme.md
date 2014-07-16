Helper for ASUS media keys
====

Thanks to:  
https://bitbucket.org/zaak/atkmediainterceptor


This is an utility for the ASUS media keys. Normally it would start windows media player or whatever, but this sends the proper media_play/pause events to programs.

Installation
----

Build with mingw:  
make  
then move output.exe to C:/Program Files (x86)/ASUS/ATK Package/ATK Media/DMedia.exe (kill DMedia.exe in the taskmanager if it complains)  
Backup the old DMedia.exe if you want.

If you want, place settings.conf next to DMedia.exe for setting what the quick key and the calculator key should do. The default is a command prompt and the calculator respectively.
