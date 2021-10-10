1. Create a cs300 inside linux-stable (cd linux-5.4.109)
2. Put all .c and .h files into cs300
3. Run the makefile for the KERNEL
4. In the linux-stable Makefile
	a. find the line which defines core-y (near line 1039)
	b. add cs300 folder to the end ".../block/" = ".../block/cs300/"
5. In syscall_64.tbl (dir: arch/x86/entry/syscalls/syscall_64.tbl), add the following line at the end of the first block of sys-call defines 
	(i.e. after the call numbered ~360, and before the second block defining calls in the 512+ range): 437 common array_stats __x64_sys_array_stats
6. Rebuild kernel -j4 ($make -j4)
7. Outside of the linux-stable, put array_stats Makefile and array_stats_test.c
8. Run the Makefile (This Makefile should automatically transfer the executable to VM)
9. Test code in VM using ./test