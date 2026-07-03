#ifndef _MT_USART_H
#define _MT_USART_H

#  ifdef  __cplusplus
extern "C" {
#  endif

#include "usart.h"

//-----------------------------------------------------------------
// Command codes
//-----------------------------------------------------------------
typedef enum _MU_CMD_CODE_
{
	MU_CMD_UNKNOWN = 0,
	MU_CMD_MOV,
	MU_CMD_PULSE,
	MU_CMD_EXIT,
} MU_CMD_CODE;

//-----------------------------------------------------------------
// Per-command argument structs
//-----------------------------------------------------------------
typedef struct { s32 current_ma; }              MU_MOV_ARGS;   // MOV <current_ma>
typedef struct { s32 current_ma1; s32 time_ms; s32 current_ma2; } MU_PULSE_ARGS; // PULSE <time_ms> <current_ma>
// EXIT takes no arguments, no struct needed

//-----------------------------------------------------------------
// Parsed command context. line[] is filled directly from the rx
// ring buffer (see MU_TryReadCmd), no intermediate copy.
//-----------------------------------------------------------------
typedef struct _MU_CTX_
{
	u8          line[USART_REC_LINE_LEN];
	MU_CMD_CODE code;
	u8          valid;   // 1 = keyword recognized AND arguments well-formed
	union
	{
		MU_MOV_ARGS   mov;
		MU_PULSE_ARGS pulse;
	} args;
} MU_CTX;

//-----------------------------------------------------------------
// Public API — this is the only function the application needs.
// It pulls the next pending line straight into ctx->line and
// parses it in place. Returns 1 if a line was consumed (check
// ctx->valid / ctx->code for the outcome), 0 if nothing was
// pending (ctx left untouched).
//-----------------------------------------------------------------
u8 MU_TryReadCmd(MU_CTX *ctx);


#  ifdef  __cplusplus
}
#  endif

# endif
