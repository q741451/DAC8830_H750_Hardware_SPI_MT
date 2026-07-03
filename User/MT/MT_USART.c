#include "MT_USART.h"
#include "usart.h"

#include <string.h>
#include <stdlib.h>

//-----------------------------------------------------------------
// Command keyword table (table-driven; add new commands here only,
// nothing below this table needs to change). Lengths are computed
// at compile time via sizeof(), no runtime strlen() calls anywhere
// in this file.
//-----------------------------------------------------------------
typedef struct
{
	const char  *str;
	u8           len;
	MU_CMD_CODE  code;
} MU_CMD_MAP;

#define MU_KW(s, c)  { s, sizeof(s) - 1, c }

static const MU_CMD_MAP mu_cmd_table[] =
{
	MU_KW("MOV",   MU_CMD_MOV),
	MU_KW("PULSE", MU_CMD_PULSE),
	MU_KW("EXIT",  MU_CMD_EXIT),
};
#define MU_CMD_TABLE_SIZE (sizeof(mu_cmd_table) / sizeof(mu_cmd_table[0]))

//-----------------------------------------------------------------
// static const char *mu_skip_spaces(const char *p)
//-----------------------------------------------------------------
static const char *mu_skip_spaces(const char *p)
{
	while(*p == ' ') p++;
	return p;
}

//-----------------------------------------------------------------
// static u8 mu_next_token(const char **pp, const char **tok, u8 *len)
//-----------------------------------------------------------------
// Non-destructive tokenizer. Finds the next space-delimited token
// starting at *pp WITHOUT modifying the underlying buffer (unlike
// strtok, which writes '\0' at each delimiter). Advances *pp past
// the token. Returns 0 if no token remains (end of line reached).
//-----------------------------------------------------------------
static u8 mu_next_token(const char **pp, const char **tok, u8 *len)
{
	const char *p = mu_skip_spaces(*pp);
	const char *start = p;
	
	while(*p != ' ' && *p != '\0') p++;
	if(p == start) return 0;
	
	*tok = start;
	*len = (u8)(p - start);
	*pp  = p;
	return 1;
}

//-----------------------------------------------------------------
// static u8 mu_next_int(const char **pp, s32 *out)
//-----------------------------------------------------------------
// Parses one decimal integer starting at *pp (leading spaces are
// skipped first). strtol's endptr naturally lands on the next
// space or '\0' for a clean number, so this single call both
// extracts the value AND validates the token boundary — no
// separate token-length bookkeeping is needed. Rejects a missing
// number (no digits at all) and garbage glued onto a number
// (e.g. "100abc", where endptr would land on 'a', neither a
// space nor '\0'). Advances *pp past the number on success.
//-----------------------------------------------------------------
static u8 mu_next_int(const char **pp, s32 *out)
{
	const char *p = mu_skip_spaces(*pp);
	char *endptr;
	long  val;
	
	if(*p == '\0') return 0;
	
	val = strtol(p, &endptr, 10);
	if(endptr == p) return 0;                       // no digits consumed
	if(*endptr != ' ' && *endptr != '\0') return 0;  // trailing garbage
	
	*out = (s32)val;
	*pp  = endptr;
	return 1;
}

//-----------------------------------------------------------------
// static MU_CMD_CODE mu_str2code(const char *tok, u8 len)
//-----------------------------------------------------------------
static MU_CMD_CODE mu_str2code(const char *tok, u8 len)
{
	u8 i;
	for(i = 0; i < MU_CMD_TABLE_SIZE; i++)
	{
		if(mu_cmd_table[i].len == len && strncmp(tok, mu_cmd_table[i].str, len) == 0)
			return mu_cmd_table[i].code;
	}
	return MU_CMD_UNKNOWN;
}

//-----------------------------------------------------------------
// static void mu_parse(MU_CTX *ctx)
//-----------------------------------------------------------------
// Single left-to-right pass over ctx->line. No copies, no writes
// to ctx->line — the original text stays fully intact for logging
// even when parsing fails partway through (e.g. bad arguments).
//-----------------------------------------------------------------
static void mu_parse(MU_CTX *ctx)
{
	const char *p = (const char*)ctx->line;
	const char *tok;
	u8 len;
	
	ctx->valid = 0;
	
	if(!mu_next_token(&p, &tok, &len))
	{
		ctx->code = MU_CMD_UNKNOWN;
		return;
	}
	ctx->code = mu_str2code(tok, len);
	
	switch(ctx->code)
	{
		case MU_CMD_MOV:
			if(!mu_next_int(&p, &ctx->args.mov.current_ma)) return;
			if(*mu_skip_spaces(p) != '\0') return;   // trailing extra token
			ctx->valid = 1;
			break;
		
		case MU_CMD_PULSE:
		{
			s32 i1, t, i2;
      if(!mu_next_int(&p, &i1)) return;
			if(!mu_next_int(&p, &t)) return;
			if(!mu_next_int(&p, &i2)) return;
			if(*mu_skip_spaces(p) != '\0') return;
      ctx->args.pulse.current_ma1 = i1;
			ctx->args.pulse.time_ms     = t;
			ctx->args.pulse.current_ma2 = i2;
			ctx->valid = 1;
			break;
		}
		
		case MU_CMD_EXIT:
			if(*mu_skip_spaces(p) != '\0') return;   // EXIT takes no arguments
			ctx->valid = 1;
			break;
		
		case MU_CMD_UNKNOWN:
		default:
			return;
	}
}

//-----------------------------------------------------------------
// u8 MU_TryReadCmd(MU_CTX *ctx)
//-----------------------------------------------------------------
// USART_RX_Try_ReadLine() is called only here — the application
// no longer needs to know it exists, or reach into the usart
// ring buffer directly at all.
//-----------------------------------------------------------------
u8 MU_TryReadCmd(MU_CTX *ctx)
{
	if(!USART_RX_Try_ReadLine(ctx->line))
		return 0;
	
	mu_parse(ctx);
	return 1;
}
