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
#include <stdlib.h>

#include "cpu.h"

int main(int argc, char *argv[])
{
	struct cpu_ctx	 ctx;
	FILE		*input;
	uint32_t	 ins;
	int		 i;

	if (argc != 2) {
		printf("usage: %s <image>\n", argv[0]);
		return 1;
	}

	for (i = 0; i < REG_LAST; i++)
		ctx.regs[i] = 0;

	if ((input = fopen(argv[1], "r")) == NULL)
	{
		fprintf(stderr, "cannot open image file for reading\n");
		return 2;
	}

	while (fread(&ins, sizeof(uint32_t), 1, input) == 1)
		cpu_run(&ctx, ins);

	fclose(input);
	cpu_dump(&ctx);
	return 0;
}
