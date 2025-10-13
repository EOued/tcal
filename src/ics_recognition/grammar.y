%{
  #include <stdio.h>
  int yyerror(char* s);
  int yylex(void);
%}

%token <sval> BEG_CAL END_CAL BEG_EVT END_EVT DESC LOC SUMM STRT END
%type <sval> LINE
%union {
    char *sval;
}
%start FILE;

%%

FILE : BEG_CAL EVENTS END_CAL;
EVENTS : EVENT
| EVENT EVENTS;
EVENT: BEG_EVT LINES END_EVT;
LINES : { $$ = NULL; }
| LINE LINES;
LINE: DESC {printf("%s\n", $1);}
| LOC
| SUMM
| STRT
| END {printf("\n%s\n", $1);};

%%

int main(void)
{
  yyparse();
  return 0;
}

int yyerror(char* s)
{
  printf("Error %s\n", s);
  return 1;
}
