# This will compile the new kernel, then apply it, then reboot.
# To run type ./compileAndInstall.sh
make -j8
make modules
make modules_install
export KERNELVERSION=$(make kernelversion)
cp arch/x86/boot/bzImage /boot/vmlinuz-$KERNELVERSION
cp System.map /boot/System.map-$KERNELVERSION
new-kernel-pkg --mkinitrd --install $KERNELVERSION
reboot
