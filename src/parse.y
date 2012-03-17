/*
 * Copyright (c) 2011, 2012 Arto Jonsson <artoj@iki.fi>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
%{
#include <stdio.h>
#include <stdint.h>

#include "cpu.h"

#define YYSTYPE int

void yyerror(const char *s)
{
	fprintf(stderr, "error: %s\n", s);
}

int yywrap(void)
{
	return 1;
}

int main(void)
{
	return yyparse();
}

%}

%token OP COMMA REG NUMBER

%%

instructions:
	| instructions instruction

instruction:
	Rinstruction
	|
	Iinstruction
	|
	Jinstruction
	;

Rinstruction:
	OP REG COMMA REG COMMA REG
	{
		uint32_t ins = 0;

		ins = ins | $1;
		ins <<= 5;

		ins = ins | $4;
		ins <<= 5;

		ins = ins | $6;
		ins <<= 5;

		/* XXX shamt and funct are ignored */
		ins = ins | $2;
		ins <<= 11;

		fwrite(&ins, sizeof(uint32_t), 1, stdout);
	}
	;

Iinstruction:
	OP REG COMMA REG COMMA NUMBER
	{
		uint32_t ins = 0;

		ins = ins | $1;
		ins <<= 5;

		ins = ins | $4;
		ins <<= 5;

		ins = ins | $2;
		ins <<= 16;

		ins = ins | $6;

		fwrite(&ins, sizeof(uint32_t), 1, stdout);
	}
	;

Jinstruction:
	OP NUMBER
	{
		uint32_t ins = 0;

		ins = ins | $1;
		ins <<= 26;

		ins = ins | $2;

		fwrite(&ins, sizeof(uint32_t), 1, stdout);
	}
	;
