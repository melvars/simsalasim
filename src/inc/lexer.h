#ifndef LEXER_H
#define LEXER_H

#include <def.h>

enum token_type {
	UNKNOWN,
	NEWLINE,
	NOP,
	JBC,
	JB,
	JNB,
	JC,
	JNC,
	JZ,
	JNZ,
	SJMP,
	MOV,
	ORL,
	ANL,
	PUSH,
	POP,
	MOVX,
	AJMP,
	ACALL,
	LJMP,
	LCALL,
	RETI,
	RET,
	XRL,
	CPL,
	CLR,
	SETB,
	RR,
	RRC,
	RL,
	RLC,
	XLR,
	JMP,
	MOVC,
	INC,
	DEC,
	ADD,
	ADDC,
	DIV,
	DUBB,
	MUL,
	CJNE,
	SWAP,
	DA,
	CRL,
	XCH,
	DJNZ,
	XCHD,
	CALL,
	ORG,
	DB,
	DW,
	INCLUDE,
};

struct token {
	enum token_type type;
	char *start;
	u32 length;
	void *data;
};

void token_print(struct token *tok);
struct token token_resolve(char *token, u32 size);

#endif
