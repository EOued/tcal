#include "calendar.h"
#include "ics.h"
#include <stdio.h>
#include <string.h>

extern int yylex(yyscan_t scanner);
extern int yylex_init(yyscan_t* scanner);
extern int yylex_destroy(yyscan_t scanner);
extern YY_BUFFER_STATE yy_scan_string(const char* str, yyscan_t scanner);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer, yyscan_t scanner);

event_list* test(void)
{
  event_list* l;
  void* scanner;
  char* to_parse =
      "BEGIN:VCALENDAR\nBEGIN:VEVENT\nDESCRIPTION:Filières : 1A "
      "IR\nDTEND:20251002T081500Z\nGEO:48.525750;7.737741\nLOCATION:A207 - "
      "Amphi (ILL  -CAMPUS ILLKIRCH, API - BAT A "
      "CENTRAL)\nDTSTART:20251002T063000Z\nSUMMARY:CI Analyse "
      "matricielle\nUID:806b8563-171f-4ee8-b9ab-c34c1b93a5b4@806b.org\nEND:"
      "VEVENT\nEND:VCALENDAR";
  yylex_init(&scanner);
  yyset_extra(&l, scanner);
  YY_BUFFER_STATE buf = yy_scan_string(to_parse, scanner);
  yylex(scanner);
  yy_delete_buffer(buf, scanner);
  yylex_destroy(scanner);
  return l;
}
