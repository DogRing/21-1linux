cmd_/home/hwan/21-1linux/assignment/simple_module/hello_module.ko := ld -r -m elf_x86_64  -z max-page-size=0x200000  --build-id  -T ./scripts/module-common.lds -o /home/hwan/21-1linux/assignment/simple_module/hello_module.ko /home/hwan/21-1linux/assignment/simple_module/hello_module.o /home/hwan/21-1linux/assignment/simple_module/hello_module.mod.o;  true