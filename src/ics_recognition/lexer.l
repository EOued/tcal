%option noyywrap

%{
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "y.tab.h"  

 void clean_string(char* str)
 {
   int n = strlen(str);
   char* copy = calloc(n+1, sizeof(char));
   if(!copy) return;
   int index = 0; 
   for(int i = 0; i < n; i++)
   {
     if(str[i] == '\n') continue;
     if(str[i] == ' ' && (i+1 == n || str[i+1] == ' ')) continue;
     copy[index] = str[i];
     index++;
   }
   strncpy(str, copy, index+1);
   free(copy);
   return;
 }

 void remove_key(char* str)
 {
   int n = strlen(str);
   int shift = 0;
   int colon_enc = 0;
   for(int i = 0; i < n; i++)
   {
     if(colon_enc && str[i] != ' ') break;
     if(!colon_enc && str[i] == ':') colon_enc = 1;
     shift++;
   }
   for(int i = shift; i < n; i++)
     str[i - shift] = str[i];
   str[n-shift] = '\0';
   return;
 }

   #define SET_VAL				\
   do {\
     clean_string(yytext);\
     remove_key(yytext);\
     yylval.sval = strdup(yytext);\
   } while (0)

%}

LINE ([^\n]*(\n[ \t]+)?)+\n?

%%

BEGIN:VCALENDAR {return BEG_CAL;}
BEGIN:VEVENT {return BEG_EVT;}
END:VEVENT {return END_EVT;}
END:VCALENDAR {return END_CAL;}
DESCRIPTION:{LINE} {SET_VAL; return DESC;}
SUMMARY:{LINE} {SET_VAL; return SUMM;}
LOCATION:{LINE} {SET_VAL; return LOC;}
DTSTART:{LINE} {SET_VAL; return STRT;}
DTEND:{LINE} {SET_VAL; return END;}

%%

