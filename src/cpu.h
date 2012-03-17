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

#ifndef CPU_H
#define CPU_H

#include <stdint.h>

enum
{
	OP_NOP = 0,
	OP_ADD,
	OP_ADDI,
	OP_SUB,
	OP_LAST /* used only for indexing */
};

enum
{
	OP_TYPE_NONE,
	OP_TYPE_R,
	OP_TYPE_I,
	OP_TYPE_J,
};

enum
{
	/* the constant value 0 */
	REG_ZERO = 0,
	/* assembler temporary */
	REG_AT,
	/* function results or expression evaluation */
	REG_V0, REG_V1,
	/* arguments */
	REG_A0, REG_A1, REG_A2, REG_A3,
	/* temporaries */
	REG_T0, REG_T1, REG_T2, REG_T3, REG_T4, REG_T5, REG_T6, REG_T7,
	/* saved temporaries */
	REG_S0, REG_S1, REG_S2, REG_S3, REG_S4, REG_S5, REG_S6, REG_S7,
	/* temporaries */
	REG_T8, REG_T9,
	/* reserved for OS kernel */
	REG_K0, REG_K1,
	/* global pointer */
	REG_GP,
	/* stack pointer */
	REG_SP,
	/* frame pointer */
	REG_FP,
	/* return address */
	REG_RA,
	/* used only for indexing */
	REG_LAST
};

struct op
{
	char	*name;
	uint8_t	 type;
	uint8_t	 code;
};

struct reg
{
	char	*name;
	uint8_t	 num;
};

struct cpu_ctx
{
	uint32_t	regs[REG_LAST]; /* registers */
};

void cpu_run(struct cpu_ctx *, uint32_t);
void cpu_dump(struct cpu_ctx *);
int name2op(const char *s);
int name2reg(const char *s);

#endif /* CPU_H */
