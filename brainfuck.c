#include <stdio.h>

void compile(FILE *, FILE *);

void compile(FILE *in, FILE *out) {
	fprintf(out,
		"section .bss\n"
		"\tarray\tresb\t30000\n"
		"section .text\n"
		"\tglobal _start\n"
		"_start:\n"
		"\txor\tr11, r11\n"
		"mem_zero:\n"
		"\tmov\tbyte [r11+array], 0\n"
		"\tinc\tr11\n"
		"\tcmp\tr11, 30000\n"
		"\tjl\tmem_zero\n"
		"\txor\tr11, r11\n");

	int c = 0;
	int unique = 0;
	int stack[100] = {0};
	int sp = 0;
	while ((c = fgetc(in)) != EOF) {
		switch (c) {
		case '>':
			fprintf(out, "\tinc\tr11\n");
			break;
		case '<':
			fprintf(out, "\tdec\tr11\n");
			break;
		case '+':
			fprintf(out, "\tinc\tbyte [r11+array]\n");
			break;
		case '-':
			fprintf(out, "\tdec\tbyte [r11+array]\n");
			break;
		case '.':
			fprintf(out, "\tcall\tputchar\n");
			break;
		case ',':
			fprintf(out, "\tcall\tgetchar\n");
			break;
		case '[':
			stack[sp++] = unique;
			fprintf(out,
				"\tcmp\tbyte [r11+array], 0\n"
				"\tje be%d\n"
				"bs%d:\n",
				unique, unique);
			unique++;
			break;
		case ']':
			sp--;
			fprintf(out,
				"\tcmp\tbyte [r11+array], 0\n"
				"\tjne bs%d\n"
				"be%d:\n",
				stack[sp], stack[sp]);
			break;
		default:
			break;
		}
	}

	fprintf(out,
		"\tmov\trax, 60\n"
		"\txor\trdi, rdi\n"
		"\tsyscall\n\n"
		"putchar:\n"
		"\tpush\tr11\n"
		"\tmov\trax, 1\n"
		"\tmov\trdi, 1\n"
		"\tlea\trsi, [r11+array]\n"
		"\tmov\trdx, 1\n"
		"\tsyscall\n"
		"\tpop\tr11\n"
		"\tret\n\n"
		"getchar:\n"
		"\tpush\tr11\n"
		"\tmov\trax, 0\n"
		"\tmov\trdi, 0\n"
		"\tlea\trsi, [r11+array]\n"
		"\tmov\trdx, 1\n"
		"\tsyscall\n"
		"\tpop\tr11\n"
		"\tret\n");
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: brainfuck <program>\n");
		return 0;
	}

	FILE *in = fopen(argv[1], "r");
	if (in == NULL) {
		perror("fopen");
		return -1;
	}
	FILE *out = fopen("out.s", "w");
	if (out == NULL) {
		perror("fopen");
		return -1;
	}
	compile(in, out);
	fclose(in);
	fclose(out);
	return 0;
}
