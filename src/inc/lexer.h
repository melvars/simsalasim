#ifndef LEXER_H
#define LEXER_H

enum token {
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
	SWP,
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
	BRACE_OPEN,
	BRACE_CLOSE,
	DATA,
	BIT,
};

#endif
