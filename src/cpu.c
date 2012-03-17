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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cpu.h"

/* must be sorted */
static const struct op ops_by_name[] = {
	{ "add",  OP_TYPE_R,    OP_ADD  },
	{ "addi", OP_TYPE_R,    OP_ADDI },
	{ "nop",  OP_TYPE_NONE, OP_NOP  },
	{ "sub",  OP_TYPE_R,    OP_SUB  },
	{ NULL,   OP_TYPE_NONE, OP_LAST }
};

/* must be sorted */
static const struct reg regs_by_name[] = {
	{ "$a0", REG_A0 },
	{ "$a1", REG_A1 },
	{ "$a2", REG_A2 },
	{ "$a3", REG_A3 },
	{ "$at", REG_AT },
	{ "$fp", REG_FP },
	{ "$gp", REG_GP },
	{ "$k0", REG_K0 },
	{ "$k1", REG_K1 },
	{ "$ra", REG_RA },
	{ "$s0", REG_S0 },
	{ "$s1", REG_S1 },
	{ "$s2", REG_S2 },
	{ "$s3", REG_S3 },
	{ "$s4", REG_S4 },
	{ "$s5", REG_S5 },
	{ "$s6", REG_S6 },
	{ "$s7", REG_S7 },
	{ "$sp", REG_SP },
	{ "$t0", REG_T0 },
	{ "$t1", REG_T1 },
	{ "$t2", REG_T2 },
	{ "$t3", REG_T3 },
	{ "$t4", REG_T4 },
	{ "$t5", REG_T5 },
	{ "$t6", REG_T6 },
	{ "$t7", REG_T7 },
	{ "$t8", REG_T8 },
	{ "$t9", REG_T9 },
	{ "$v0", REG_V0 },
	{ "$v1", REG_V1 },
	{ "$zero", REG_ZERO },
	{ NULL, REG_LAST }
};

void cpu_nop(struct cpu_ctx *, uint32_t);
void cpu_add(struct cpu_ctx *, uint32_t);
void cpu_addi(struct cpu_ctx *, uint32_t);
void cpu_sub(struct cpu_ctx *, uint32_t);

typedef void (*opfunc)(struct cpu_ctx *, uint32_t);

static const opfunc oplist[OP_LAST + 1] =
{
	cpu_nop,
	cpu_add,
	cpu_addi,
	cpu_sub,
	NULL
};

static int
opcmp_name(const void* a, const void *b)
{
	return (strcmp((char *)a, ((struct op *)b)->name));
}

static int
regcmp_name(const void* a, const void *b)
{
	return (strcmp((char *)a, ((struct reg *)b)->name));
}

int
name2op(const char *s)
{
	struct op *o;

	o = bsearch(s, ops_by_name, OP_LAST, sizeof(struct op), opcmp_name);
	if (o != NULL)
		return o->code;

	return -1;
}

int
name2reg(const char *s)
{
	struct reg *r;

	r = bsearch(s, regs_by_name, REG_LAST, sizeof(struct reg), regcmp_name);
	if (r != NULL)
		return r->num;

	return -1;
}

uint8_t
cpu_get_op(uint32_t ins)
{
	return (ins & 037400000000) >> 26;
}

uint8_t
cpu_get_rs(uint32_t ins)
{
	return (ins & 000370000000) >> 21;
}

uint8_t
cpu_get_rt(uint32_t ins)
{
	return (ins & 000007600000) >> 16;
}

uint8_t
cpu_get_rd(uint32_t ins)
{
	return (ins & 000000174000) >> 11;
}

uint8_t
cpu_get_shamt(uint32_t ins)
{
	return (ins & 000000003700) >> 6;
}

uint8_t
cpu_get_funct(uint32_t ins)
{
	return (ins & 000000000077);
}

uint16_t
cpu_get_imm(uint32_t ins)
{
	return (ins & 000000177777);
}

void
cpu_run(struct cpu_ctx *ctx, uint32_t ins)
{
	uint8_t op;

	op = cpu_get_op(ins);

	if (op < OP_LAST)
		oplist[op](ctx, ins);
	else
		printf("Unrecognized op code: %o\n", op);
}

void
cpu_dump(struct cpu_ctx *ctx)
{
	int i;

	for (i = 0; i < REG_LAST; i++)
		printf("%2d %d\n", i, ctx->regs[i]);
}

void
cpu_nop(struct cpu_ctx *ctx, uint32_t ins)
{
}

void
cpu_add(struct cpu_ctx *ctx, uint32_t ins)
{
	ctx->regs[cpu_get_rd(ins)] = ctx->regs[cpu_get_rs(ins)] +
	    ctx->regs[cpu_get_rt(ins)];
}

void
cpu_addi(struct cpu_ctx *ctx, uint32_t ins)
{
	ctx->regs[cpu_get_rt(ins)] = ctx->regs[cpu_get_rs(ins)] +
	    cpu_get_imm(ins);
}

void
cpu_sub(struct cpu_ctx *ctx, uint32_t ins)
{
	ctx->regs[cpu_get_rd(ins)] = ctx->regs[cpu_get_rs(ins)] -
	    ctx->regs[cpu_get_rt(ins)];
}
