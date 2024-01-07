## AK lab 5 (1st)

Yulia Mikhaylenko IO-13

# Requirements

```
export PATH=/opt/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin:$PATH
```
```
export CROSS_COMPILE="ccache arm-linux-gnueabihf-"
```
```
export ARCH=arm
```
```
export KDIR=$HOME/repos/linux-stable
```

# Compile

```
make
```

# Install

```
cp hello5.ko $HOME/repos/busybox/_install/
```
```
cd ~/repos/busybox/_install
```
```
find . | cpio -o -H newc | gzip > ../rootfs.cpio.gz
```

# Start QEMU

```
cd ..
```
```
qemu-system-arm -kernel _install/boot/zImage -initrd rootfs.cpio.gz -machine virt -nographic -m 512 --append "root=/dev/ram0 rw console=ttyAMA0,115200 mem=512M"
```

# Kernel commands

```
insmod hello5.ko
```
```
modinfo hello5.ko
```
```
rmmod hello5
```

# For Debug

```
${CROSS_COMPILE}objdump -dS hello5.ko.unstripped | less
```