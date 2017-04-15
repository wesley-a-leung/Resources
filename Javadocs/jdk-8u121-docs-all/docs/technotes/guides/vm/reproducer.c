#include <numaif.h>
#include <numa.h>
#include <stddef.h>
#include <sys/mman.h>
#include <stdint.h>

int main(void) {
   if (numa_all_nodes_ptr == (void*)0) {
     return -1;
   }

   size_t pagesize = getpagesize();

   void* mapped_memory = mmap(NULL, 3 * pagesize, PROT_READ|PROT_WRITE,
MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
   if (mapped_memory == MAP_FAILED) {
     return -2;
   }

   void* page0 = mapped_memory;
   void* page1 = (void*)((uintptr_t)page0 + pagesize);
   void* page2 = (void*)((uintptr_t)page1 + pagesize); 

 // Set up the last page as interleaved.
   mbind(page2, pagesize, MPOL_INTERLEAVE, numa_all_nodes_ptr->maskp,
numa_all_nodes_ptr->size, 0);

   // Setup the last two pages as interleaved.
   mbind(page1, 2 * pagesize, MPOL_INTERLEAVE,
numa_all_nodes_ptr->maskp, numa_all_nodes_ptr->size, 0);

   *((char*)page2) = 2;
   *((char*)page1) = 1;
   *((char*)page0) = 0; // Crash here, when mbind_merge was broken.

   return 0;
}

