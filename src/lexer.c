#include <def.h>
#include <lexer.h>
#include <stdio.h>
#include <string.h>

#define ALPHA(a) (((a) >= 'a' && (a) <= 'z') || ((a) >= 'A' && (a) <= 'Z'))
#define NUMERIC(a) ((a) >= '0' && (a) <= '9')
#define ALPHA_NUMERIC(a) (ALPHA(a) || NUMERIC(a))
#define HEX_NUMERIC(a) (NUMERIC(a) || ((a) >= 'a' && (a) <= 'f') || ((a) >= 'A' && (a) <= 'F'))

#define CMPSP(tok)                                                                                 \
	(strncasecmp(tok " ", str, MIN(strlen(tok) + 1, size)) == 0) // CMP with space at end
#define CMPNA(tok)                                                                                 \
	(strncasecmp(tok, str, MIN(strlen(tok), size)) == 0 &&                                     \
	 !ALPHA(str[strlen(tok)])) // CMP with non alpha at end

void token_print(struct token *tok)
{
	if (!tok->length)
		return;

	char swp = tok->start[tok->length];
	tok->start[tok->length] = 0;
	printf("%s (%d)\n", tok->start, tok->type);
	tok->start[tok->length] = swp;
}

struct token token_resolve(char *str, u32 size)
{
	enum token_type type = UNKNOWN;
	u32 length = 0;

	// "Beautiful" ~ Everyone. Probably.
	if (NUMERIC(str[0])) {
		while (HEX_NUMERIC(str[length]))
			length++;

		if (str[length] == 'h' || str[length] == 'H') {
			type = HEX_NUM;
			length++;
		} else if (str[length] == 'b' || str[length] == 'B') {
			type = BIN_NUM;
			length++;
		} else {
			type = DEC_NUM;
		}
	} else if (str[0] == '\n') {
		type = NEWLINE;
		length = 1;
	} else if (str[0] == ' ') {
		type = SPACE;
		length = 1;
	} else if (str[0] == ',') {
		type = COMMA;
		length = 1;
	} else if (str[0] == '$') {
		type = DOLLAR;
		length = 1;
	} else if (str[0] == '/') {
		type = SLASH;
		length = 1;
	} else if (str[0] == '+') {
		type = PLUS;
		length = 1;
	} else if (str[0] == '.') {
		type = DOT;
		length = 1;
	} else if (str[0] == ':') {
		type = COLON;
		length = 1;
	} else if (str[0] == ';') {
		type = SEMICOLON;
		length = 1;
	} else if (str[0] == '#') {
		type = HASH;
		length = 1;
	} else if (str[0] == 'A' && !ALPHA(str[1])) {
		type = ACCU;
		length = 1;
	} else if (CMPNA("R0")) {
		type = R0;
		length = 2;
	} else if (CMPNA("R1")) {
		type = R1;
		length = 2;
	} else if (CMPNA("R2")) {
		type = R2;
		length = 2;
	} else if (CMPNA("R3")) {
		type = R3;
		length = 2;
	} else if (CMPNA("R4")) {
		type = R4;
		length = 2;
	} else if (CMPNA("R5")) {
		type = R5;
		length = 2;
	} else if (CMPNA("R6")) {
		type = R6;
		length = 2;
	} else if (CMPNA("R7")) {
		type = R7;
		length = 2;
	} else if (CMPSP("nop")) {
		type = NOP;
		length = 3;
	} else if (CMPSP("jbc")) {
		type = JBC;
		length = 3;
	} else if (CMPSP("jb")) {
		type = JB;
		length = 2;
	} else if (CMPSP("jnb")) {
		type = JNB;
		length = 3;
	} else if (CMPSP("jc")) {
		type = JC;
		length = 2;
	} else if (CMPSP("jnc")) {
		type = JNC;
		length = 3;
	} else if (CMPSP("jz")) {
		type = JZ;
		length = 2;
	} else if (CMPSP("jnz")) {
		type = JNZ;
		length = 3;
	} else if (CMPSP("sjmp")) {
		type = SJMP;
		length = 4;
	} else if (CMPSP("mov")) {
		type = MOV;
		length = 3;
	} else if (CMPSP("orl")) {
		type = ORL;
		length = 3;
	} else if (CMPSP("anl")) {
		type = ANL;
		length = 3;
	} else if (CMPSP("push")) {
		type = PUSH;
		length = 4;
	} else if (CMPSP("pop")) {
		type = POP;
		length = 3;
	} else if (CMPSP("movx")) {
		type = MOVX;
		length = 4;
	} else if (CMPSP("ajmp")) {
		type = AJMP;
		length = 4;
	} else if (CMPSP("acall")) {
		type = ACALL;
		length = 5;
	} else if (CMPSP("ljmp")) {
		type = LJMP;
		length = 4;
	} else if (CMPSP("lcall")) {
		type = LCALL;
		length = 5;
	} else if (CMPSP("reti")) {
		type = RETI;
		length = 4;
	} else if (CMPSP("ret")) {
		type = RET;
		length = 3;
	} else if (CMPSP("xrl")) {
		type = XRL;
		length = 3;
	} else if (CMPSP("cpl")) {
		type = CPL;
		length = 3;
	} else if (CMPSP("clr")) {
		type = CLR;
		length = 3;
	} else if (CMPSP("setb")) {
		type = SETB;
		length = 4;
	} else if (CMPSP("rr")) {
		type = RR;
		length = 2;
	} else if (CMPSP("rrc")) {
		type = RRC;
		length = 3;
	} else if (CMPSP("rl")) {
		type = RL;
		length = 2;
	} else if (CMPSP("rlc")) {
		type = RLC;
		length = 3;
	} else if (CMPSP("xlr")) {
		type = XLR;
		length = 3;
	} else if (CMPSP("jmp")) {
		type = JMP;
		length = 3;
	} else if (CMPSP("movc")) {
		type = MOVC;
		length = 4;
	} else if (CMPSP("inc")) {
		type = INC;
		length = 3;
	} else if (CMPSP("dec")) {
		type = DEC;
		length = 3;
	} else if (CMPSP("add")) {
		type = ADD;
		length = 3;
	} else if (CMPSP("addc")) {
		type = ADDC;
		length = 4;
	} else if (CMPSP("div")) {
		type = DIV;
		length = 3;
	} else if (CMPSP("dubb")) {
		type = DUBB;
		length = 4;
	} else if (CMPSP("mul")) {
		type = MUL;
		length = 3;
	} else if (CMPSP("cjne")) {
		type = CJNE;
		length = 4;
	} else if (CMPSP("swap")) {
		type = SWAP;
		length = 4;
	} else if (CMPSP("da")) {
		type = DA;
		length = 2;
	} else if (CMPSP("crl")) {
		type = CRL;
		length = 3;
	} else if (CMPSP("xch")) {
		type = XCH;
		length = 3;
	} else if (CMPSP("djnz")) {
		type = DJNZ;
		length = 4;
	} else if (CMPSP("xchd")) {
		type = XCHD;
		length = 4;
	} else if (CMPSP("call")) {
		type = CALL;
		length = 4;
	} else if (CMPSP("org")) {
		type = ORG;
		length = 3;
	} else if (CMPSP("db")) {
		type = DB;
		length = 2;
	} else if (CMPSP("dw")) {
		type = DW;
		length = 2;
	} else if (CMPSP("data")) { // Lars: "Who cares?"
		type = DATA;
		length = 4;
	} else if (CMPSP("bit")) {
		type = BIT;
		length = 3;
	} else if (CMPSP("include")) {
		type = INCLUDE;
		length = 7;
	} else {
		if (ALPHA(str[0])) {
			while (ALPHA_NUMERIC(str[length]))
				length++;

			if (length) {
				type = STRING;
				length++;
			}
		}
	}

	struct token tok = {
		.type = type,
		.start = str,
		.length = length,
	};

	return tok;
}
