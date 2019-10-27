#NO_APP
	.file	"NG_BG.c"
	.text
#APP
	.equ SCB1,0x0000
	.equ FIXMAP,0x7000
	.equ SCB2,0x8000
	.equ SCB3,0x8200
	.equ SCB4,0x8400
	.equ NG_RAM,0x100000
	.equ NG_PALETTE,0x400000
	.equ VRAM_ADDR,0x3C0000
	.equ VRAM_RW,0x3C0002
	.equ VRAM_MOD,0x3C0004
	.equ FIX_ASCII,0x300
	
	.macro digit_convert id
	cmp.w %d4,%d2
	bmi.w NG_Print_u16_if\id
	sub.w %d4,%d2
	addq.w	#1,%d0
	NG_Print_u16_if\id:
	.endm
	.macro digit_convertA id
	cmp.w %d4,%d2
	bmi.w NG_Print_u16_ifA\id
	sub.w %d4,%d2
	addq.w	#1,%d0
	NG_Print_u16_ifA\id:
	.endm
	.macro digit_convertB id
	cmp.w %d4,%d2
	bmi.w NG_Print_u16_ifB\id
	sub.w %d4,%d2
	addq.w	#1,%d0
	NG_Print_u16_ifB\id:
	.endm
	.macro digit_convertC id
	cmp.w %d4,%d2
	bmi.w NG_Print_u16_ifC\id
	sub.w %d4,%d2
	addq.w	#1,%d0
	NG_Print_u16_ifC\id:
	.endm
	.macro digit_convertD id
	cmp.w %d4,%d2
	bmi.w NG_Print_u16_ifD\id
	sub.w %d4,%d2
	addq.w	#1,%d0
	NG_Print_u16_ifD\id:
	.endm
	
	.macro digit_convert5 v,id
	cmp.w #\v,%d2
	bmi NG_Print_if\id
	sub.w #\v,%d2
	addq.w	#5,%d0
	NG_Print_if\id:
	.endm
	
	.macro digit_convert1000 id
	move.w %d3,%d0
	move.w #1000,%d4
	digit_convert5 5000,\id
	digit_convertA \id
	digit_convertB \id
	digit_convertC \id
	digit_convertD \id
	move.w  %d0,(%a0)
	.endm
	
	.macro digit_convert100 id
	move.w %d3,%d0
	move.w #100,%d4
	digit_convert5 500,\id
	digit_convertA \id
	digit_convertB \id
	digit_convertC \id
	digit_convertD \id
	move.w  %d0,(%a0)
	.endm
	
	.macro digit_convert10 id
	move.w %d3,%d0
	move.w #10,%d4
	digit_convert5 50,\id
	digit_convertA \id
	digit_convertB \id
	digit_convertC \id
	digit_convertD \id
	move.w  %d0,(%a0)
	.endm
	
	.macro hexa_convert id
	andi.w	#0x0F,%d1
	cmpi.w	#0x0A,%d1
	bmi.w	NG_PrinthA_\id
	add.w	%d4,%d1
	bra.w	NG_PrinthB_\id
	NG_PrinthA_\id:
	add.w	%d3,%d1
	NG_PrinthB_\id:
	move.w %d1,(%a0)
	.endm
	
#NO_APP
	.align	2
	.globl	NG_Map_Decompress
	.type	NG_Map_Decompress, @function
NG_Map_Decompress:
	move.l 4(%sp),NG_arg1_u32
	move.l 8(%sp),NG_arg2_u32
	move.w 14(%sp),NG_arg1_u16
#APP
| 13 "NG_BG.c" 1
	movem.l %d2-%d4,-(%a7)
	move.l NG_arg1_u32,%a0
	move.l NG_arg2_u32,%a1
	move.w	NG_arg1_u16,%d0
	NG_Map_Decompress_loop:
	move.w	(%a0)+,%d2
	move.w	(%a0)+,%d3
	move.w	%d3,%d4
	andi.w  #0xFF3F,%d3
	asr.w  #6,%d4
	andi.w  #0x3,%d4
	move.w	%d2,(%a1)+
	move.w	%d3,(%a1)+
	cmpi.w  #1,%d4
	bne.w NG_Map_Decompress_cmp1
	move.w	(%a0)+,%d1
	sub.w	%d1,%d0
	subq.w	#1,%d1
	NG_Map_Decompress_loop1:
	move.w	%d2,(%a1)+
	move.w	%d3,(%a1)+
	dbra.w %d1,NG_Map_Decompress_loop1
	NG_Map_Decompress_cmp1:
	cmpi.w  #2,%d4
	bne.w NG_Map_Decompress_cmp2
	move.w	(%a0)+,%d1
	sub.w	%d1,%d0
	subq.w	#1,%d1
	NG_Map_Decompress_loop2:
	addq.w  #1,%d2
	move.w	%d2,(%a1)+
	move.w	%d3,(%a1)+
	dbra.w %d1,NG_Map_Decompress_loop2
	NG_Map_Decompress_cmp2:
	dbra.w %d0,NG_Map_Decompress_loop
	move.w	%d1,NG_arg1_u16
	movem.l (%a7)+,%d2-%d4
	
| 0 "" 2
#NO_APP
	rts
	.size	NG_Map_Decompress, .-NG_Map_Decompress
	.align	2
	.globl	NG_BGWidthSet
	.type	NG_BGWidthSet, @function
NG_BGWidthSet:
	move.w 6(%sp),%d0
	add.w %d0,%d0
	add.w %d0,%d0
	move.w %d0,NG_BG_W
	rts
	.size	NG_BGWidthSet, .-NG_BGWidthSet
	.align	2
	.globl	NG_BGWidthGet
	.type	NG_BGWidthGet, @function
NG_BGWidthGet:
	move.w NG_BG_W,%d0
	lsr.w #2,%d0
	rts
	.size	NG_BGWidthGet, .-NG_BGWidthGet
	.align	2
	.globl	NG_SpriteUpdateMapHorizontal
	.type	NG_SpriteUpdateMapHorizontal, @function
NG_SpriteUpdateMapHorizontal:
	move.w 10(%sp),NG_arg1_u16
	move.l 4(%sp),NG_arg1_u32
	move.w 14(%sp),NG_arg2_u16
#APP
| 101 "NG_BG.c" 1
	movem.l %d3/%a2,-(%a7)
	move.w  #0x01,VRAM_MOD
	lea     NG_Sprite,%a0
	lea     VRAM_RW,%a1
	move.l NG_arg1_u32,%a2
	move.w  NG_arg1_u16,%d0
	andi.w  #0x7F,%d0
	asl.w   #4,%d0
	add.w   %d0,%a0
	move.w  10(%a0),%d1
	subi.w  #1,%d1
	move.w  (%a0),%d0
	asl.w   #6,%d0
	lea     VRAM_ADDR,%a0
	move.w  #0x40,%d2
	move.w   NG_arg2_u16,%d3
	asl.w   #1,%d3
	add.w   %d3,%d0
	NG_SpriteUpdateMapHorizontal_loop:
	move.w  %d0,(%a0)
	move.w  (%a2)+,(%a1)
	move.w  (%a2)+,(%a1)
	add.w   %d2,%d0
	dbra.w %d1,NG_SpriteUpdateMapHorizontal_loop
	movem.l (%a7)+,%d3/%a2
	
| 0 "" 2
#NO_APP
	rts
	.size	NG_SpriteUpdateMapHorizontal, .-NG_SpriteUpdateMapHorizontal
	.align	2
	.globl	NG_SpriteUpdateMapVertical
	.type	NG_SpriteUpdateMapVertical, @function
NG_SpriteUpdateMapVertical:
	move.w 10(%sp),NG_arg1_u16
	move.w 14(%sp),NG_arg2_u16
	move.l 4(%sp),NG_arg1_u32
#APP
| 147 "NG_BG.c" 1
	movem.l %a2,-(%a7)
	move.w  #0x01,VRAM_MOD
	lea     NG_Sprite,%a0
	lea     VRAM_RW,%a1
	move.l  NG_arg1_u32,%a2
	move.w  NG_arg1_u16,%d0
	andi.w  #0x7F,%d0
	asl.w   #4,%d0
	add.w   %d0,%a0
	move.w  (%a0),%d0
	add.w   NG_arg2_u16,%d0
	asl.w   #6,%d0
	move.w  %d0,VRAM_ADDR
	move.w  6(%a0),%d1
	subi.w  #1,%d1
	move.w  NG_BG_W,%d0
	NG_SpriteUpdateMapVertical_loop:
	move.w  (%a2),(%a1)
	move.w  2(%a2),(%a1)
	add.w   %d0,%a2
	dbra.w %d1,NG_SpriteUpdateMapVertical_loop
	movem.l (%a7)+,%a2
	
| 0 "" 2
#NO_APP
	rts
	.size	NG_SpriteUpdateMapVertical, .-NG_SpriteUpdateMapVertical
	.align	2
	.globl	NG_BGScrollx
	.type	NG_BGScrollx, @function
NG_BGScrollx:
	move.l %a2,-(%sp)
	move.l %d2,-(%sp)
	move.l 12(%sp),%a1
	move.l 16(%sp),%a0
	move.w %a0,%d0
	moveq #0,%d2
	move.w 22(%sp),%d2
	move.l %d2,%d1
	lsl.l #4,%d1
	add.l #NG_Sprite+8,%d1
	move.l %d1,%a2
	move.w (%a2),%d1
	lsr.w #4,%d1
	jeq .L12
	add.w #4096,%d0
	sub.w %d1,%d0
.L12:
	moveq #31,%d1
	and.l %d0,%d1
	move.l %d1,20(%sp)
	move.l %d2,16(%sp)
	add.w %d0,%d0
	add.w %d0,%d0
	and.l #65535,%d0
	add.l %a1,%d0
	move.l %d0,12(%sp)
	move.l (%sp)+,%d2
	move.l (%sp)+,%a2
	jra NG_SpriteUpdateMapVertical
	.size	NG_BGScrollx, .-NG_BGScrollx
	.ident	"GCC: (GNU) 8.1.1 20180626 (Red Hat Cross 8.1.1-3)"
	.section	.note.GNU-stack,"",@progbits
