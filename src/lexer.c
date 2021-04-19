#include <def.h>
#include <lexer.h>
#include <stdio.h>
#include <string.h>

#define CMP(tok) (strncmp(tok " ", str, MIN(strlen(tok) + 1, size)) == 0)

void token_print(struct token *tok)
{
	if (!tok->length)
		return;

	char swp = tok->start[tok->length];
	tok->start[tok->length] = 0;
	printf("%s\n", tok->start);
	tok->start[tok->length] = swp;
}

struct token token_resolve(char *str, u32 size)
{
	enum token_type type = UNKNOWN;
	u32 length = 0;

	// "Beautiful" ~ Everyone. Probably.
	if (CMP("nop")) {
		type = NOP;
		length = 3;
	} else if (CMP("jbc")) {
		type = JBC;
		length = 3;
	} else if (CMP("jb")) {
		type = JB;
		length = 2;
	} else if (CMP("jnb")) {
		type = JNB;
		length = 3;
	} else if (CMP("jc")) {
		type = JC;
		length = 2;
	} else if (CMP("jnc")) {
		type = JNC;
		length = 3;
	} else if (CMP("jz")) {
		type = JZ;
		length = 2;
	} else if (CMP("jnz")) {
		type = JNZ;
		length = 3;
	} else if (CMP("sjmp")) {
		type = SJMP;
		length = 4;
	} else if (CMP("mov")) {
		type = MOV;
		length = 3;
	} else if (CMP("orl")) {
		type = ORL;
		length = 3;
	} else if (CMP("anl")) {
		type = ANL;
		length = 3;
	} else if (CMP("push")) {
		type = PUSH;
		length = 4;
	} else if (CMP("pop")) {
		type = POP;
		length = 3;
	} else if (CMP("movx")) {
		type = MOVX;
		length = 4;
	} else if (CMP("ajmp")) {
		type = AJMP;
		length = 4;
	} else if (CMP("acall")) {
		type = ACALL;
		length = 5;
	} else if (CMP("ljmp")) {
		type = LJMP;
		length = 4;
	} else if (CMP("lcall")) {
		type = LCALL;
		length = 5;
	} else if (CMP("reti")) {
		type = RETI;
		length = 4;
	} else if (CMP("ret")) {
		type = RET;
		length = 3;
	} else if (CMP("xrl")) {
		type = XRL;
		length = 3;
	} else if (CMP("cpl")) {
		type = CPL;
		length = 3;
	} else if (CMP("clr")) {
		type = CLR;
		length = 3;
	} else if (CMP("setb")) {
		type = SETB;
		length = 4;
	} else if (CMP("rr")) {
		type = RR;
		length = 2;
	} else if (CMP("rrc")) {
		type = RRC;
		length = 3;
	} else if (CMP("rl")) {
		type = RL;
		length = 2;
	} else if (CMP("rlc")) {
		type = RLC;
		length = 3;
	} else if (CMP("xlr")) {
		type = XLR;
		length = 3;
	} else if (CMP("jmp")) {
		type = JMP;
		length = 3;
	} else if (CMP("movc")) {
		type = MOVC;
		length = 4;
	} else if (CMP("inc")) {
		type = INC;
		length = 3;
	} else if (CMP("dec")) {
		type = DEC;
		length = 3;
	} else if (CMP("add")) {
		type = ADD;
		length = 3;
	} else if (CMP("addc")) {
		type = ADDC;
		length = 4;
	} else if (CMP("div")) {
		type = DIV;
		length = 3;
	} else if (CMP("dubb")) {
		type = DUBB;
		length = 4;
	} else if (CMP("mul")) {
		type = MUL;
		length = 3;
	} else if (CMP("cjne")) {
		type = CJNE;
		length = 4;
	} else if (CMP("swap")) {
		type = SWAP;
		length = 4;
	} else if (CMP("da")) {
		type = DA;
		length = 2;
	} else if (CMP("crl")) {
		type = CRL;
		length = 3;
	} else if (CMP("xch")) {
		type = XCH;
		length = 3;
	} else if (CMP("djnz")) {
		type = DJNZ;
		length = 4;
	} else if (CMP("xchd")) {
		type = XCHD;
		length = 4;
	} else if (CMP("call")) {
		type = CALL;
		length = 4;
	} else if (CMP("org")) {
		type = ORG;
		length = 3;
	} else if (CMP("db")) {
		type = DB;
		length = 2;
	} else if (CMP("dw")) {
		type = DW;
		length = 2;
	} else if (CMP("include")) {
		type = INCLUDE;
		length = 7;
	}

	struct token tok = {
		.type = type,
		.start = str,
		.length = length,
		.data = 0,
	};

	return tok;
}
