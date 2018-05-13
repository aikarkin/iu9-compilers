#define ERROR -1
#define TAG_IDENT 1
#define TAG_DIRECTIVE 2

char *tag_names[] = {
    "TAG_IDENT",
    "TAG_DIRECTIVE"
};

struct Position {
	int pos;
	int line;
	int index;
};

struct Fragment {
	struct Position starting, following;
};

typedef struct Fragment YYLTYPE;

struct Token {
	int tag;
	char *val;
};

typedef struct Token YYSTYPE;

int continued;
struct Position cur;


#define YY_USER_ACTION					\
    {   								\
        int i;  						\
        if (!  continued)   			\
            yylloc->starting = cur;    	\
        continued = 0;  				\
        								\
		for (i = 0; i < yyleng; i++)    \
		{   							\
			if (yytext[i] == '\n') 		\
            {   						\
                cur.line ++;    		\
                cur.pos = 1;    		\
            }   						\
			else    					\
			    cur.pos ++; 			\
			cur.index ++;   			\
		}   							\
    									\
		yylloc ->following = cur;   	\
    }									\

