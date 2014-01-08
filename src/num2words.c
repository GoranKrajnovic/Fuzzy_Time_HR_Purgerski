#include "num2words.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 86

char buffer[BUFFER_SIZE];

static const char* HOURS[] = {
"dvanajst",
"jedan",
"dva",
"tri",
"cetri",
"pet",
"sest",
"sedam",
"osam",
"devet",
"deset",
"jedanajst",
"dvanajst",
"jedan",
"dva",
"tri",
"cetri",
"pet",
"sest",
"sedam",
"osam",
"devet",
"deset",
"jedanajst",
};

/* In the following array, # is replaced with the current hour. $ is replaced by the next hour (current+1 modulo 24).
 * For example, if it is 4:32 right now, # will expand to 4 while $ will expand to 5.
 */

static const char* MINUTES[] = {
/* 00 */ "ravno #",
/* 01 */ "taman proslo #",
/* 02 */ "par minuta nakon #",
/* 03 */ "par minuta nakon #",
/* 04 */ "par minuta nakon #",

/* 05 */ "# i pet",
/* 06 */ "# i pet",
/* 07 */ "# i pet",
/* 08 */ "skoro # i deset",
/* 09 */ "skoro # i deset",

/* 10 */ "ravno # i deset",
/* 11 */ "taman proslo # i deset",
/* 12 */ "skoro frtalj $",
/* 13 */ "skoro frtalj $",
/* 14 */ "skoro frtalj $",

/* 15 */ "frtalj $",
/* 16 */ "taman proslo frtalj $",
/* 17 */ "taman proslo frtalj $",
/* 18 */ "skoro # i dvajst",
/* 19 */ "skoro # i dvajst",

/* 20 */ "ravno # i dvajst",
/* 21 */ "# i dvajst",
/* 22 */ "# i dvajst",
/* 23 */ "jos malo pa pola $",
/* 24 */ "jos malo pa pola $",

/* 25 */ "prema pola $",
/* 26 */ "prema pola $",
/* 27 */ "skoro pola $",
/* 28 */ "samo kaj nije pola $",
/* 29 */ "samo kaj nije pola $",

/* 30 */ "ravno pola $",
/* 31 */ "pola $",
/* 32 */ "taman proslo pola $",
/* 33 */ "taman proslo pola $",
/* 34 */ "malo iza pola $",

/* 35 */ "pola $ i pet",
/* 36 */ "pola $ i pet",
/* 37 */ "skoro dvajst do $",
/* 38 */ "skoro dvajst do $",
/* 39 */ "samo kaj nije dvajst do $",

/* 40 */ "ravno dvajst do $",
/* 41 */ "dvajst do $",
/* 42 */ "samo kaj nije tri frtalja $",
/* 43 */ "samo kaj nije tri frtalja $",
/* 44 */ "samo kaj nije tri frtalja $",

/* 45 */ "ravno tri frtalja $",
/* 46 */ "tri frtalja $",
/* 47 */ "taman proslo tri frtalja $",
/* 48 */ "taman proslo tri frtalja $",
/* 49 */ "malo iza tri frtalja $",

/* 50 */ "ravno deset do $",
/* 51 */ "deset do $",
/* 52 */ "jos malo pa $",
/* 53 */ "jos malo pa $",
/* 54 */ "pet do $",

/* 55 */ "pet do $",
/* 56 */ "pet do $",
/* 57 */ "samo kaj nije $",
/* 58 */ "samo kaj nije $",
/* 59 */ "samo kaj nije $"
};

/* do a simple search-replace of a token in place in a string buffer without using malloc for a new buffer.
 * caveats: only replaces a single occurence of character token
 *          if resulting string is too long, will cut off the end.
 *          not thoroughly tested for boundary conditions with long strings,
 *          my longest strings were safely shorter than the buffer size.
 */
void expand_in_place(char *string, char token, const char *replacement, size_t maxlength)
{
  int pos, len, i;
  // i should be size_t probably but also needs to be negative therefore int
  // find position of token, -1 if not found (0 == found at pos 0)
  for (pos=strlen(string); pos>=-1; pos--)
  {
    if (string[pos]==token || pos<0) break;
  }

  // length of replacement string to avoid unnecesary calls to the same function
  len=strlen(replacement);

  // if there is anything to replace
  if (pos>=0 && len>0)
  {
	// first pad the end of result with \0, at position which is either maxlength or actual length, whichever is smaller
    string[(strlen(string)+len)>maxlength ? (maxlength-1) : (strlen(string)+len-1)] = '\0';

	// going backwards from end, shifting byte by byte
    for (i= (strlen(string)+len)>maxlength ? (maxlength-2) : (strlen(string)+len-2); i>=(pos+len); i--)
    {
      string[i]=string[i-len+1];
    }

	// inserting replacement string
    for (; i>=pos; i--)
    {
      string[i]=replacement[i-pos];
    }
  }
}


void fuzzy_time_to_words(int hours, int minutes, char* words, size_t length)
{
  
  memset(words, 0, length);
 
  strncpy(words, MINUTES[minutes], length);
  expand_in_place(words, '#', HOURS[hours], length);
  expand_in_place(words, '$', HOURS[ (hours+1)%24 ], length);
}

