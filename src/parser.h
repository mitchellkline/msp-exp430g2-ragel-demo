#ifndef __PARSER_H__
#define __PARSER_H__

enum eparser {
	EPARSER_SUCCESS,
	EPARSER_CS,
	EPARSER_UNDEF,
	EPARSER_RANGE
};

/*
 * Parse the receive buffer and re-initialize.
 */
enum eparser parse(char *);
#endif
