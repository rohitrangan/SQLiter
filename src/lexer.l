%{
#include "parser.tab.hpp"
#include <iostream>
using namespace std;
%}

%option noyywrap
digit [0-9]
sel [s|S][e|E][l|L][e|E][c|C][t|T]
use [u|U][s|S][e|E]
create [c|C][r|R][e|E][a|A][t|T][e|E]
dat [d|D][a|A][t|T][a|A][b|B][a|A][s|S][e|E]
tab [t|T][a|A][b|B][l|L][e|E]
show [s|S][h|H][o|O][w|W]
from [f|F][r|R][o|O][m|M]
where [w|W][h|H][e|E][r|R][e|E]
comp "<""="|">""="|"<"|">"|"="|"=""="|"<"">"|"!""="
star ["*"]
drop [d|D][r|R][o|O][p|P]
integer [i|I][n|N][t|T][e|E][g|G][e|E][r|R]
decimal [d|D][o|O][u|U][b|B][l|L][e|E]
character [c|C][h|H][a|A][r|R]
insert [i|I][n|N][s|S][e|E][r|R][t|T]
into [i|I][n|N][t|T][o|O]
values [v|V][a|A][l|L][u|U][e|E][s|S]
update [u|U][p|P][d|D][a|A][t|T][e|E]
set [s|S][e|E][t|T]
delete [d|D][e|E][l|L][e|E][t|T][e|E]
quit [q|Q][u|U][i|I][t|T]
%%

{sel}                            {return SELECT;}
{use}                            {return USE;}
{create}                         {return CREATE;}
{dat}                            {return DATABASE;}
{tab}                            {return TABLE;}    
{show}                           {return SHOW;}
{dat}[s|S]                       {return DATABASES;}
{tab}[s|S]                       {return TABLES;}
{from}                           {return FROM;}
{star}                           {return STAR;}
{where}                          {return WHERE;}
{drop}                           {return DROP;}
";"                              {return STOP; }
","                              {return COMMA;}
{insert}                         {return INSERT;}
{into}                           {return INTO;}
{values}                         {return VALUES;}
{update}                         {return UPDATE;}
{set}                            {return SET;}
{delete}                         {return DELETE;}
{quit}                           {return QUIT;}
[0-9]+                           { yylval.int_val = atoi(yytext); return NUM;}
{integer}                        {yylval.a_val=yytext;return INTEGER;}
{decimal}                        {yylval.a_val=yytext;return DECIMAL;}
{character}                      {yylval.a_val=yytext;return CHARACTER;}
{comp}                           {yylval.a_val=yytext;return COMPARISION;}
[a-zA-Z0-9_]+                    {yylval.a_val=yytext;return NAME;}
"("                              { yylval.a_val=yytext;return LP; }
")"                              { yylval.a_val=yytext;return RP; }
[ \t\n]+                         { }
{digit}*"."{digit}+              { yylval.fl_val=atof(yytext); return DOUBLE;}
.                                { cerr << "Unrecognized token!" << endl; exit(1); }
%%

