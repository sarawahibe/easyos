#ifndef easyos_MACHINE_H
#define easyos_MACHINE_H

#define EnableSetFunction	1

/* Information generated by machine.c */
#define easyos_STACK_GROWS_DOWNWARDS 1 /* Stack direction */
#define easyos_BIG_ENDIAN 	   1 /* Big or little endian */
#define easyos_SIZEOFULONG	   4 /* Size of a ULONG */
#define easyos_WORDALIGN		   4 /* Alignment for WORD */
#define easyos_LONGALIGN		   4 /* Alignment for LONG */
#define easyos_PTRALIGN		   4 /* Alignment for PTR */
#define easyos_IPTRALIGN		   4 /* Alignment for IPTR */
#define easyos_DOUBLEALIGN	   4 /* Alignment for double */
#define easyos_WORSTALIGN 	   8 /* Worst case alignment */

#define SIZEOF_ALL_REGISTERS	   (20*4)

#define easyos_GET_SYSBASE        struct ExecBase * SysBase = *(struct ExecBase **)0x4;
#define easyos_GET_DOSBASE        struct DosLibrary * DOSBase = (struct DosLibrary *)OpenLibrary((UBYTE *)"dos.library",0); \
                                CloseLibrary(DOSBase);
#define easyos_GET_SYSBASE_OK     struct ExecBase * SysBase = *(struct ExecBase **)0x4;

/* do we need a function attribute to get parameters on the stack? */
#define __stackparm

/* ??? */
#define SP_OFFSET 0

/*
    One entry in a libraries' jumptable. For assembler compatibility, the
    field jmp should contain the code for an absolute jmp to a 32bit
    address. There are also a couple of macros which you should use to
    access the vector table from C.
*/
struct FullJumpVec
{
    unsigned short jmp;       // not necessary - if this leaves, change machine.i offsets!
    unsigned char vec[4];
};

struct JumpVec
{
    unsigned char vec[4];
};

/* Internal macros */
#define __easyos_SET_VEC(v,a)             (*(ULONG*)(v)->vec=(ULONG)(a))
#define __easyos_GET_VEC(v)               ((APTR)(*(ULONG*)(v)->vec))


/* Use these to acces a vector table */
#define LIB_VECTSIZE			(sizeof (struct JumpVec))
#define __easyos_GETJUMPVEC(lib,n)        (&((struct JumpVec *)lib)[-n])
#define __easyos_GETVECADDR(lib,n)        (__easyos_GET_VEC(__easyos_GETJUMPVEC(lib,n)))
#define __easyos_SETVECADDR(lib,n,addr)   (__easyos_SET_VEC(__easyos_GETJUMPVEC(lib,n),(APTR)(addr)))
#define __easyos_INITVEC(lib,n)           __easyos_SETVECADDR(lib,n,_easyos_not_implemented)



/*
   Code to use to generate stub functions.
   It must be *printed* with a function like printf in a file
   to be compiled with gcc.
   - The first parameter is the function name,
   - The second parameter is the basename,
   - The third parameter is the library vector to be called.
     It's value must be computed by the stub generator with this code:
     &(__easyos_GETJUMPVEC(0, n+1)->vec), where n is the library vector position in
     the library vectors list.
*/

#define STUBCODE                                       \
		"#define EMITSTUB(fname, bname, vec) " \
		".globl fname ; "                      \
		"fname : "                             \
		"ldr  r0, .L1; "                       \
		"ldr  r0 , [r0, $ vec ]; "             \
		"mov  pc, r0;"                         \
		".L1: .word bname;\n"                  \
		"EMITSTUB(%s, %s, %d) "

/*
   We want to activate the execstubs and preserve all registers
   when calling obtainsemaphore, obtainsemaphoreshared, releasesemaphore,
   getcc, permit, forbid, enable, disable
*/
#undef UseExecstubs
//#define UseExecstubs 1

/* For debugging only: Pass errnos from the emulated OS. dos/Fault() will
   recognise them */
#undef PassThroughErrnos
#define PassThroughErrnos 0x40000000

/* Macros to test/set failure of AllocEntry() */
#define easyos_ALLOCENTRY_FAILED(memType) \
	((struct MemList *)((IPTR)(memType) | 0x80ul<<(sizeof(APTR)-1)*8))
#define easyos_CHECK_ALLOCENTRY(memList) \
	(!((IPTR)(memList) & 0x80ul<<(sizeof(APTR)-1)*8))

/*
    Find the next valid alignment for a structure if the next x bytes must
    be skipped.
*/
#define easyos_ALIGN(x)        (((x)+easyos_WORSTALIGN-1)&-easyos_WORSTALIGN)

/* Prototypes */
extern void _easyos_not_implemented ();
extern void easyos_not_implemented ();

/*
    How much stack do we need ? Lots :-) ?
    Not so much, I think (schulz) ;-))
*/

#define easyos_STACKSIZE	0x4000

/* How to map function arguments to CPU registers */
/*
  The ARM processor has enough registers to map the m68k
  registers set onto them - but right now I don't care.
  I just use the calling convention of the compiler.
*/

/* What to do with the library base in header, prototype and call */
#define __easyos_LH_BASE(basetype,basename)   basetype basename
#define __easyos_LP_BASE(basetype,basename)   void *
#define __easyos_LC_BASE(basetype,basename)   basename
#define __easyos_LD_BASE(basetype,basename)   basetype

/* How to transform an argument in header, opt prototype, call and forced
   prototype. */
#define __easyos_LHA(type,name,reg)     type name
#define __easyos_LPA(type,name,reg)     type
#define __easyos_LCA(type,name,reg)     name
#define __easyos_LDA(type,name,reg)     type
#define __easyos_UFHA(type,name,reg)    type name
#define __easyos_UFPA(type,name,reg)    type
#define __easyos_UFCA(type,name,reg)    name
#define __easyos_UFDA(type,name,reg)    type
#define __easyos_LHAQUAD(type,name,reg1,reg2)     type name
#define __easyos_LPAQUAD(type,name,reg1,reg2)     type
#define __easyos_LCAQUAD(type,name,reg1,reg2)     name

/* Prefix for library function in header, prototype and call */
#define __easyos_LH_PREFIX    /* eps */
#define __easyos_LP_PREFIX    /* eps */
#define __easyos_LC_PREFIX    /* eps */
#define __easyos_LD_PREFIX    /* eps */
#define __easyos_UFH_PREFIX   /* eps */
#define __easyos_UFP_PREFIX   /* eps */
#define __easyos_UFC_PREFIX   /* eps */
#define __easyos_UFD_PREFIX   /* eps */

/* if this is defined, all easyos_LP*-macros will expand to nothing. */
#define __easyos_USE_MACROS_FOR_LIBCALL

#define __UFC3R(t,n,t1,n1,r1,t2,n2,r2,t3,n3,r3,p) \
({\
    long _n1 = (long)(n1);\
    long _n2 = (long)(n2);\
    long _n3 = (long)(n3);\
    long _re;\
    __asm__ __volatile__(\
	"ldr	%%r0, %1\n\t"\
	"str	%%sp,[%%r0, $0]\n\t"\
	"mov	%%r2,%5\n\t"\
	"mov	%%r1,%4\n\t"\
	"mov	%%r0,%3\n\t"\
	"mov	%%lr,%%pc\n\t"\
	"mov	%%pc,%2\n\t"\
	"str	%%r0,%0"\
	: "=m"(_re), "=m"(*(int *)p)\
	: "r"(n), "r"(_n1), "r"(_n2), "r"(_n3)\
	: "cc", "memory", "%r0", "%r1", "%r2" );\
    (t)_re;\
})
#define easyos_UFC3R(t,n,a1,a2,a3,p,ss) __UFC3R(t,n,a1,a2,a3,p)

#endif /* easyos_MACHINE_H */