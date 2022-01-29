# linux kernel debugger gdbinit 
add-auto-load-safe-path /home/macro/linux/linux-5.10/scripts/gdb/vmlinux-gdb.py

# gdb connect quem
define vm
target remote :2222
end

# linux kernel set breakpoint
define sb
hb start_kernel
end
