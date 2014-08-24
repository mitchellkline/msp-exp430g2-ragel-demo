#include "board.h"
#include <string.h>
#include <stdio.h>
#include "parser.h"

%%{
	machine board;
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

	enum {
		UNDEF,
		GET,
		SET
	} command = UNDEF;

	%%{
		action set_regaddr {
			unsigned char tmp = 0;
			h2i(&tmp, fc);
			regaddr = (regaddr << 4) + tmp;
		}
		
		action set_regval {
			unsigned char tmp = 0;
			h2i(&tmp, fc);
			regval = (regval << 4) + tmp;
		}

		action set_get {
			command = GET;
		}

		action set_set {
			command = SET;
		}

		regaddr_f = (xdigit@set_regaddr)+;
		regval_f = (xdigit@set_regval)+;
		get_cmd_f = 'get' space regaddr_f;
		set_cmd_f = 'set' space regaddr_f space regval_f;
		main := ( get_cmd_f@set_get | set_cmd_f@set_set );

		write init;
		write exec;
	}%%
	
	if ( cs < board_first_final ) {
		return EPARSER_CS;
	}
	if (command == GET)  {
		#ifdef DEBUG
		printf("Register address: %u\r\n", regaddr);
		#endif
		printf("%x\r\n",*((unsigned int *)regaddr));

		return EPARSER_SUCCESS;
	}
	else if (command == SET) {
		#ifdef DEBUG
		printf("Register address: %u\r\n", regaddr);
		printf("Register value: %u\r\n", regval);
		#endif
		*((unsigned int *)regaddr) = regval;
		return EPARSER_SUCCESS;
	}
	else {
		return EPARSER_UNDEF;
	}

}
