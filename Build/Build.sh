# vars
cmsgl="GCC   :   '"
nmsgl="NASM  :   '"
cmsgm="' -> '"
cmsge="'"
outdir="Bin/objs/"
cdir=""
inf=""
outf=""

# Clear the console
clear

# Setup build directory
rm -r "Bin"
mkdir "Bin"
mkdir "Bin/objs"

# Bootstrap.asm
cdir="Source/Kernel/Assembly/" outdir="Bin/" inf="Bootstrap.asm" outf="Bootstrap.o"
nasm -felf32 "$cdir$inf" -o "$outdir$outf"
echo "$nmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"

# RegisterUtil.asm
cdir="Source/Kernel/Assembly/" outdir="Bin/objs/" inf="RegisterUtil.asm" outf="RegisterUtil.o"
nasm -felf32 "$cdir$inf" -o "$outdir$outf"
echo "$nmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"

# IRQs.asm
cdir="Source/Kernel/Assembly/" outdir="Bin/objs/" inf="IRQs.asm" outf="IRQs.o"
nasm -felf32 "$cdir$inf" -o "$outdir$outf"
echo "$nmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"

# ThreadSwitch.asm
cdir="Source/Kernel/Assembly/" outdir="Bin/objs/" inf="ThreadSwitch.asm" outf="ThreadSwitch.o"
nasm -felf32 "$cdir$inf" -o "$outdir$outf"
echo "$nmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"

# ----------------------------------------------- CORE --------------------------------------------------------------
cdir="Source/Kernel/Core/"
for file in $cdir*.cpp
do
    inf="$(basename $file)"
    outf="$(echo ${inf%.*}.o)"
    i686-elf-g++ -w -IInclude -c "$cdir$inf" -o "$outdir$outf" -nostdlib -ffreestanding -O0 -fno-use-cxa-atexit -fno-exceptions -fno-rtti -ffreestanding -mno-red-zone -Wall -Wextra -nostdlib -fno-builtin -fno-builtin-function
    echo "$cmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"
done

# ----------------------------------------------- LIB --------------------------------------------------------------
cdir="Source/Lib/"
for file in $cdir*.cpp
do
    inf="$(basename $file)"
    outf="$(echo ${inf%.*}.o)"
    i686-elf-g++ -w -IInclude -c "$cdir$inf" -o "$outdir$outf" -nostdlib -ffreestanding -O0 -fno-use-cxa-atexit -fno-exceptions -fno-rtti -ffreestanding -mno-red-zone -Wall -Wextra -nostdlib -fno-builtin -fno-builtin-function
    echo "$cmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"
done

# ----------------------------------------------- RUNTIME --------------------------------------------------------------
cdir="Source/Kernel/Runtime/"
for file in $cdir*.cpp
do
    inf="$(basename $file)"
    outf="$(echo ${inf%.*}.o)"
    i686-elf-g++ -w -IInclude -c "$cdir$inf" -o "$outdir$outf" -nostdlib -ffreestanding -O0 -fno-use-cxa-atexit -fno-exceptions -fno-rtti -ffreestanding -mno-red-zone -Wall -Wextra -nostdlib -fno-builtin -fno-builtin-function
    echo "$cmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"
done

# ----------------------------------------------- SYSTEM --------------------------------------------------------------
cdir="Source/Kernel/System/"
for file in $cdir*.cpp
do
    inf="$(basename $file)"
    outf="$(echo ${inf%.*}.o)"
    i686-elf-g++ -w -IInclude -c "$cdir$inf" -o "$outdir$outf" -nostdlib -ffreestanding -O0 -fno-use-cxa-atexit -fno-exceptions -fno-rtti -ffreestanding -mno-red-zone -Wall -Wextra -nostdlib -fno-builtin -fno-builtin-function
    echo "$cmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"
done

# ----------------------------------------------- FS --------------------------------------------------------------
cdir="Source/Kernel/FS/"
for file in $cdir*.cpp
do
    inf="$(basename $file)"
    outf="$(echo ${inf%.*}.o)"
    i686-elf-g++ -w -IInclude -c "$cdir$inf" -o "$outdir$outf" -nostdlib -ffreestanding -O0 -fno-use-cxa-atexit -fno-exceptions -fno-rtti -ffreestanding -mno-red-zone -Wall -Wextra -nostdlib -fno-builtin -fno-builtin-function
    echo "$cmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"
done

# ----------------------------------------------- HAL/COMMON --------------------------------------------------------------
cdir="Source/Kernel/HAL/Common/"
for file in $cdir*.cpp
do
    inf="$(basename $file)"
    outf="$(echo ${inf%.*}.o)"
    i686-elf-g++ -w -IInclude -c "$cdir$inf" -o "$outdir$outf" -nostdlib -ffreestanding -O0 -fno-use-cxa-atexit -fno-exceptions -fno-rtti -ffreestanding -mno-red-zone -Wall -Wextra -nostdlib -fno-builtin -fno-builtin-function
    echo "$cmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"
done

# ----------------------------------------------- HAL/MEMORY --------------------------------------------------------------
cdir="Source/Kernel/HAL/Memory/"
for file in $cdir*.cpp
do
    inf="$(basename $file)"
    outf="$(echo ${inf%.*}.o)"
    i686-elf-g++ -w -IInclude -c "$cdir$inf" -o "$outdir$outf" -nostdlib -ffreestanding -O0 -fno-use-cxa-atexit -fno-exceptions -fno-rtti -ffreestanding -mno-red-zone -Wall -Wextra -nostdlib -fno-builtin -fno-builtin-function
    echo "$cmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"
done

# ----------------------------------------------- HAL/DRIVERS --------------------------------------------------------------
cdir="Source/Kernel/HAL/Drivers/"
for file in $cdir*.cpp
do
    inf="$(basename $file)"
    outf="$(echo ${inf%.*}.o)"
    i686-elf-g++ -w -IInclude -c "$cdir$inf" -o "$outdir$outf" -nostdlib -ffreestanding -O0 -fno-use-cxa-atexit -fno-exceptions -fno-rtti -ffreestanding -mno-red-zone -Wall -Wextra -nostdlib -fno-builtin -fno-builtin-function
    echo "$cmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"
done

# ----------------------------------------------- HAL/INTERRUPTS --------------------------------------------------------------
cdir="Source/Kernel/HAL/Interrupts/"
for file in $cdir*.cpp
do
    inf="$(basename $file)"
    outf="$(echo ${inf%.*}.o)"
    i686-elf-g++ -w -IInclude -c "$cdir$inf" -o "$outdir$outf" -nostdlib -ffreestanding -O0 -fno-use-cxa-atexit -fno-exceptions -fno-rtti -ffreestanding -mno-red-zone -Wall -Wextra -nostdlib -fno-builtin -fno-builtin-function
    echo "$cmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"
done

# Link message
linkmsg="Linking Bootstrap.o "
for file in Bin/objs/*.o
do
    inf="$(basename $file)"
    linkmsg="$linkmsg$inf "
done
echo "$linkmsg"

# Link all files
cd 'Bin/objs'
i686-elf-ld -T '../../Build/Linker.ld' -o '../Kernel.bin' '../Bootstrap.o' *.o
cd '../../'

# Create ISO image
mkdir -p 'Bin/isodir/boot/grub'
cp 'Bin/Kernel.bin' 'Bin/isodir/boot/kernel.bin'
cp 'Build/GrubMenu.cfg' 'Bin/isodir/boot/grub/grub.cfg'
grub-mkrescue -o  'NapalmOS.iso' 'Bin/isodir'
cp 'NapalmOS.iso' 'NapalmOS_flp.img'

i686-elf-gcc -Idisk -o "Programs/test.o" "Programs/test.c" -nostdlib -ffreestanding -Wall -Wextra -O0

# Run QEMU instance of operating system
qemu-system-i386 -m 1024M -vga std -cdrom 'NapalmOS.iso' -serial stdio -boot d -soundhw ac97 -enable-kvm -rtc base=localtime -cpu host
