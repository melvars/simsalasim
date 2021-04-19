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
 * Main parsing
 */

static u32 parse_nop(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	rom_add(0);
	return 0;
}

static u32 parse_jbc(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_jb(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_jnb(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_jc(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_jnc(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_jz(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_jnz(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_sjmp(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_mov(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_orl(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_anl(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_push(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_pop(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_movx(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_ajmp(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_acall(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_ljmp(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_lcall(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_reti(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_ret(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_xrl(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_cpl(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_clr(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_setb(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_rr(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_rrc(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_rl(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_rlc(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_xlr(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_jmp(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_movc(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_inc(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_dec(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_add(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_addc(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_div(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_dubb(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_mul(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_cjne(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_swap(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_da(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_crl(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_xch(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_djnz(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_xchd(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_call(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_org(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_db(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_dw(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_include(u32 line, char *str, u32 size)
{
	UNUSED(line);
	UNUSED(str);
	UNUSED(size);
	return 0;
}

static u32 parse_instruction(u32 line, char *str, u32 size)
{
	struct token tok = token_resolve(str, size);

	token_print(&tok);

	u32 ret = tok.length;

	switch (tok.type) {
	case UNKNOWN:
		warnings_add(line, "Unknown instruction");
		return 0;
	case NEWLINE:
		break;
	case NOP:
		ret += parse_nop(line, str, size);
		break;
	case JBC:
		ret += parse_jbc(line, str, size);
		break;
	case JB:
		ret += parse_jb(line, str, size);
		break;
	case JNB:
		ret += parse_jnb(line, str, size);
		break;
	case JC:
		ret += parse_jc(line, str, size);
		break;
	case JNC:
		ret += parse_jnc(line, str, size);
		break;
	case JZ:
		ret += parse_jz(line, str, size);
		break;
	case JNZ:
		ret += parse_jnz(line, str, size);
		break;
	case SJMP:
		ret += parse_sjmp(line, str, size);
		break;
	case MOV:
		ret += parse_mov(line, str, size);
		break;
	case ORL:
		ret += parse_orl(line, str, size);
		break;
	case ANL:
		ret += parse_anl(line, str, size);
		break;
	case PUSH:
		ret += parse_push(line, str, size);
		break;
	case POP:
		ret += parse_pop(line, str, size);
		break;
	case MOVX:
		ret += parse_movx(line, str, size);
		break;
	case AJMP:
		ret += parse_ajmp(line, str, size);
		break;
	case ACALL:
		ret += parse_acall(line, str, size);
		break;
	case LJMP:
		ret += parse_ljmp(line, str, size);
		break;
	case LCALL:
		ret += parse_lcall(line, str, size);
		break;
	case RETI:
		ret += parse_reti(line, str, size);
		break;
	case RET:
		ret += parse_ret(line, str, size);
		break;
	case XRL:
		ret += parse_xrl(line, str, size);
		break;
	case CPL:
		ret += parse_cpl(line, str, size);
		break;
	case CLR:
		ret += parse_clr(line, str, size);
		break;
	case SETB:
		ret += parse_setb(line, str, size);
		break;
	case RR:
		ret += parse_rr(line, str, size);
		break;
	case RRC:
		ret += parse_rrc(line, str, size);
		break;
	case RL:
		ret += parse_rl(line, str, size);
		break;
	case RLC:
		ret += parse_rlc(line, str, size);
		break;
	case XLR:
		ret += parse_xlr(line, str, size);
		break;
	case JMP:
		ret += parse_jmp(line, str, size);
		break;
	case MOVC:
		ret += parse_movc(line, str, size);
		break;
	case INC:
		ret += parse_inc(line, str, size);
		break;
	case DEC:
		ret += parse_dec(line, str, size);
		break;
	case ADD:
		ret += parse_add(line, str, size);
		break;
	case ADDC:
		ret += parse_addc(line, str, size);
		break;
	case DIV:
		ret += parse_div(line, str, size);
		break;
	case DUBB:
		ret += parse_dubb(line, str, size);
		break;
	case MUL:
		ret += parse_mul(line, str, size);
		break;
	case CJNE:
		ret += parse_cjne(line, str, size);
		break;
	case SWAP:
		ret += parse_swap(line, str, size);
		break;
	case DA:
		ret += parse_da(line, str, size);
		break;
	case CRL:
		ret += parse_crl(line, str, size);
		break;
	case XCH:
		ret += parse_xch(line, str, size);
		break;
	case DJNZ:
		ret += parse_djnz(line, str, size);
		break;
	case XCHD:
		ret += parse_xchd(line, str, size);
		break;
	case CALL:
		ret += parse_call(line, str, size);
		break;
	case ORG:
		ret += parse_org(line, str, size);
		break;
	case DB:
		ret += parse_db(line, str, size);
		break;
	case DW:
		ret += parse_dw(line, str, size);
		break;
	case INCLUDE:
		ret += parse_include(line, str, size);
		break;
	default:
		warnings_add(line, "Super-unknown instruction");
	}

	return ret;
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

	u32 line = 0;

	for (u32 i = 0; i < size; i++) {
		if (buf[i] == '\0')
			break;

		if (buf[i] == '\n') {
			line++;
			continue;
		}

		i += parse_instruction(line, buf + i, size - i);
	}

	if (warnings_exist()) {
		warnings_print();
		return 0;
	}

	return 1;
}
