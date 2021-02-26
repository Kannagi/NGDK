#include "NGDK.h"
#include "asm.h"

#include "NG_arge.h"


void __attribute__((optimize("-O0"))) NG_BCD_Add(u32 *val,u32 add)
{
	NG_arg1_u32 = (u32)*val;
	NG_arg2_u32 = add;

	asm (
		"lea    NG_arg1_u32+4,%a0\n	"
		"lea    NG_arg2_u32+4,%a1\n	"

		"and	#0,%ccr\n	"
		"abcd	-(%a1),-(%a0)\n	"
		"abcd	-(%a1),-(%a0)\n	"
		"abcd	-(%a1),-(%a0)\n	"
		"abcd	-(%a1),-(%a0)\n	"

		"bcc    NG_BCD_Add_End\n	"
		"move.l #0x99999999,NG_arg1_u32\n	"
		"NG_BCD_Add_End:\n	"
		);

		(*val) = NG_arg1_u32;
}

void __attribute__((optimize("-O0"))) NG_BCD_Sub(u32 *val,u32 sub)
{
	NG_arg1_u32 = (u32)*val;
	NG_arg2_u32 = sub;

	asm (
		"lea    NG_arg1_u32+4,%a0\n	"
		"lea    NG_arg2_u32+4,%a1\n	"

		"and	#0,%ccr\n	"
		"sbcd	-(%a1),-(%a0)\n	"
		"sbcd	-(%a1),-(%a0)\n	"
		"sbcd	-(%a1),-(%a0)\n	"
		"sbcd	-(%a1),-(%a0)\n	"

		"bcc    NG_BCD_Sub_End\n	"
		"move.l #0,NG_arg1_u32\n	"
		"NG_BCD_Sub_End:\n	"
		);

		(*val) = NG_arg1_u32;
}
