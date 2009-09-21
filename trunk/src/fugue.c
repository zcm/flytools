/*********************************************************
 *
 * Filename     : fugue.c
 *
 * Originator   : Charanjit S. Jutla
 *
 * Purpose      : Optimized Implementation of Fugue for
 *                224 and 256 bit hash sizes
 *
 *********************************************************/

#include <string.h>
#include <time.h>
#include "fugue.h"
#ifndef _AESTAB_H
#define _AESTAB_H

#ifndef _AESTAB_T_H
#define _AESTAB_T_H

#include <stdio.h>
#include <limits.h>

#if defined(BYTE_ORDER) && defined(BIG_ENDIAN) && BYTE_ORDER == BIG_ENDIAN
#define HO2BE_8(_x)  (_x)
#define HO2BE_4(_x)  (_x)
#else
#define HO2BE_8(_x)  ((_x<<56)|((_x<<40)&0xff000000000000ull)|((_x<<24)&0xff0000000000ull)|((_x<<8)&0xff00000000ull)|\
                     ((_x>>8)&0xff000000ull)|((_x>>24)&0xff0000ull)|((_x>>40)&0xff00ull)|(_x>>56))
#define HO2BE_4(_x)  ((_x<<24)|((_x<<8)&0xff0000)|((_x>>8)&0xff00)|(_x>>24))
#endif

#endif

#define brot(x,n)   (((crypto_uint32)(x) <<  n) | ((crypto_uint32)(x) >> ((32 - n) & 31)))
#define to_byte(x) ((x) & 0xff)
#define to_nib(x)  ((x) & 0xfc)
#define bval(x,n)   to_byte((x) >> (8 * (n)))
#define bval4(x,n)  (n==0 ? ((x<<2) & 0x3fc): n==1? ((x>>6) & 0x3fc) : n==2 ? ((x>>14)& 0x3fc) : ((x>>22)&0x3fc))
#define nibval(x,n)     to_nib((x) >> (8 * (n)))
#define bytes2word(b0, b1, b2, b3)  \
        (((crypto_uint32)(b3) << 24) | ((crypto_uint32)(b2) << 16) | ((crypto_uint32)(b1) << 8) | (b0))

//#define SIXTEEN_TABLES

#define sbox(w) \
    w(0x63), w(0x7c), w(0x77), w(0x7b), w(0xf2), w(0x6b), w(0x6f), w(0xc5),\
    w(0x30), w(0x01), w(0x67), w(0x2b), w(0xfe), w(0xd7), w(0xab), w(0x76),\
    w(0xca), w(0x82), w(0xc9), w(0x7d), w(0xfa), w(0x59), w(0x47), w(0xf0),\
    w(0xad), w(0xd4), w(0xa2), w(0xaf), w(0x9c), w(0xa4), w(0x72), w(0xc0),\
    w(0xb7), w(0xfd), w(0x93), w(0x26), w(0x36), w(0x3f), w(0xf7), w(0xcc),\
    w(0x34), w(0xa5), w(0xe5), w(0xf1), w(0x71), w(0xd8), w(0x31), w(0x15),\
    w(0x04), w(0xc7), w(0x23), w(0xc3), w(0x18), w(0x96), w(0x05), w(0x9a),\
    w(0x07), w(0x12), w(0x80), w(0xe2), w(0xeb), w(0x27), w(0xb2), w(0x75),\
    w(0x09), w(0x83), w(0x2c), w(0x1a), w(0x1b), w(0x6e), w(0x5a), w(0xa0),\
    w(0x52), w(0x3b), w(0xd6), w(0xb3), w(0x29), w(0xe3), w(0x2f), w(0x84),\
    w(0x53), w(0xd1), w(0x00), w(0xed), w(0x20), w(0xfc), w(0xb1), w(0x5b),\
    w(0x6a), w(0xcb), w(0xbe), w(0x39), w(0x4a), w(0x4c), w(0x58), w(0xcf),\
    w(0xd0), w(0xef), w(0xaa), w(0xfb), w(0x43), w(0x4d), w(0x33), w(0x85),\
    w(0x45), w(0xf9), w(0x02), w(0x7f), w(0x50), w(0x3c), w(0x9f), w(0xa8),\
    w(0x51), w(0xa3), w(0x40), w(0x8f), w(0x92), w(0x9d), w(0x38), w(0xf5),\
    w(0xbc), w(0xb6), w(0xda), w(0x21), w(0x10), w(0xff), w(0xf3), w(0xd2),\
    w(0xcd), w(0x0c), w(0x13), w(0xec), w(0x5f), w(0x97), w(0x44), w(0x17),\
    w(0xc4), w(0xa7), w(0x7e), w(0x3d), w(0x64), w(0x5d), w(0x19), w(0x73),\
    w(0x60), w(0x81), w(0x4f), w(0xdc), w(0x22), w(0x2a), w(0x90), w(0x88),\
    w(0x46), w(0xee), w(0xb8), w(0x14), w(0xde), w(0x5e), w(0x0b), w(0xdb),\
    w(0xe0), w(0x32), w(0x3a), w(0x0a), w(0x49), w(0x06), w(0x24), w(0x5c),\
    w(0xc2), w(0xd3), w(0xac), w(0x62), w(0x91), w(0x95), w(0xe4), w(0x79),\
    w(0xe7), w(0xc8), w(0x37), w(0x6d), w(0x8d), w(0xd5), w(0x4e), w(0xa9),\
    w(0x6c), w(0x56), w(0xf4), w(0xea), w(0x65), w(0x7a), w(0xae), w(0x08),\
    w(0xba), w(0x78), w(0x25), w(0x2e), w(0x1c), w(0xa6), w(0xb4), w(0xc6),\
    w(0xe8), w(0xdd), w(0x74), w(0x1f), w(0x4b), w(0xbd), w(0x8b), w(0x8a),\
    w(0x70), w(0x3e), w(0xb5), w(0x66), w(0x48), w(0x03), w(0xf6), w(0x0e),\
    w(0x61), w(0x35), w(0x57), w(0xb9), w(0x86), w(0xc1), w(0x1d), w(0x9e),\
    w(0xe1), w(0xf8), w(0x98), w(0x11), w(0x69), w(0xd9), w(0x8e), w(0x94),\
    w(0x9b), w(0x1e), w(0x87), w(0xe9), w(0xce), w(0x55), w(0x28), w(0xdf),\
    w(0x8c), w(0xa1), w(0x89), w(0x0d), w(0xbf), w(0xe6), w(0x42), w(0x68),\
    w(0x41), w(0x99), w(0x2d), w(0x0f), w(0xb0), w(0x54), w(0xbb), w(0x16)

#define h0(x)   (x)

//this is the new mix for the hash funciton , replacing AES 2113 by new 1174 -CSJ
#define u0(p)   bytes2word(p, p, f7(p), f4(p))
#define u1(p)   bytes2word(f4(p),  p, p, f7(p))
#define u2(p)   bytes2word(f7(p), f4(p), p, p)
#define u3(p)   bytes2word(p, f7(p), f4(p), p)

#define WPOLY   0x011b

#define f2(x)   ((x<<1) ^ (((x>>7) & 1) * WPOLY))
#define f4(x)   ((x<<2) ^ (((x>>6) & 1) * WPOLY) ^ (((x>>6) & 2) * WPOLY))
#define f8(x)   ((x<<3) ^ (((x>>5) & 1) * WPOLY) ^ (((x>>5) & 2) * WPOLY) \
                        ^ (((x>>5) & 4) * WPOLY))
#define f3(x)   (f2(x) ^ x)
#define f7(x)   (f4(x)^ f2(x) ^x)   //this is new -CSJ

#define d_16(tab,b,e,f,g,h) static const crypto_uint32 tab[4][256*4] =\
{ {sbox(e), sbox(f),sbox(g),sbox(h)},\
  {sbox(f), sbox(g),sbox(h), sbox(e)},\
  {sbox(h), sbox(g),sbox(h), sbox(e)},\
  {sbox(e), sbox(f),sbox(g), sbox(h)}}


#if defined(SIXTEEN_TABLES)
d_16(aes_style_table, u0, u1, u2, u3);
#else
static const crypto_uint32 aes_style_table[4][256] = { {sbox(u0)}, {sbox(u1)}, {sbox(u2)}, {sbox(u3)} };
#endif

#endif




// set the IMPL_TYPE to _ASM for ASM_INLINE
// eet it to _OPT for C OPTIMIZED
//same rules hold for EXTRA_ROUND_IMPL_TYPE, though they cant be set to _ASM...yet !
// ASS_INLINE will be used in some macro names as a suiffix _ASM
// and   C OPTIMIZED as _OPT

/* Uncomment the desired default configuration */
#if !defined(CONFIG_ASM_INLINE) && !defined(CONFIG_C_OPTIMIZED)
//#define CONFIG_ASM_INLINE
#define CONFIG_C_OPTIMIZED
#endif

#ifdef CONFIG_ASM_INLINE
#define IMPL_TYPE  _ASM
#endif

#ifdef CONFIG_C_OPTIMIZED
#define IMPL_TYPE  _OPT
#endif

#define EXTRA_ROUND_IMPL_TYPE  _OPT

#define FUGUE_SIZE 30
#define FUGUE_FAR 24


#define column(i) col[i]
#define row(i) row[i]


#define vector2words(rc,i) \
bytes2word(bval(rc[0],i),  bval(rc[1],i),   bval(rc[2],i),   bval(rc[3],i))

#define off(p,n) ((n+p+FUGUE_SIZE) % FUGUE_SIZE)

#define tix(p,j, input_array)							\
  state[off(p,10)] ^= state[off(p,0)];  state[off(p,0)] = ((crypto_uint32 *)input_array)[j];  \
  state[off(p,8)]  ^= state[off(p,0)];  state[off(p,1)] ^= state[off(p,FUGUE_FAR)]

#define column_mix(p)\
  state[off(p,0)] ^= state[off(p,4)];  state[off(p,1)] ^= state[off(p,5)];  state[off(p,2)] ^= state[off(p,6)];\
  state[off(p,15)] ^= state[off(p,4)];  state[off(p,16)] ^= state[off(p,5)];  state[off(p,17)] ^= state[off(p,6)]

#define print_rc_ASM
/*printf("In type 0\n");
 printf("col: %x %x %x %x \n", b0,b1,b2,b3);*/

#define print_rc_OPT
/*printf("In type 2\n");
printf("col: %x %x %x %x \n", column(0),column(1),column(2),column(3));\
printf("row: %x %x %x %x \n", row(0), row(1), row(2), row(3));*/

#define shift_edi0
#define shift_edi1 "shrl $8,%%edi\n\t"
#define shift_edi2 "shrl $16,%%edi\n\t"
#define shift_edi3 "shrl $24,%%edi\n\t"

#define asm_bval4(y,c,t)\
        asm ("movl %1, %0\n\t"\
             shift_edi##c \
             "andl $255, %%edi"\
             :"=D" (t)       \
             :"m" (y)   \
             :"memory"       \
             );

#define asm_set_template(c,t) \
        asm ("movl %0, %%esi\n\t"\
             :\
             :"m" (aes_style_table[c][t])\
             :"%esi"\
             );
#define asm_xor_template(c,t) \
        asm ("xorl %0, %%esi\n\t"\
             :\
             :"m" (aes_style_table[c][t])\
             :"%esi"\
             );

#define asm_xor_xor_template(reg,c,t) \
        asm ("xorl %%" reg ", %%esi\n\t"\
             "xorl %0, %%" reg "\n\t"\
             "xorl %%" reg ",%%esi\n\t"\
             :\
             :"m" (aes_style_table[c][t])\
             :"%" reg, "%esi"\
             );

#define asm_set_set_template(reg,c,t) \
        asm ("movl %0, %%" reg "\n\t"\
             "movl %%" reg ",%%esi\n\t"\
             :\
             :"m" (aes_style_table[c][t])\
             :"%" reg, "%esi"\
             );

#define asm_set_xor_template(reg,c,t) \
        asm ("movl %0, %%" reg "\n\t"\
             "xorl %%" reg ",%%esi\n\t"\
             :\
             :"m" (aes_style_table[c][t])\
             :"%" reg, "%esi"\
             );

#define asm_xor_set_template(reg,c,t) \
        asm ("movl %0, %%esi \n\t"\
             "xorl %%esi, %%" reg "\n\t"\
             :\
             :"m" (aes_style_table[c][t])\
             :"%" reg, "%esi"\
             );
#define put_back_to_b_ASM(i) \
asm (        "movl %%esi,%0\n\t"\
             :"=m" (b##i)\
             :\
             :"memory", "%esi"\
             );

#define put_back_to_b_OPT(i)  print_rc_OPT;

/***if define (ASS_INLINE) *****/
#define table_index_ASM(p,i,k)  asm_bval4(state[off(p,i)],k,tmp)
#define asm_template0(xs_type,k,i,tmp)  xs_type("eax",k,tmp);
#define asm_template1(xs_type,k,i,tmp)  xs_type("ebx",k,tmp);
#define asm_template2(xs_type,k,i,tmp)  xs_type("ecx",k,tmp);
#define asm_template3(xs_type,k,i,tmp)  xs_type("edx",k,tmp);

#define xor_col_ASM(k,i) asm_xor_template(k,tmp);
#define set_col_ASM(k,i) asm_set_template(k,tmp);

#define xor_row_xor_col_ASM(k,i) asm_template##k(asm_xor_xor_template, k,i,tmp);
#define xor_row_set_col_ASM(k,i) asm_template##k(asm_xor_set_template, k,i,tmp);
#define set_row_xor_col_ASM(k,i) asm_template##k(asm_set_xor_template, k,i,tmp);
#define set_row_set_col_ASM(k,i) asm_template##k(asm_set_set_template, k,i,tmp);
/********ELSE*********/
#define table_index_OPT(p,i,k) tmp = aes_style_table[k][bval(state[off(p,i)],k)]
#define xor_col_OPT(k,i) column(i) ^= tmp;
#define set_col_OPT(k,i) column(i) = tmp;
#define xor_row_xor_col_OPT(k,i) row(k) ^= tmp; column(i) ^= tmp;
#define xor_row_set_col_OPT(k,i) row(k) ^= tmp; column(i) = tmp;
#define set_row_xor_col_OPT(k,i) row(k) = tmp;  column(i) ^= tmp;
#define set_row_set_col_OPT(k,i) row(k) = tmp;  column(i) = tmp;
/*******ENDIF *******/


#define xor_short_form_i_eq_k(p,i,k,type)       \
table_index##type(p,i,k); xor_col##type(k,i)

#define xor_short_form_i_eq_k_col_first(p,i,k,type)     \
table_index##type(p,i,k);  set_col##type(k,i)

#define xor_short_form_i_neq_k(p,i,k,type)  \
table_index##type(p,i,k);   xor_row_xor_col##type(k,i)

#define xor_short_form_i_neq_k_col_first(p,i,k,type)  \
table_index##type(p,i,k); xor_row_set_col##type(k,i)

#define xor_short_form_i_neq_k_row_first(p,i,k,type)  \
table_index##type(p,i,k);  set_row_xor_col##type(k,i)

#define xor_short_form_i_neq_k_row_and_col_first(p,i,k,type)  \
table_index##type(p,i,k);   set_row_set_col##type(k,i)

#define xor_rc_0_0(p,i,k,type) xor_short_form_i_eq_k_col_first(p,i,k,type)
#define xor_rc_0_1(p,i,k,type)  xor_short_form_i_neq_k_row_first(p,i,k,type)
#define xor_rc_0_2(p,i,k,type)  xor_short_form_i_neq_k_row_first(p,i,k,type)
#define xor_rc_0_3(p,i,k,type)  xor_short_form_i_neq_k_row_first(p,i,k,type)

#define xor_rc_1_0(p,i,k,type)  xor_short_form_i_neq_k_row_and_col_first(p,i,k,type)
#define xor_rc_1_1(p,i,k,type) xor_short_form_i_eq_k(p,i,k,type)
#define xor_rc_1_2(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)
#define xor_rc_1_3(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)

#define xor_rc_2_0(p,i,k,type)  xor_short_form_i_neq_k_col_first(p,i,k,type)
#define xor_rc_2_1(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)
#define xor_rc_2_2(p,i,k,type) xor_short_form_i_eq_k(p,i,k,type)
#define xor_rc_2_3(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)

#define xor_rc_3_0(p,i,k,type)  xor_short_form_i_neq_k_col_first(p,i,k,type)
#define xor_rc_3_1(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)
#define xor_rc_3_2(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)
#define xor_rc_3_3(p,i,k,type) xor_short_form_i_eq_k(p,i,k,type)

#define xor_rc_outer(p,i,type)\
   xor_rc_##i##_0(p,i,0,type);\
   xor_rc_##i##_1(p,i,1,type); xor_rc_##i##_2(p,i,2,type); xor_rc_##i##_3(p,i,3,type)

#define xor_row_and_col(p,type) \
     xor_rc_outer(p,0,type); put_back_to_b##type(0)\
     xor_rc_outer(p,1,type); put_back_to_b##type(1)\
     xor_rc_outer(p,2,type); put_back_to_b##type(2) \
     xor_rc_outer(p,3,type); put_back_to_b##type(3)




/******if defined (ASM_INLINE) *******/

#define init_row_and_col_ASM

#define load_eax(mask) "movl %%eax,%%esi\n\t"\
                 "xorl %1,%%esi\n\t"\
                 "andl $" mask ",%%esi\n\t"\
                 "movl %%esi, %%edi\n\t"
#define xor_reg(reg,c,mask) \
             "movl %%" reg ",%%esi\n\t"\
             "xorl %" c ",%%esi\n\t"\
             "andl $" mask ",%%esi\n\t"\
             "xorl %%esi, %%edi\n\t"

#define rotate_regs\
 asm("ror $8,%%eax\n\t"\
     "ror $8,%%ebx\n\t"\
     "ror $8,%%ecx\n\t"\
     "ror $8,%%edx\n\t"\
     :\
     :\
     :"%eax","%ebx","%ecx","%edx")

/* can improve this write_back_to_state listed even when p, i are variables
   and not constants. Write now, if these are variables, there is no index register
   to put them into. So, if they are variables. They can be computed, before hand
   into a temporary variable, and then loaded into %esi, and then
   the final movl edi, %0 can be movl edi, %0(%esi).
*/

#define write_back_to_state_listed(p,i,a,b,c,ddd) \
 asm(load_eax("255") \
     xor_reg("ebx","2","65280") \
     xor_reg("ecx","3","16711680")  \
     xor_reg("edx","4","4278190080")\
     "movl %%edi,%0"\
     :"=m" (state[off(p,i)])\
     :"m" (a), "m" (b), "m" (c), "m" (ddd)\
     :"%esi","%edi","%eax","%ebx","%ecx","%edx");

#define write_back_to_state_listed0(p,i) \
 write_back_to_state_listed(p,i, b0,b1,b2,b3); rotate_regs

#define write_back_to_state_listed1(p,i) \
 write_back_to_state_listed(p,i, b1,b2,b3,b0); rotate_regs

#define write_back_to_state_listed2(p,i) \
 write_back_to_state_listed(p,i, b2,b3,b0,b1); rotate_regs

#define write_back_to_state_listed3(p,i) \
 write_back_to_state_listed(p,i, b3,b0,b1,b2)

#define write_back_to_state_indexed_ASM(p,i) \
    write_back_to_state_listed##i(p,i)

/*************ELSE**********/

#define rotate_regs_OPT \
row(0)=brot(row(0),24);\
row(1)=brot(row(1),24);\
row(2)=brot(row(2),24);\
row(3)=brot(row(3),24);

#define write_back_to_state_listed_OPT(p,i,a,b,c,d) \
  state[off(p,i)]= ((row(0)^column(a)) & 0xff) ^   \
                   ((row(1)^column(b)) & 0xff00) ^ \
                   ((row(2)^column(c)) & 0xff0000) ^ \
                   ((row(3)^column(d)) & 0xff000000)


#define write_back_to_state_listed_OPT_0(p,i) \
 write_back_to_state_listed_OPT(p,i, 0,1,2,3); rotate_regs_OPT

#define write_back_to_state_listed_OPT_1(p,i) \
 write_back_to_state_listed_OPT(p,i, 1,2,3,0); rotate_regs_OPT

#define write_back_to_state_listed_OPT_2(p,i) \
 write_back_to_state_listed_OPT(p,i, 2,3,0,1); rotate_regs_OPT

#define write_back_to_state_listed_OPT_3(p,i) \
 write_back_to_state_listed_OPT(p,i, 3,0,1,2)

#define write_back_to_state_indexed_OPT(p,i) \
    write_back_to_state_listed_OPT_##i(p,i)

#define init_row_and_col_OPT    column(0)=column(1)=column(2)=column(3)=0; row(0)= row(1)= row(2)=row(3) =0;

#define xor_col_to_rows_ASM print_rc_ASM;
#define xor_col_to_rows_OPT print_rc_OPT;

/**************END************/

#define write_back_to_state(p,type) \
write_back_to_state_indexed##type(p,0);write_back_to_state_indexed##type(p,1);\
write_back_to_state_indexed##type(p,2);write_back_to_state_indexed##type(p,3);

#define super_mix(ptr,type)\
 init_row_and_col##type;  \
 xor_row_and_col(ptr,type); \
 xor_col_to_rows##type; \
 write_back_to_state(ptr,type);


#define sub_round(ptr,type) \
             column_mix(ptr);   super_mix(ptr,type)

#define super_mix_convert(p,type) super_mix(p,type)

#define ror(state,n) ptr = ((ptr-n+FUGUE_SIZE) % FUGUE_SIZE)

#define cross_mix(p,cross) \
       state[off(p,4)] ^=state[off(p,0)]; \
       state[off(p,cross)] ^=state[off(p,0)]

#define final_sub_round(p,cross,n) \
        cross_mix(p,cross);\
       ror(state,n); super_mix_convert(p,EXTRA_ROUND_IMPL_TYPE)

#define final_round2(ptr,num,c)\
    for (j=0; j<c; j++)      \
         { final_sub_round(ptr, 15,15); \
           final_sub_round(ptr, 16,14);} \
           cross_mix(ptr,15);

#define final_round1(ptr,num,c) \
    for (j=0; j<c; j++)  \
      {sub_round(((-3)*(2*((num+j)%5)+1)),EXTRA_ROUND_IMPL_TYPE); \
       sub_round(((-3)*(2*((num+j)%5)+2)),EXTRA_ROUND_IMPL_TYPE);}


#define round(j,num,in,type)						\
  tix(((-3)*2*num),j,in);						\
          empty_pretty_print(state, ptr, 30, "after tix");\
           sub_round(((-3)*(2*num+1)),type); \
            empty_update(ptr); empty_pretty_print(state, ptr, 30, "after subround 1");\
             sub_round(((-3)*(2*num+2)),type);\
              empty_update(ptr);empty_pretty_print(state, ptr, 30, "after subround 2");


#define dround(j,num,in,type)						\
  tix(((-3)*2*num),j,in);						\
          pretty_print(state, ptr, 30, "after tix");\
           sub_round(((-3)*(2*num+1)),type); \
            update(ptr); pretty_print(state, ptr, 30, "after subround 1");\
             sub_round(((-3)*(2*num+2)),type);\
              update(ptr);pretty_print(state, ptr, 30, "after subround 2");


//#define DEBUG 

#ifndef DEBUG
#define empty_pretty_print(s,a,b,c)
#define empty_update(ptr)
#else

#define empty_update(ptr) \
ptr = (ptr-3 + FUGUE_SIZE) %FUGUE_SIZE;


void empty_pretty_print (crypto_uint32 * state, int ptr, int n, char * st)
{
    int i,j;
    crypto_uint32 tmp;
    char next;

    printf("%s\n", st);
    for (i=0; i<n; i++)
    {
        tmp = state[off(ptr,i)];
        for (j=0; j<4; j++)
        {
            printf("%02x", (crypto_uint8) tmp & 0xff);
            tmp = tmp>>8;
        }
        printf(" ");
    }
    printf("\n\n");
    fflush(stdout);
    next = getchar();
}
#endif

#define update(ptr) \
ptr = (ptr-3 + FUGUE_SIZE) %FUGUE_SIZE;

void pretty_print (crypto_uint32 * state, int ptr, int n, char * st)
{
    int i,j;
    crypto_uint32 tmp;
    char next;

    printf("%s\n", st);
    for (i=0; i<n; i++)
    {
        tmp = state[off(ptr,i)];
        for (j=0; j<4; j++)
        {
            printf("%02x", (crypto_uint8) tmp & 0xff);
            tmp = tmp>>8;
        }
        printf(" ");
    }
    printf("\n\n");
    fflush(stdout);
    next = getchar();
}




#define append_fugue(k) \
    if (len_mod4)     \
     { round(0,k,filler_in, IMPL_TYPE); j++; \
       len_in = HO2BE_8 (inlen*8);   \
       len_in_bot = (len_in >> 32); len_in_top = len_in ; \
       round(0, ((k+1)%5), (crypto_uint32 *) &len_in_top, IMPL_TYPE); j++; \
       round(0, ((k+2)%5), (crypto_uint32 *) &len_in_bot, IMPL_TYPE); j++;} \
    else \
     { \
       len_in = HO2BE_8 (inlen*8);   \
       len_in_bot = (len_in >> 32) ; len_in_top = len_in ; \
       round(0, ((k)%5), (crypto_uint32 *) &len_in_top, IMPL_TYPE); j++; \
       round(0, ((k+1)%5), (crypto_uint32 *) &len_in_bot, IMPL_TYPE); j++;} 

int fugue_hash(
       unsigned char *out,
       const unsigned char *in,
       unsigned long long inlen
     )
{
crypto_uint32 state[30] = {0,0,0,0,0,0,0,0,
			   0,0,0,0,0,0,0,0,
			   0,0,0,0,0,0,
			   HO2BE_4(0xe952bdde),HO2BE_4(0x6671135f),HO2BE_4(0xe0d4f668),HO2BE_4(0xd2b0b594),
			   HO2BE_4(0xf96c621d),HO2BE_4(0xfbf929de),HO2BE_4(0x9149e899),HO2BE_4(0x34f8c248)};
				  
#ifdef CONFIG_ASM_INLINE
    crypto_uint32 b0,b1,b2,b3;
    crypto_uint32 row[4];
    crypto_uint32 col[4];
#else
    crypto_uint32 row[4];
    crypto_uint32 col[4];
#endif
    crypto_uint8  filler_in[4];
    crypto_uint32 tmp;
    short len_mod, len_mod4;
    crypto_uint64 len_div;
    crypto_uint64 j,j1,j2, len_in;
    crypto_uint32 len_in_top, len_in_bot;

    short ptr = 0;

    j1 = 0; 
    j=0;

    len_mod4 = inlen & 0x3;   
    len_div = (inlen >> 2);  //byte vs word
    len_mod = (len_div)%5;
    len_div = (len_div)/5;

    for (j2=0; j2<len_div;j2++)
    {
      round(j,0,in,IMPL_TYPE); j++;
      round(j,1,in,IMPL_TYPE); j++;
      round(j,2,in,IMPL_TYPE); j++;
      round(j,3,in,IMPL_TYPE); j++;
      round(j,4,in,IMPL_TYPE); j++;
    }

    if (len_mod >=1)
    {
      round(j,0,in,IMPL_TYPE); j++;

        if (len_mod >=2)
        {
	  round(j,1,in,IMPL_TYPE); j++;

            if (len_mod >=3)
            {
	      round(j,2,in,IMPL_TYPE); j++;

                if (len_mod >=4)
                {
		  round(j,3,in,IMPL_TYPE); j++;

                }
            }
        }
    }



    if (len_mod4) 
      {
	memcpy( filler_in, in+(j*4), len_mod4);
	memset( filler_in + len_mod4, 0 , 4 - len_mod4);
      }

    if (len_mod==0)	{append_fugue(0)}
else
    if (len_mod==1)	{append_fugue(1)}
else
    if (len_mod==2)	{append_fugue(2)}
else
    if (len_mod==3)	{append_fugue(3)}
else
    if (len_mod==4)	{append_fugue(4)};


    j1 = len_mod +2;
    if (len_mod4) j1++;
    j1 = j1 %5;
    ptr = FUGUE_SIZE - 6*j1;


    final_round1(ptr,j1,5);
    final_round2(ptr,j1,13);

    for (j=0; j<4; j++)
    {
        ((crypto_uint32 *)out)[j]   = state[off(ptr,1+j)];
        ((crypto_uint32 *)out)[4+j] = state[off(ptr,15+j)];
    }
    return 0;
}
