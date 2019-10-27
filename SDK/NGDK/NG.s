#NO_APP
	.file	"NG.c"
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
	.globl	NG_Anim_Play_Fast
	.type	NG_Anim_Play_Fast, @function
NG_Anim_Play_Fast:
	move.l %d2,-(%sp)
	move.l 8(%sp),%a0
	move.l 12(%sp),%d1
	move.l 16(%sp),%d0
	move.l (%a0),%a1
	cmp.l 4(%a0),%a1
	jeq .L9
	clr.l 12(%a0)
	clr.l 8(%a0)
	move.l %a1,4(%a0)
	sub.l %a1,%a1
	moveq #1,%d2
.L3:
	and.l #65535,%d1
	cmp.l %d1,%d2
	jge .L4
	move.l %d2,12(%a0)
.L5:
	and.l #65535,%d0
	cmp.l %d0,%a1
	jlt .L1
	clr.l 8(%a0)
	clr.l 12(%a0)
.L1:
	move.l (%sp)+,%d2
	rts
.L4:
	clr.l 12(%a0)
	addq.l #1,%a1
	move.l %a1,8(%a0)
	jra .L5
.L9:
	move.l 12(%a0),%d2
	addq.l #1,%d2
	move.l 8(%a0),%a1
	jra .L3
	.size	NG_Anim_Play_Fast, .-NG_Anim_Play_Fast
	.align	2
	.globl	NG_Anim_Play
	.type	NG_Anim_Play, @function
NG_Anim_Play:
	move.l %d2,-(%sp)
	move.l 8(%sp),%a0
	move.l 12(%sp),%d1
	move.l 16(%sp),%d0
	move.l (%a0),%a1
	cmp.l 4(%a0),%a1
	jne .L17
	move.l 12(%a0),%d2
	addq.l #1,%d2
	move.l %d2,12(%a0)
	move.l 8(%a0),%a1
	and.l #65535,%d1
	cmp.l %d2,%d1
	jgt .L13
	clr.l 12(%a0)
	addq.l #1,%a1
	move.l %a1,8(%a0)
.L13:
	clr.l 16(%a0)
	and.l #65535,%d0
	cmp.l %d0,%a1
	jge .L18
	clr.w %d0
	move.l (%sp)+,%d2
	rts
.L18:
	clr.l 8(%a0)
	clr.l 12(%a0)
	moveq #1,%d0
	move.l %d0,16(%a0)
	move.l (%sp)+,%d2
	rts
.L17:
	clr.l 12(%a0)
	clr.l 8(%a0)
	clr.l 16(%a0)
	move.l %a1,4(%a0)
	moveq #2,%d0
	move.l (%sp)+,%d2
	rts
	.size	NG_Anim_Play, .-NG_Anim_Play
	.align	2
	.globl	NG_Anim_Init
	.type	NG_Anim_Init, @function
NG_Anim_Init:
	move.l 4(%sp),%a0
	clr.l 12(%a0)
	clr.l 8(%a0)
	clr.l 16(%a0)
	clr.l (%a0)
	clr.l 4(%a0)
	rts
	.size	NG_Anim_Init, .-NG_Anim_Init
	.align	2
	.globl	NG_PrintFreeMem
	.type	NG_PrintFreeMem, @function
NG_PrintFreeMem:
	subq.l #4,%sp
	move.l %d3,-(%sp)
	move.l %d2,-(%sp)
	move.l 16(%sp),%d2
	moveq #0,%d3
	move.w 22(%sp),%d3
	move.l %d3,-(%sp)
	move.w %d2,-(%sp)
	clr.w -(%sp)
	moveq #18,%d0
	add.l %sp,%d0
	move.l #addrssbss,%d1
	sub.w %d1,%d0
	move.w %d0,-(%sp)
	clr.w -(%sp)
	jsr NG_Printu16
	move.l %d3,-(%sp)
	addq.w #5,%d2
	move.w %d2,-(%sp)
	clr.w -(%sp)
	pea 868.w
	jsr NG_PrintFix
	lea (24,%sp),%sp
	move.l (%sp)+,%d2
	move.l (%sp)+,%d3
	addq.l #4,%sp
	rts
	.size	NG_PrintFreeMem, .-NG_PrintFreeMem
	.align	2
	.globl	NG_Cycle
	.type	NG_Cycle, @function
NG_Cycle:
	move.l %d3,-(%sp)
	move.l %d2,-(%sp)
	move.l 12(%sp),%d2
	move.w 1048578,%d1
	moveq #0,%d3
	move.w 18(%sp),%d3
	move.l %d3,-(%sp)
	move.w %d2,-(%sp)
	clr.w -(%sp)
	move.w %d1,%d0
	lsl.w #3,%d0
	sub.w %d1,%d0
	move.w #20025,%d1
	sub.w %d0,%d1
	move.w %d1,-(%sp)
	clr.w -(%sp)
	jsr NG_Printu16
	move.l %d3,-(%sp)
	addq.w #5,%d2
	move.w %d2,-(%sp)
	clr.w -(%sp)
	pea 816.w
	jsr NG_PrintFix
	lea (24,%sp),%sp
	move.l (%sp)+,%d2
	move.l (%sp)+,%d3
	rts
	.size	NG_Cycle, .-NG_Cycle
	.align	2
	.globl	NG_Joypad1
	.type	NG_Joypad1, @function
NG_Joypad1:
	move.l 4(%sp),%a0
	move.w 1048580,%d0
	lsr.w #8,%d0
	eor.w #255,%d0
	move.w 8(%a0),%d1
	btst #0,%d0
	jeq .L26
	cmp.w #1,%d1
	jeq .L60
	tst.w %d1
	jne .L28
	move.w #1,8(%a0)
.L28:
	move.w 14(%a0),%d1
	btst #1,%d0
	jeq .L30
.L70:
	cmp.w #1,%d1
	jeq .L61
	tst.w %d1
	jne .L32
	move.w #1,14(%a0)
.L32:
	move.w 12(%a0),%d1
	btst #2,%d0
	jeq .L34
.L72:
	cmp.w #1,%d1
	jeq .L62
	tst.w %d1
	jne .L36
	move.w #1,12(%a0)
.L36:
	move.w 10(%a0),%d1
	btst #3,%d0
	jeq .L38
.L74:
	cmp.w #1,%d1
	jeq .L63
	tst.w %d1
	jne .L40
	move.w #1,10(%a0)
.L40:
	move.w (%a0),%d1
	btst #4,%d0
	jeq .L42
.L76:
	cmp.w #1,%d1
	jeq .L64
	tst.w %d1
	jne .L44
	move.w #1,(%a0)
.L44:
	move.w 2(%a0),%d1
	btst #5,%d0
	jeq .L46
.L78:
	cmp.w #1,%d1
	jeq .L65
	tst.w %d1
	jne .L48
	move.w #1,2(%a0)
.L48:
	move.w 4(%a0),%d1
	btst #6,%d0
	jeq .L50
.L80:
	cmp.w #1,%d1
	jeq .L66
	tst.w %d1
	jne .L52
	move.w #1,4(%a0)
.L52:
	move.w 6(%a0),%d1
	tst.b %d0
	jge .L54
	cmp.w #1,%d1
	jeq .L67
	tst.w %d1
	jne .L25
	move.w #1,6(%a0)
.L25:
	rts
.L54:
	cmp.w #3,%d1
	jeq .L68
	cmp.w #2,%d1
	jne .L25
	move.w #3,6(%a0)
	rts
.L26:
	cmp.w #3,%d1
	jeq .L69
	cmp.w #2,%d1
	jne .L28
	move.w #3,8(%a0)
	move.w 14(%a0),%d1
	btst #1,%d0
	jne .L70
.L30:
	cmp.w #3,%d1
	jeq .L71
	cmp.w #2,%d1
	jne .L32
	move.w #3,14(%a0)
	move.w 12(%a0),%d1
	btst #2,%d0
	jne .L72
.L34:
	cmp.w #3,%d1
	jeq .L73
	cmp.w #2,%d1
	jne .L36
	move.w #3,12(%a0)
	move.w 10(%a0),%d1
	btst #3,%d0
	jne .L74
.L38:
	cmp.w #3,%d1
	jeq .L75
	cmp.w #2,%d1
	jne .L40
	move.w #3,10(%a0)
	move.w (%a0),%d1
	btst #4,%d0
	jne .L76
.L42:
	cmp.w #3,%d1
	jeq .L77
	cmp.w #2,%d1
	jne .L44
	move.w #3,(%a0)
	move.w 2(%a0),%d1
	btst #5,%d0
	jne .L78
.L46:
	cmp.w #3,%d1
	jeq .L79
	cmp.w #2,%d1
	jne .L48
	move.w #3,2(%a0)
	move.w 4(%a0),%d1
	btst #6,%d0
	jne .L80
.L50:
	cmp.w #3,%d1
	jeq .L81
	cmp.w #2,%d1
	jne .L52
	move.w #3,4(%a0)
	jra .L52
.L60:
	move.w #2,8(%a0)
	jra .L28
.L67:
	move.w #2,6(%a0)
	rts
.L66:
	move.w #2,4(%a0)
	jra .L52
.L65:
	move.w #2,2(%a0)
	jra .L48
.L64:
	move.w #2,(%a0)
	jra .L44
.L63:
	move.w #2,10(%a0)
	jra .L40
.L62:
	move.w #2,12(%a0)
	jra .L36
.L61:
	move.w #2,14(%a0)
	jra .L32
.L69:
	clr.w 8(%a0)
	jra .L28
.L68:
	clr.w 6(%a0)
	rts
.L71:
	clr.w 14(%a0)
	jra .L32
.L73:
	clr.w 12(%a0)
	jra .L36
.L79:
	clr.w 2(%a0)
	jra .L48
.L81:
	clr.w 4(%a0)
	jra .L52
.L75:
	clr.w 10(%a0)
	jra .L40
.L77:
	clr.w (%a0)
	jra .L44
	.size	NG_Joypad1, .-NG_Joypad1
	.align	2
	.globl	NG_Joypad2
	.type	NG_Joypad2, @function
NG_Joypad2:
	move.l %d2,-(%sp)
	move.l 8(%sp),%a0
	move.w 1048580,%d1
	not.w %d1
	move.w %d1,%d0
	and.w #255,%d0
	move.w 8(%a0),%d2
	btst #0,%d1
	jeq .L83
	cmp.w #1,%d2
	jeq .L117
	tst.w %d2
	jne .L85
	move.w #1,8(%a0)
.L85:
	move.w 14(%a0),%d2
	btst #1,%d1
	jeq .L87
.L127:
	cmp.w #1,%d2
	jeq .L118
	tst.w %d2
	jne .L89
	move.w #1,14(%a0)
.L89:
	move.w 12(%a0),%d2
	btst #2,%d1
	jeq .L91
.L129:
	cmp.w #1,%d2
	jeq .L119
	tst.w %d2
	jne .L93
	move.w #1,12(%a0)
.L93:
	move.w 10(%a0),%d2
	btst #3,%d1
	jeq .L95
.L131:
	cmp.w #1,%d2
	jeq .L120
	tst.w %d2
	jne .L97
	move.w #1,10(%a0)
.L97:
	move.w (%a0),%d2
	btst #4,%d1
	jeq .L99
.L133:
	cmp.w #1,%d2
	jeq .L121
	tst.w %d2
	jne .L101
	move.w #1,(%a0)
.L101:
	move.w 2(%a0),%d2
	btst #5,%d1
	jeq .L103
.L135:
	cmp.w #1,%d2
	jeq .L122
	tst.w %d2
	jne .L105
	move.w #1,2(%a0)
.L105:
	move.w 4(%a0),%d2
	btst #6,%d1
	jeq .L107
.L137:
	cmp.w #1,%d2
	jeq .L123
	tst.w %d2
	jne .L109
	move.w #1,4(%a0)
.L109:
	move.w 6(%a0),%d2
	tst.b %d1
	jge .L111
	cmp.w #1,%d2
	jeq .L124
	tst.w %d2
	jne .L82
	move.w #1,6(%a0)
.L82:
	move.l (%sp)+,%d2
	rts
.L111:
	cmp.w #3,%d2
	jeq .L125
	cmp.w #2,%d2
	jne .L82
	move.w #3,6(%a0)
	move.l (%sp)+,%d2
	rts
.L83:
	cmp.w #3,%d2
	jeq .L126
	cmp.w #2,%d2
	jne .L85
	move.w #3,8(%a0)
	move.w 14(%a0),%d2
	btst #1,%d1
	jne .L127
.L87:
	cmp.w #3,%d2
	jeq .L128
	cmp.w #2,%d2
	jne .L89
	move.w #3,14(%a0)
	move.w 12(%a0),%d2
	btst #2,%d1
	jne .L129
.L91:
	cmp.w #3,%d2
	jeq .L130
	cmp.w #2,%d2
	jne .L93
	move.w #3,12(%a0)
	move.w 10(%a0),%d2
	btst #3,%d1
	jne .L131
.L95:
	cmp.w #3,%d2
	jeq .L132
	cmp.w #2,%d2
	jne .L97
	move.w #3,10(%a0)
	move.w (%a0),%d2
	btst #4,%d1
	jne .L133
.L99:
	cmp.w #3,%d2
	jeq .L134
	cmp.w #2,%d2
	jne .L101
	move.w #3,(%a0)
	move.w 2(%a0),%d2
	btst #5,%d1
	jne .L135
.L103:
	cmp.w #3,%d2
	jeq .L136
	cmp.w #2,%d2
	jne .L105
	move.w #3,2(%a0)
	move.w 4(%a0),%d2
	btst #6,%d1
	jne .L137
.L107:
	cmp.w #3,%d2
	jeq .L138
	cmp.w #2,%d2
	jne .L109
	move.w #3,4(%a0)
	jra .L109
.L117:
	move.w #2,8(%a0)
	jra .L85
.L124:
	move.w #2,6(%a0)
	move.l (%sp)+,%d2
	rts
.L123:
	move.w #2,4(%a0)
	jra .L109
.L122:
	move.w #2,2(%a0)
	jra .L105
.L121:
	move.w #2,(%a0)
	jra .L101
.L120:
	move.w #2,10(%a0)
	jra .L97
.L119:
	move.w #2,12(%a0)
	jra .L93
.L118:
	move.w #2,14(%a0)
	jra .L89
.L126:
	clr.w 8(%a0)
	jra .L85
.L125:
	clr.w 6(%a0)
	move.l (%sp)+,%d2
	rts
.L128:
	clr.w 14(%a0)
	jra .L89
.L130:
	clr.w 12(%a0)
	jra .L93
.L136:
	clr.w 2(%a0)
	jra .L105
.L138:
	clr.w 4(%a0)
	jra .L109
.L132:
	clr.w 10(%a0)
	jra .L97
.L134:
	clr.w (%a0)
	jra .L101
	.size	NG_Joypad2, .-NG_Joypad2
	.align	2
	.globl	NG_ClearFix
	.type	NG_ClearFix, @function
NG_ClearFix:
#APP
| 151 "NG.c" 1
	movem.l %d0-%d2/%a0,-(%a7)
	move.w  #0x1,VRAM_MOD
	move.w  #0x00FF,%d1
	move.w  #FIXMAP+0x22,%d2
	lea VRAM_RW,%a0
	move.l  #37,%d0
	NG_Clear_Fix_loop:
	move.w  %d2,VRAM_ADDR
	move.w  %d1,(%a0)
	move.w  %d1,(%a0)
	move.w  %d1,(%a0)
	move.w  %d1,(%a0)
	move.w  %d1,(%a0)
	move.w  %d1,(%a0)
	move.w  %d1,(%a0)
	move.w  %d1,(%a0)
	move.w  %d1,(%a0)
	move.w  %d1,(%a0)
	move.w  %d1,(%a0)
	move.w  %d1,(%a0)
	move.w  %d1,(%a0)
	move.w  %d1,(%a0)
	move.w  %d1,(%a0)
	move.w  %d1,(%a0)
	move.w  %d1,(%a0)
	move.w  %d1,(%a0)
	move.w  %d1,(%a0)
	move.w  %d1,(%a0)
	move.w  %d1,(%a0)
	move.w  %d1,(%a0)
	move.w  %d1,(%a0)
	move.w  %d1,(%a0)
	move.w  %d1,(%a0)
	move.w  %d1,(%a0)
	move.w  %d1,(%a0)
	move.w  %d1,(%a0)
	add.w   #0x20,%d2
	dbra.w %d0,NG_Clear_Fix_loop
	movem.l (%a7)+,%d0-%d2/%a0
	
| 0 "" 2
#NO_APP
	rts
	.size	NG_ClearFix, .-NG_ClearFix
	.align	2
	.globl	NG_LoadPal
	.type	NG_LoadPal, @function
NG_LoadPal:
	move.l 4(%sp),NG_arg1_u32
	move.w 10(%sp),NG_arg1_u16
#APP
| 206 "NG.c" 1
	movem.l %d0/%a0-%a1,-(%a7)
	move.l NG_arg1_u32,%a0
	move.w	 NG_arg1_u16,%d0
	asl.w	#5,%d0
	lea     NG_PALETTE,%a1
	add.w	 %d0,%a1
	move.l  (%a0)+,(%a1)+
	move.l  (%a0)+,(%a1)+
	move.l  (%a0)+,(%a1)+
	move.l  (%a0)+,(%a1)+
	move.l  (%a0)+,(%a1)+
	move.l  (%a0)+,(%a1)+
	move.l  (%a0)+,(%a1)+
	move.l  (%a0)+,(%a1)+
	movem.l (%a7)+,%d0/%a0-%a1
	
| 0 "" 2
#NO_APP
	rts
	.size	NG_LoadPal, .-NG_LoadPal
	.align	2
	.globl	NG_LoadPalN
	.type	NG_LoadPalN, @function
NG_LoadPalN:
	move.l 4(%sp),NG_arg1_u32
	move.w 10(%sp),NG_arg1_u16
	move.w 14(%sp),NG_arg2_u16
#APP
| 236 "NG.c" 1
	movem.l %d0/%a0-%a1,-(%a7)
	move.l NG_arg1_u32,%a0
	move.w	 NG_arg1_u16,%d0
	asl.w	#5,%d0
	lea     NG_PALETTE,%a1
	add.w	 %d0,%a1
	move.w  NG_arg2_u16,%d0
	NG_Load_Pal_loop:
	move.l  (%a0)+,(%a1)+
	move.l  (%a0)+,(%a1)+
	move.l  (%a0)+,(%a1)+
	move.l  (%a0)+,(%a1)+
	move.l  (%a0)+,(%a1)+
	move.l  (%a0)+,(%a1)+
	move.l  (%a0)+,(%a1)+
	move.l  (%a0)+,(%a1)+
	dbra.w %d0,NG_Load_Pal_loop
	movem.l (%a7)+,%d0/%a0-%a1
	
| 0 "" 2
#NO_APP
	rts
	.size	NG_LoadPalN, .-NG_LoadPalN
	.align	2
	.globl	NG_BGColor
	.type	NG_BGColor, @function
NG_BGColor:
	move.w 6(%sp),NG_arg1_u16
#APP
| 267 "NG.c" 1
	movem.l %d0/%a0,-(%a7)
	lea     NG_PALETTE+0x1FFE,%a0
	move.w  NG_arg1_u16,(%a0)
	movem.l (%a7)+,%d0/%a0
	
| 0 "" 2
#NO_APP
	rts
	.size	NG_BGColor, .-NG_BGColor
	.comm	addrssbss,1,1
	.comm	NG_BG_W,2,2
	.comm	NG_PAL_ID,2,2
	.comm	NG_ID,2,2
	.comm	NG_Sprite,2048,2
	.comm	NG_arg4_u32,4,2
	.comm	NG_arg3_u32,4,2
	.comm	NG_arg2_u32,4,2
	.comm	NG_arg1_u32,4,2
	.comm	NG_arg4_u16,2,2
	.comm	NG_arg3_u16,2,2
	.comm	NG_arg2_u16,2,2
	.comm	NG_arg1_u16,2,2
	.comm	NG_arg4_u8,1,1
	.comm	NG_arg3_u8,1,1
	.comm	NG_arg2_u8,1,1
	.comm	NG_arg1_u8,1,1
	.ident	"GCC: (GNU) 8.1.1 20180626 (Red Hat Cross 8.1.1-3)"
	.section	.note.GNU-stack,"",@progbits
