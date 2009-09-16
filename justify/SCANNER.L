%top{
	#include "scanner.h"
}
NONSPACEBLOCK			[[:alnum:][:punct:]]+
%option noyywrap
%option prefix="__internal_scanner_yy"
%{
	#ifdef __TURBOC__
	char *matchedtext;
	int matchtype;
	int matchlen;
	#else
	static char *matchedtext;
	static int matchtype;
	static int matchlen;
	#endif
%}
%%
(\r\n){2,}			|
(\n){2,}			{	matchtype = MATCH_TYPE_PARAGRAPH;
					matchedtext = strdup(__internal_scanner_yytext);
					matchlen = __internal_scanner_yyleng;
					return matchtype;
				}
" "+				|
[[:space:]]			;
{NONSPACEBLOCK}			{	matchtype = MATCH_TYPE_NONSPACEBLOCK;
					matchedtext = strdup(__internal_scanner_yytext);
					matchlen = __internal_scanner_yyleng;
					return matchtype;
				}
<<EOF>>				{	matchtype = MATCH_TYPE_EOF;
					matchedtext = strdup(__internal_scanner_yytext);
					matchlen = __internal_scanner_yyleng;
					return matchtype;
				}
%%
void scanner_scanfile(FILE *file) {
	__internal_scanner_yyrestart(yyin);
	__internal_scanner_yyin = file;
}

void scanner_scanstring(const char *str) {
	__internal_scanner_yy_scan_string(str);
}

matchresult *scanner_next() {
	matchresult *ret = (matchresult *)malloc(sizeof(matchresult));
	__internal_scanner_yylex();
	ret->type = matchtype;
	ret->text = matchedtext;
	ret->len = matchlen;
	return ret;
}

void destroy_matchresult(matchresult *match) {
	free(match->text);
	free(match);
}
