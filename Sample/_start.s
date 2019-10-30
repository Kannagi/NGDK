#NO_APP
	.file	"start.s"
	.text
	.align	2

.equ ROM_ID,0x0052
.equ ROM_SIZE,0x80000*1

.equ RAMSTART,0x100000
.equ BIOS_SYSSTAT,0x10FD80
.equ REG_DIPSW,0x300001
.equ VRAM_ADDR,0x3C0000
.equ VRAM_RW,0x3C0002
.equ VRAM_MOD,0x3C0004
.equ REG_HBLANKCNT,0x3C0006
.equ REG_HBLANKPOS,0x3C0008
.equ REG_IRQACK,0x3C000C
.equ BIOSF_BOOTSCR,0xC00438

.equ REG_P1CNT,0x300000
.equ REG_P2CNT,0x340000
.equ BIOSF_CLEARFIX,0xC004C2
.equ BIOSF_CLEARSPR,0xC004C8

.org 0
.int 0x0010F300
.int 0x00C00402
.int 0x00C00408
.int 0x00C0040E

.int 0x00c00414
.int 0x00c00426
.int 0x00c00426
.int 0x00c00426

.int 0x00C0041A
.int 0x00C00420
.int 0x00c00426
.int 0x00c00426

.int 0x00C00426
.int 0x00C00426
.int 0x00C00426
.int 0x00C0042C

.int 0x00C00426
.int 0x00C00426
.int 0x00C00426
.int 0x00C00426

.int 0x00C00426
.int 0x00C00426
.int 0x00C00426
.int 0x00C00426

.int 0x00C00432

.org 0x0064
.int _VBLANK
.int _IRQ
.int _IRQ2
.int 0x00C00426
.int 0x00C00426
.int 0x00C00426
.int 0x00C00426

.org 0x0100
.string "NEO-GEO"
.short ROM_ID
.int ROM_SIZE
.int 0x10001E
.short 1
.short 0x0200

.int _JPConfig
.int _USConfig
.int _EUConfig

    jmp _start
    jmp _player
    jmp _demo
    jmp _coin

.org 0x0182
.int _CodeSNK

_CodeSNK:
.int 0x76004A6D,0x0A146600,0x003C206D,0x0A043E2D
.int 0x0A0813C0,0x00300001,0x32100C01,0x00FF671A
.int 0x30280002,0xB02D0ACE,0x66103028,0x0004B02D
.int 0x0ACF6606,0xB22D0AD0,0x67085088,0x51CFFFD4
.int 0x36074E75,0x206D0A04,0x3E2D0A08,0x3210E049
.int 0x0C0100FF,0x671A3010,0xB02D0ACE,0x66123028
.int 0x0002E048,0xB02D0ACF,0x6606B22D,0x0AD06708
.int 0x588851CF,0xFFD83607
.short 0x4e75
_JPConfig:
.ascii "0123456789ABCDEF"
.byte 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
.byte 0x24,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
.ascii "LIVES       "
.ascii "1           "
.ascii "2           "
.ascii "3           "
.ascii "4           "
.ascii "HOW TO PLAY "
.ascii "WITH        "
.ascii "WITHOUT     "

_USConfig:
.ascii "0123456789ABCDEF"
.byte 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
.byte 0x24,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
.ascii "LIVES       "
.ascii "1           "
.ascii "2           "
.ascii "3           "
.ascii "4           "
.ascii "HOW TO PLAY "
.ascii "WITH        "
.ascii "WITHOUT     "

_EUConfig:
.ascii "0123456789ABCDEF"
.byte 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
.byte 0x24,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
.ascii "LIVES       "
.ascii "1           "
.ascii "2           "
.ascii "3           "
.ascii "4           "
.ascii "HOW TO PLAY "
.ascii "WITH        "
.ascii "WITHOUT     "

.org 0x0380
_player:
_demo:
_coin:

    rts

_IRQ2:
	nop
	rte
_IRQ:
	nop
	rte

_VBLANK:

    btst    #7,BIOS_SYSSTAT
    bne label_BIOS_SYSSTAT
		jmp     BIOSF_BOOTSCR
		rte
label_BIOS_SYSSTAT:


    move.w  #4,REG_IRQACK
    move.b  #0,REG_DIPSW

    movem.l %d0,-(%a7)

    addq.b #1,RAMSTART

    move.b  REG_P1CNT,%d0
    move.b  %d0,RAMSTART+4

    move.b  REG_P2CNT,%d0
    move.b  %d0,RAMSTART+5

    movem.l (%a7)+,%d0
_VBLANK_END:
    rte

	.align	2
	.globl	_start
	.type	_start, @function

_start:
	lea     0x10F300,%sp
    move.b  #0,REG_DIPSW
    move.w  #4,REG_IRQACK
    move.w  #0x2000,%sr

    clr.l RAMSTART

    jsr BIOSF_CLEARFIX
    jsr BIOSF_CLEARSPR

    jsr main

_start_end:
	nop
    bra _start

	.size	_start, .-_start


	.align	2
	.globl	NG_WaitVBlank
	.type	NG_WaitVBlank, @function


NG_WaitVBlank:
	clr.l RAMSTART
	move.w REG_HBLANKCNT,%d0
	lsr.w #7,%d0
	subi.w #0xF8,%d0
	mulu.w #97,%d0
	asr.w #8,%d0
	move.w %d0,RAMSTART+10


NG_wait:

	addq.w #1,RAMSTART+2
	cmpi.b #0,RAMSTART
    beq NG_wait

    rts

    .size	NG_WaitVBlank, .-NG_WaitVBlank


	.ident	"GCC: (GNU) 8.1.1 20180626 (Red Hat Cross 8.1.1-3)"
	.section	.note.GNU-stack,"",@progbits
