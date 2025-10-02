%option noyywrap
%option noyy_top_state
%option nounput
%option noinput
%option reentrant

%{
  #include "calendar.h"
  #include "macro.h"
  #include <string.h>
  #include <stdlib.h>

  #define CPY(ptr, src) int n = strlen(src); ptr = calloc(n+1, sizeof(char)); strncpy(ptr, src, n);

  event_list* l;
  char* dtstart;
  char* dtend;
  char* summary;
  char* description;
  char* location;


  void clear_key(char* line){
    int n = strlen(line);
    int i;
     for(i =
 1; i < n; i++)
    {
      if(line[i-1] == ':') break;
    }
    // Suppose we have not more than 246 characters.
    for(int j = i; j < n; j++)
      line[j-i] = line[j];
    line[n-i-1] = '\0';
  }
%}

%%

BEGIN\:VCALENDAR {
  l = calloc(1, sizeof(event_list));
  l->e = calloc(2, sizeof(event));
  l->capacity = 2;
  l->size = 0;

 }
BEGIN\:VEVENT\n {FREE(dtstart);
		 FREE(dtend);
		 FREE(summary);
		 FREE(description);
		 FREE(location);}
DESCRIPTION\:.*\n { clear_key(yytext); CPY(description, yytext);}
LOCATION\:.*\n {clear_key(yytext); CPY(location, yytext);}
SUMMARY\:.*\n {clear_key(yytext); CPY(summary, yytext);}
DTSTART\:.*\n {clear_key(yytext); CPY(dtstart, yytext);}
DTEND\:.*\n {clear_key(yytext); CPY(dtend, yytext);}
END\:VEVENT\n {
    REALLOC(l->size, l->capacity, sizeof(event), l->e);
    l->e[l->size++] = initCalendar(dtstart, dtend, summary, description, location);}
END\:VCALENDAR {FREE(dtstart);
		FREE(dtend);
		FREE(summary);
		FREE(description);
		FREE(location);
		event_list** extra = (event_list**)yyget_extra(yyscanner);
		*extra = l;
		return 0;}
[.*] {}

%%
