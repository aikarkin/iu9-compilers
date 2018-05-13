%option noyywrap bison-bridge bison-locations
%{
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "lexer_types.h"

%}

ident		[[:upper:]][[:alpha:][:digit:]\\-]*
directive	[$][[:upper:]]+
error		.

%%

[\n\t ]		;


{ident}		{
				yylval->val = yytext;
				yylval->tag = TAG_IDENT;
				return TAG_IDENT;
			}

{directive} {
				yylval->val = yytext;
				yylval->tag = TAG_DIRECTIVE;
				return TAG_DIRECTIVE;
			}
			
<<EOF>>		{
				return 0;
			}

{error}		{
				yylval->val = yytext;
				yylval->tag = ERROR;
				return ERROR;
			}

%%

void init_scanner(char *program)
{
	continued = 0;
	cur.line = 1;
	cur.pos = 1;
	cur.index = 0;
	yy_scan_string(program);
}

void print_err(char *msg)
{
	printf("Error at (%d, %d): %s\n", cur.line, cur.pos, msg);
}

char *get_src(FILE *in) {
	char *buf = (char*)malloc(1000);
	char ch;
	int i = 0;
	
	ch = fgetc(in);
	
	while(ch != EOF) {
		ch = fgetc(in);	
		if(ch != EOF)
			buf[i++] = ch;
	}
	buf[i] = '\0';
	
	return buf;
}

void print_token(YYSTYPE *value, YYLTYPE *coords) {
	printf(
		"%s (%d, %d)-(%d, %d): %s\n",
		tag_names[value->tag - 1], 
		coords->starting.line, 
		coords->starting.pos, 
		coords->following.line, 
		coords->following.pos, 
		value->val
	);
}

int main(int argc, char **argv) {	
	FILE *in = argc > 0 ? fopen(argv[1], "r") : stdin;
	
	if(in == NULL) {
		printf("error: unable to read input stream");
		return 0;
	}
	
	char *src = get_src(in);
	int tag;
	YYSTYPE value;
	YYLTYPE coords;
	
	init_scanner(src);
	
	do {
		tag = yylex(&value, &coords);
		if(tag != 0) {
			if(tag == ERROR) {
				char msg[100];
				sprintf(msg, "Invalid token - %s", value.val);
				print_err(msg);
			}
			else
				print_token(&value, &coords);
		}
	} while(tag != 0);
	
	free(src);

	return 0;
}
