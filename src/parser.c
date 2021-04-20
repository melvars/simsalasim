#include <assert.h>
#include <def.h>
#include <lexer.h>
#include <parser.h>
#include <stdio.h>
#include <string.h>
#include <warnings.h>

/**
 * Definitions
 */

#define ROM_SIZE 4096 // TODO: Actual ROM size?

/**
 * ROM blub blabs
 */

static u8 rom[ROM_SIZE] = { 0 };
static u32 rom_index = 0;
static void rom_add(u8 byte)
{
	rom[rom_index] = byte;
	rom_index++;
}

/**
 * Toks parsing
 */

static u32 toks_count(struct token *toks)
{
	struct token *p = toks;
	while (p && p->type)
		p++;
	return p - toks;
}

static struct token *toks_peek(struct token *toks, u32 cnt)
{
	return toks + cnt;
}

static struct token *toks_peek_end(struct token *toks)
{
	return toks + toks_count(toks) - 1;
}

/**
 * Main parsing
 */

static void parse_nop(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
	rom_add(0);
}

static void parse_jbc(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_jb(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_jnb(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_jc(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_jnc(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_jz(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_jnz(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_sjmp(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_mov(struct context *ctx, struct token *toks)
{
	if (toks_peek_end(toks)->type == COMMA)
		warnings_add(ctx, "Unexpected end of line");
	else if (toks_count(toks) > 5)
		warnings_add(ctx, "Too many arguments");
}

static void parse_orl(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_anl(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_push(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_pop(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_movx(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_ajmp(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_acall(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_ljmp(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_lcall(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_reti(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_ret(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_xrl(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_cpl(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_clr(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_setb(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_rr(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_rrc(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_rl(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_rlc(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_xlr(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_jmp(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_movc(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_inc(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_dec(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_add(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_addc(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_div(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_dubb(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_mul(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_cjne(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_swap(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_da(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_crl(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_xch(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_djnz(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_xchd(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_call(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_org(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_db(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_dw(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_include(struct context *ctx, struct token *toks)
{
	UNUSED(ctx);
	UNUSED(toks);
}

static void parse_string(struct context *ctx, struct token *toks)
{
	enum token_type next = toks_peek(toks, 1)->type;
	if (next == COLON) {
		// TODO: Add label lookup tables
	} else if (next == DATA) {
		// TODO: Constants map
	} else if (next == BIT) {
		// TODO: Constants map
	} else {
		warnings_add(ctx, "Expected data/bit/colon");
	}
}

static u32 parse_line(struct context *ctx, char *str, u32 size)
{
	struct token toks[32] = { 0 };
	u8 tok_ind = 0;
	u8 str_ind = 0;

	while (1) {
		struct token tok = token_resolve(str + str_ind, size - str_ind);
		if (tok.type == NEWLINE) {
			ctx->line++;
			ctx->column = 0;
			break;
		}

		if (tok.type == UNKNOWN) {
			break;
		}

		if (tok.type == SPACE) {
			str_ind += tok.length;
			continue;
		}

		token_print(&tok);

		if (tok_ind + 1 >= (u8)(sizeof(toks) / sizeof(toks[0]))) {
			warnings_add(ctx, "Token overflow");
			return str_ind;
		}

		toks[tok_ind++] = tok;
		str_ind += tok.length;
	}

	ctx->column = str_ind;

	if (!tok_ind) {
		warnings_add(ctx, "Parsing failed");
		return str_ind;
	}

	switch (toks[0].type) {
	case UNKNOWN:
		warnings_add(ctx, "Unknown instruction");
		break;
	case NEWLINE:
		break;
	case NOP:
		parse_nop(ctx, toks);
		break;
	case JBC:
		parse_jbc(ctx, toks);
		break;
	case JB:
		parse_jb(ctx, toks);
		break;
	case JNB:
		parse_jnb(ctx, toks);
		break;
	case JC:
		parse_jc(ctx, toks);
		break;
	case JNC:
		parse_jnc(ctx, toks);
		break;
	case JZ:
		parse_jz(ctx, toks);
		break;
	case JNZ:
		parse_jnz(ctx, toks);
		break;
	case SJMP:
		parse_sjmp(ctx, toks);
		break;
	case MOV:
		parse_mov(ctx, toks);
		break;
	case ORL:
		parse_orl(ctx, toks);
		break;
	case ANL:
		parse_anl(ctx, toks);
		break;
	case PUSH:
		parse_push(ctx, toks);
		break;
	case POP:
		parse_pop(ctx, toks);
		break;
	case MOVX:
		parse_movx(ctx, toks);
		break;
	case AJMP:
		parse_ajmp(ctx, toks);
		break;
	case ACALL:
		parse_acall(ctx, toks);
		break;
	case LJMP:
		parse_ljmp(ctx, toks);
		break;
	case LCALL:
		parse_lcall(ctx, toks);
		break;
	case RETI:
		parse_reti(ctx, toks);
		break;
	case RET:
		parse_ret(ctx, toks);
		break;
	case XRL:
		parse_xrl(ctx, toks);
		break;
	case CPL:
		parse_cpl(ctx, toks);
		break;
	case CLR:
		parse_clr(ctx, toks);
		break;
	case SETB:
		parse_setb(ctx, toks);
		break;
	case RR:
		parse_rr(ctx, toks);
		break;
	case RRC:
		parse_rrc(ctx, toks);
		break;
	case RL:
		parse_rl(ctx, toks);
		break;
	case RLC:
		parse_rlc(ctx, toks);
		break;
	case XLR:
		parse_xlr(ctx, toks);
		break;
	case JMP:
		parse_jmp(ctx, toks);
		break;
	case MOVC:
		parse_movc(ctx, toks);
		break;
	case INC:
		parse_inc(ctx, toks);
		break;
	case DEC:
		parse_dec(ctx, toks);
		break;
	case ADD:
		parse_add(ctx, toks);
		break;
	case ADDC:
		parse_addc(ctx, toks);
		break;
	case DIV:
		parse_div(ctx, toks);
		break;
	case DUBB:
		parse_dubb(ctx, toks);
		break;
	case MUL:
		parse_mul(ctx, toks);
		break;
	case CJNE:
		parse_cjne(ctx, toks);
		break;
	case SWAP:
		parse_swap(ctx, toks);
		break;
	case DA:
		parse_da(ctx, toks);
		break;
	case CRL:
		parse_crl(ctx, toks);
		break;
	case XCH:
		parse_xch(ctx, toks);
		break;
	case DJNZ:
		parse_djnz(ctx, toks);
		break;
	case XCHD:
		parse_xchd(ctx, toks);
		break;
	case CALL:
		parse_call(ctx, toks);
		break;
	case ORG:
		parse_org(ctx, toks);
		break;
	case DB:
		parse_db(ctx, toks);
		break;
	case DW:
		parse_dw(ctx, toks);
		break;
	case INCLUDE:
		parse_include(ctx, toks);
		break;
	case STRING:
		parse_string(ctx, toks);
		break;
	case SPACE:
	case HASH:
	case DOLLAR:
	case SLASH:
	case PLUS:
	case COMMA:
	case DOT:
	case COLON:
	case SEMICOLON:
	case DEC_NUM:
	case HEX_NUM:
	case BIN_NUM:
	case ACCU:
	case ATR0:
	case ATR1:
	case R0:
	case R1:
	case R2:
	case R3:
	case R4:
	case R5:
	case R6:
	case R7:
	case DATA:
	case BIT:
		warnings_add(ctx, "Random non-instruction found");
		break;
	case INSTR_START:
	case INSTR_END:
	case NUM_START:
	case NUM_END:
	case REGS_START:
	case REGS_END:
		warnings_add(ctx, "Got enum boundary");
		break;
	default:
		warnings_add(ctx, "Super-unknown instruction");
		break;
	}

	return str_ind;
}

static void clean_buffers(void)
{
	if (rom_index)
		memset(rom, 0, sizeof(rom));
	warnings_clear();
}

u8 parse(char *buf, u32 size)
{
	clean_buffers();

	struct context ctx = { .line = 0, .column = 0 };

	for (u32 i = 0; i < size; i++) {
		if (buf[i] == '\0')
			break;

		if (buf[i] == '\n') {
			ctx.line++;
			ctx.column = 0;
			continue;
		}

		u32 len = parse_line(&ctx, buf + i, size - i);
		i += len;
		ctx.column += len;
	}

	if (warnings_exist()) {
		warnings_print();
		return 0;
	}

	return 1;
}
