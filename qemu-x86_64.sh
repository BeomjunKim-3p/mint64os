qemu-system-x86_64 -L . -m 64 -fda ./Build/Disk.img -rtc base=localtime -M pc -S -gdb tcp::1234
