// Kenneth Witham
// NUID 001068036

#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// translate virtual to physical address
// Return: Pointer to physical address
// Errors: -1 for input argument error
//         -2 for page not allocated
int
sys_translate(void)
{
  void *arg;
  if(argptr(0, (char**)&arg, sizeof(void*)) < 0)
    return -1;

  pde_t *pgdir = myproc()->pgdir;
  pte_t *pte;
  pde_t *pde;
  pte_t *pgtab;

  pde = &pgdir[PDX(arg)];
  if(*pde & PTE_P){
    pgtab = (pte_t*)P2V(PTE_ADDR(*pde));
    pte = &pgtab[PTX(arg)];
    if(*pte & PTE_P) {
      return PTE_ADDR(*pte) | ((uint)arg & 0xFFF);
    }
  }

  return -2;
}

// translate physical to virtual address
// Return: Pointer to physical address
// Errors: -1 for input argument error
//         -2 for address not in page tables
int
sys_rtranslate(void)
{
  void ** arg;
  if(argptr(0, (char**)&arg, sizeof(void*)) < 0)
    return -1;

  pde_t *pgdir = myproc()->pgdir;
  pte_t *pte;
  pde_t *pde;
  pte_t *pgtab;
  
  // walk entries until found
  for (uint i = 0; i < NPDENTRIES; i++){
    pde = &pgdir[i];
    if(*pde & PTE_P){
      pgtab = (pte_t*)P2V(PTE_ADDR(*pde));
      for (uint j = 0; j < NPTENTRIES; j++){
        pte =  &pgtab[j];
        if(*pte & PTE_P){
          // return (uint)*arg;
          if (PTE_ADDR(*pte) == PTE_ADDR(*arg)){
            return PGADDR(i, j, ((uint)*arg & 0xFFF));
          }
        }
      }
    }
  }

  return -2;
}

// Allow process to change its priority. Input is added to priority. Values are clamped to valid range.
// Returns new priority (-20 to 19). -21 is input error
int
sys_nice(void)
{
  int arg;
  if(argptr(0, (char**)&arg, sizeof(void*)) < 0)
    return -21;

  myproc()->priority += arg;
  // Clamp value to valid ranges
  if(myproc()->priority > 19)
    myproc()->priority = 19;
  else if(myproc()->priority < -20)
    myproc()->priority = -20;

  return myproc()->priority;
}

int sys_yield(void)
{
  yield();
  return 0;
}