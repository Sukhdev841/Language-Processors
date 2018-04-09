/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    loop_while = 258,
    keyword_if = 259,
    keyword_else = 260,
    tok_int = 261,
    tok_float = 262,
    tok_double = 263,
    tok_bool = 264,
    tok_switch_stmt = 265,
    tok_case = 266,
    tok_default = 267,
    tok_colon = 268,
    tok_break = 269,
    var = 270,
    tok_num = 271,
    binary_oper_per1 = 272,
    binary_oper_per2 = 273,
    tok_l_or = 274,
    tok_l_and = 275,
    tok_b_or = 276,
    tok_b_xor = 277,
    tok_b_and = 278,
    tok_equal_to = 279,
    tok_not_equal_to = 280,
    tok_less_than = 281,
    tok_less_eql = 282,
    tok_greater_than = 283,
    tok_greater_eql = 284,
    tok_plus = 285,
    tok_minus = 286,
    tok_mul = 287,
    tok_div = 288,
    tok_mod = 289,
    tok_b_not = 290,
    tok_l_not = 291,
    IFX = 292
  };
#endif
/* Tokens.  */
#define loop_while 258
#define keyword_if 259
#define keyword_else 260
#define tok_int 261
#define tok_float 262
#define tok_double 263
#define tok_bool 264
#define tok_switch_stmt 265
#define tok_case 266
#define tok_default 267
#define tok_colon 268
#define tok_break 269
#define var 270
#define tok_num 271
#define binary_oper_per1 272
#define binary_oper_per2 273
#define tok_l_or 274
#define tok_l_and 275
#define tok_b_or 276
#define tok_b_xor 277
#define tok_b_and 278
#define tok_equal_to 279
#define tok_not_equal_to 280
#define tok_less_than 281
#define tok_less_eql 282
#define tok_greater_than 283
#define tok_greater_eql 284
#define tok_plus 285
#define tok_minus 286
#define tok_mul 287
#define tok_div 288
#define tok_mod 289
#define tok_b_not 290
#define tok_l_not 291
#define IFX 292

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 64 "parse.y" /* yacc.c:1909  */

	char *val;

#line 132 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
