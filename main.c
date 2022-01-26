#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

const char hello[] = "Hello, JIT!\n";

int main() {

  // write("Hello, World!")
  // exit(123)
  unsigned char code[] = {
    // mov rsi, hello
    0x48, 0xbe, 1, 2, 3, 4, 5, 6, 7, 8,
    // mov edx, sizeof(hello)
    0xba, 1, 2, 3, 4,

    // mov edi, 1
    0xbf, 1, 0, 0, 0,
    // mov eax, 1
    0xb8, 1, 0, 0, 0,
    // syscall
    0x0f, 0x05,

    // mov eax, 60
    0xb8, 60, 0, 0, 0,
    /// mov edi, 123
    0xbf, 123, 0, 0, 0,
    // syscall
    0x0f, 0x05,
  };

  *(void**) &code[2] = (void*)hello;
  *(int*) &code[11] = sizeof(hello);

  // allocate memory for code
  void* mem = mmap(NULL, sizeof(code), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

  // copy code
  memcpy(mem, code, sizeof(code));

  // allow code execution
  mprotect(mem,  sizeof(code), PROT_EXEC | PROT_READ);

  // execute code
  void (*function)(void) = mem;
  function();
}