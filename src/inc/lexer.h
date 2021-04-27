#ifndef LEXER_H
#define LEXER_H

#include <def.h>

enum token_type {
	UNKNOWN,
	NEWLINE,
	END,
	SPACE,

	INSTR_START,
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
	DATA,
	BIT,
	INCLUDE,
	INSTR_END,

	HASH,
	DOLLAR,
	SLASH,
	PLUS,
	COMMA,
	DOT,
	COLON,
	SEMICOLON,

	NUM_START,
	DEC_NUM,
	HEX_NUM,
	BIN_NUM,
	NUM_END,

	STRING,

	// Registers
	REGS_START,
	ACCU,
	ATR0,
	ATR1,
	R0,
	R1,
	R2,
	R3,
	R4,
	R5,
	R6,
	R7,
	REGS_END,
};

struct token {
	enum token_type type;
	char *start;
	u32 length;
};

void token_print(struct token *tok);
struct token token_resolve(char *token, u32 size);

#endif
