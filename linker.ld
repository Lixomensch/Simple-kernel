ENTRY(loader)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)

PHDRS
{
  text PT_LOAD FLAGS(0x5);   /* Executável e leitura */
  data PT_LOAD FLAGS(0x6);   /* Escrita e leitura */
  bss PT_LOAD FLAGS(0x6);    /* Escrita e leitura */
}

SECTIONS
{
  . = 0x0100000;

  .text ALIGN(4K) : AT(ADDR(.text) - 0x0100000)
  {
    *(.multiboot)
    *(.text*)
    *(.rodata)
  } :text

  .data ALIGN(4K) : AT(ADDR(.data) - 0x0100000)
  {
    start_ctors = .;
    KEEP(*( .init_array ));
    KEEP(*(SORT_BY_INIT_PRIORITY( .init_array.* )));
    end_ctors = .;

    *(.data)
  } :data

  .bss ALIGN(4K) : AT(ADDR(.bss) - 0x0100000)
  {
    *(.bss)
  } :bss

  /DISCARD/ :
  {
    *(.fini_array*) 
    *(.comment)  
  }
}
