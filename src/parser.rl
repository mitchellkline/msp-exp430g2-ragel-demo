#include "board.h"
#include <string.h>
#include <stdio.h>
#include "parser.h"

static const char *help_msg =
"             get <addr>: Get value of register at <addr>. <addr> is hex,\r\n"
"                         e.g. 0x00.\r\n"
"       set <addr> <val>: Set value of register at <addr> to <val>. <addr>\r\n"
"                         and <val> are hex, e.g. 0x00.\r\n"
"     getbit <addr> <bp>: Get value of bit position <bp> at address <addr>.\r\n"
"setbit <addr> <bp> <bv>: Set value of bit position <bp> at address <addr>\r\n"
"                         to bit value <bv>.\r\n"
"           led<n> [0|1]: Set led <n> [off|on].\r\n"
"                 help|?: Display this help message.\r\n"
;

%%{
	machine parser;
	write data;
}%%

static enum eparser h2i(unsigned char *i, char h)
{
	if (h >= '0' && h <= '9') {
		*i = h - '0';
	}
	else if (h>= 'A' && h <= 'F') {
		*i = h - 'A' + 0xA;
	}
	else if (h>= 'a' && h <= 'f') {
		*i = h - 'a' + 0xA;
	}
	else {
		return EPARSER_RANGE;
	}
	return EPARSER_SUCCESS;
}

enum eparser parse(char *p) 
{
	char *pe = p + strlen(p);
	int cs;

	unsigned int regaddr = 0;
	unsigned int regval = 0;
	unsigned int bm = 0;
	unsigned int bv = 0;
	unsigned int lednum = 0;

	enum {
		UNDEF,
		GET,
		SET,
		GETBIT,
		SETBIT,
		HELP
	} command = UNDEF;

	%%{
		action set_regaddr 
		{
			unsigned char tmp = 0;
			h2i(&tmp, fc);
			regaddr = (regaddr << 4) + tmp;
		}
		
		action set_regval 
		{
			unsigned char tmp = 0;
			h2i(&tmp, fc);
			regval = (regval << 4) + tmp;
		}
		
		action set_bm
		{
			bm = bm*10 + (fc - '0');
			bm = (1 << bm);
		}

		action set_bv
		{
			bv = fc - '0';
		}

		action set_lednum
		{
			lednum = fc - '0';
			if (lednum == 1) {
				regaddr = P1OUT_;
				bm = P1_LED_RED;
			}
			else if (lednum == 2) {
				regaddr = P1OUT_;
				bm = P1_LED_GRN;
			}
		}

		regaddr_f = '0x'(xdigit@set_regaddr)+;
		regval_f = '0x'(xdigit@set_regval)+;
		bp_f = (digit@set_bm)+;
		bv_f = [01]@set_bv;
		lednum_f = [12]@set_lednum;

		get_cmd_f = 
			'get'@{command = GET;} space regaddr_f;
		set_cmd_f = 
			'set'@{command = SET;} space regaddr_f space regval_f;
		getbit_cmd_f = 
			'getbit'@{command = GETBIT;} space regaddr_f space bp_f;
		setbit_cmd_f = 
			'setbit'@{command = SETBIT;} space regaddr_f space bp_f 
			space bv_f;

		led_cmd_f =
			'led'@{command = SETBIT;} lednum_f space bv_f;
		help_cmd_f = ('?'|'help')@{command = HELP;};
		
		main := ( get_cmd_f 
			| set_cmd_f 
			| getbit_cmd_f
			| setbit_cmd_f
			| led_cmd_f
			| help_cmd_f);

		write init;
		write exec;
	}%%
	
	unsigned int *regaddr_p = (unsigned int *)regaddr;

	if ( cs < parser_first_final ) {
		return EPARSER_CS;
	}
	if (command == GET)  {
		#ifdef DEBUG
		printf("Register address: 0x%x\r\n", regaddr);
		#endif
		printf("0x%x\r\n",*regaddr_p);

		return EPARSER_SUCCESS;
	}
	else if (command == SET) {
		#ifdef DEBUG
		printf("Register address: 0x%x\r\n", regaddr);
		printf("Register value: 0x%x\r\n", regval);
		#endif
		*regaddr_p = regval;
		return EPARSER_SUCCESS;
	}
	else if (command == GETBIT) {
		printf("%u\r\n",*regaddr_p & bm);
		return EPARSER_SUCCESS;
	}
	else if (command == SETBIT) {
		if (bv == 1) 
			*regaddr_p |= bm;
		else
			*regaddr_p &= ~bm;
		return EPARSER_SUCCESS;
	}
	else if (command == HELP) {
		printf("%s", help_msg);
		return EPARSER_SUCCESS;
	}
	else {
		return EPARSER_UNDEF;
	}

}
