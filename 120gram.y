/*
 * Grammar for 120++, a subset of C++ used in CS 120 at University of Idaho
 *
 * Adaptation by Clinton Jeffery, with help from Matthew Brown, Ranger
 * Adams, and Shea Newton.
 *
 * Based on Sandro Sigala's transcription of the ISO C++ 1996 draft standard.
 *
 */

/*	$Id: parser.y,v 1.3 1997/11/19 15:13:16 sandro Exp $	*/

/*
 * Copyright (c) 1997 Sandro Sigala <ssigala@globalnet.it>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * ISO C++ parser.
 *
 * Based on the ISO C++ draft standard of December '96.
 */

%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tree.h"
#include "nonterms.h"
//#include "token.h

extern int lineno;
extern char *yytext;
 #define YYDEBUG 1
int yydebug=0;
struct tree *root;

static void yyerror(char *);
extern int yylex(void);

extern int typenametable_insert(char *s, int cat);
extern int typenametable_lookup(char *s);

/*
 * warning, this %union is not what you need, it is for demonstration purposes.
 */
%}
%union {
  struct tree *node;
  //token *toke;

  }

  %token <node> ELLIPSIS
  %token <node> ALIGNAS
  %token <node> ALIGNOF
  %token <node> ASM
  %token <node> AUTO
  %token <node> BOOL
  %token <node> BREAK
  %token <node> CASE
  %token <node> CATCH
  %token <node> CHAR
  %token <node> CHAR16_T
  %token <node> CHAR32_T
  %token <node> CLASS
  %token <node> CLASS_NAME
  %token <node> CONST
  %token <node> CONST_CAST
  %token <node> CONSTEXPR
  %token <node> CONTINUE
  %token <node> DECLTYPE
  %token <node> DEFAULT
  %token <node> DELETE
  %token <node> DO
  %token <node> DOUBLE
  %token <node> DYNAMIC_CAST
  %token <node> ELSE
  %token <node> ENUM
  %token <node> ENUM_NAME
  %token <node> EXPLICIT
  %token <node> EXPORT
  %token <node> EXTERN
  %token <node> FALSE
  %token <node> FLOAT
  %token <node> FOR
  %token <node> FRIEND
  %token <node> GOTO
  %token <node> IF
  %token <node> INLINE
  %token <node> INT
  %token <node> LONG
  %token <node> MUTABLE
  %token <node> NAMESPACE
  %token <node> NAMESPACE_NAME
  %token <node> NEW
  %token <node> NOEXCEPT
  %token <node> NULLPTR
  %token <node> OPERATOR
  %token <node> PRIVATE
  %token <node> PROTECTED
  %token <node> PUBLIC
  %token <node> REGISTER
  %token <node> REINTERPRET_CAST
  %token <node> RETURN
  %token <node> SHORT
  %token <node> SIGNED
  %token <node> SIZEOF
  %token <node> STATIC
  %token <node> STATIC_ASSERT
  %token <node> STATIC_CAST
  %token <node> STRUCT
  %token <node> SWITCH
  %token <node> TEMPLATE
  %token <node> TEMPLATE_NAME
  %token <node> THIS
  %token <node> THREAD_LOCAL
  %token <node> THROW
  %token <node> TRUE
  %token <node> TRY
  %token <node> TYPEDEF
  %token <node> TYPEID
  %token <node> TYPENAME
  %token <node> UNION
  %token <node> UNSIGNED
  %token <node> USING
  %token <node> VIRTUAL
  %token <node> VOID
  %token <node> VOLATILE
  %token <node> WCHAR_T
  %token <node> WHILE
  %token <node> ICON
  %token <node> CCON
  %token <node> FCON
  %token <node> STRING
  %token <node> SRASN
  %token <node> PLASN
  %token <node> MIASN
  %token <node> MUASN
  %token <node> DIASN
  %token <node> MOASN
  %token <node> ANASN
  %token <node> ERASN
  %token <node> ORASN
  %token <node> SHR
  %token <node> SHL
  %token <node> INCOP
  %token <node> DECOP
  %token <node> FOLLOW
  %token <node> ANDAND
  %token <node> OROR
  %token <node> LE
  %token <node> SLASN
  %token <node> GE
  %token <node> EQ
  %token <node> NE
  %token <node> SM
  %token <node> LC
  %token <node> RC
  %token <node> CM
  %token <node> COLON
  %token <node> ASN
  %token <node> LP
  %token <node> RP
  %token <node> LB
  %token <node> RB
  %token <node> DOT
  %token <node> AND
  %token <node> BANG
  %token <node> NOT
  %token <node> MINUS
  %token <node> PLUS
  %token <node> MUL
  %token <node> DIV
  %token <node> MOD
  %token <node> LT
  %token <node> GT
  %token <node> ER
  %token <node> OR
  %token <node> QUEST
  %token <node> IDENTIFIER
  %token <node> TYPEDEF_NAME
  %token <node> BAD_TOKEN
  %token <node> COMMENT
  %token <node> COLONCOLON
  %token <node> DOTSTAR
  %token <node> ARROWSTAR




  %type <node>  typedef_name
  %type <node>  class_name
  %type <node>  template_name
  %type <node>  identifier
  %type <node>  literal
  %type <node>  integer_literal
  %type <node>  character_literal
  %type <node>  floating_literal
  %type <node>  string_literal
  %type <node>  boolean_literal
  %type <node>  translation_unit
  %type <node>  primary_expression
  %type <node>  id_expression
  %type <node>  unqualified_id
  %type <node>  qualified_id
  %type <node>  nested_name_specifier
  %type <node>  postfix_expression
  %type <node>  expression_list
  %type <node>  unary_expression
  %type <node>  unary_operator
  %type <node>  new_expression
  %type <node>  new_placement
  %type <node>  new_type_id
  %type <node>  new_declarator
  %type <node>  direct_new_declarator
  %type <node>  new_initializer
  %type <node>  delete_expression
  %type <node>  cast_expression
  %type <node>  pm_expression
  %type <node>  multiplicative_expression
  %type <node>  additive_expression
  %type <node>  shift_expression
  %type <node>  relational_expression
  %type <node>  equality_expression
  %type <node>  and_expression
  %type <node>  exclusive_or_expression
  %type <node>  inclusive_or_expression
  %type <node>  logical_and_expression
  %type <node>  logical_or_expression
  %type <node>  conditional_expression
  %type <node>  assignment_expression
  %type <node>  assignment_operator
  %type <node>  expression
  %type <node>  constant_expression
  %type <node>  statement
  %type <node>  labeled_statement
  %type <node>  expression_statement
  %type <node>  compound_statement
  %type <node>  statement_seq
  %type <node>  selection_statement
  %type <node>  condition
  %type <node>  iteration_statement
  %type <node>  for_init_statement
  %type <node>  jump_statement
  %type <node>  declaration_statement
  %type <node>  declaration_seq
  %type <node>  declaration
  %type <node>  block_declaration
  %type <node>  simple_declaration
  %type <node>  decl_specifier
  %type <node>  decl_specifier_seq
  %type <node>  storage_class_specifier
  %type <node>  function_specifier
  %type <node>  type_specifier
  %type <node>  simple_type_specifier
  %type <node>  type_name
  %type <node>  elaborated_type_specifier
  %type <node>  enum_name
  %type <node>  enum_specifier
  %type <node>  enumerator_list
  %type <node>  enumerator_definition
  %type <node>  enumerator
  %type <node>  namespace_name
  %type <node>  original_namespace_name
  %type <node>  namespace_definition
  %type <node>  named_namespace_definition
  %type <node>  original_namespace_definition
  %type <node>  extension_namespace_definition
  %type <node>  unnamed_namespace_definition
  %type <node>  namespace_body
  //%type <node>  namespace_alias
  %type <node>  namespace_alias_definition
  %type <node>  qualified_namespace_specifier
  %type <node>  using_declaration
  %type <node>  using_directive
  %type <node>  asm_definition
  %type <node>  linkage_specification
  %type <node>  init_declarator_list
  %type <node>  init_declarator
  %type <node>  declarator
  %type <node>  direct_declarator
  %type <node>  ptr_operator
  %type <node>  cv_qualifier_seq
  %type <node>  cv_qualifier
  %type <node>  declarator_id
  %type <node>  type_id
  %type <node>  type_specifier_seq
  %type <node>  abstract_declarator
  %type <node>  direct_abstract_declarator
  %type <node>  parameter_declaration_clause
  %type <node>  parameter_declaration_list
  %type <node>  parameter_declaration
  %type <node>  function_definition
  %type <node>  function_body
  %type <node>  initializer
  %type <node>  initializer_clause
  %type <node>  initializer_list
  %type <node>  class_specifier
  %type <node>  class_head
  %type <node>  class_key
  %type <node>  member_specification
  %type <node>  member_declaration
  %type <node>  member_declarator_list
  %type <node>  member_declarator
  //%type <node>  pure_specifier
  %type <node>  constant_initializer
  %type <node>  base_clause
  %type <node>  base_specifier_list
  %type <node>  base_specifier
  %type <node>  access_specifier
  %type <node>  conversion_function_id
  %type <node>  conversion_type_id
  %type <node>  conversion_declarator
  %type <node>  ctor_initializer
  %type <node>  mem_initializer_list
  %type <node>  mem_initializer
  %type <node>  mem_initializer_id
  %type <node>  operator_function_id
  %type <node>  operator
  %type <node>  template_declaration
  %type <node>  template_parameter_list
  %type <node>  template_parameter
  %type <node>  type_parameter
  %type <node>  template_id
  %type <node>  template_argument_list
  %type <node>  template_argument
  %type <node>  explicit_instantiation
  %type <node>  explicit_specialization
  %type <node>  try_block
  %type <node>  function_try_block
  %type <node>  handler_seq
  %type <node>  handler
  %type <node>  exception_declaration
  %type <node>  throw_expression
  %type <node>  exception_specification
  %type <node>  type_id_list
  %type <node>  declaration_seq_opt
  %type <node>  nested_name_specifier_opt
  %type <node>  expression_list_opt
  %type <node>  COLONCOLON_opt
  %type <node>  new_placement_opt
  %type <node>  new_initializer_opt
  %type <node>  new_declarator_opt
  %type <node>  expression_opt
  %type <node>  statement_seq_opt
  %type <node>  condition_opt
  %type <node>  enumerator_list_opt
  %type <node>  initializer_opt
  %type <node>  constant_expression_opt
  %type <node>  abstract_declarator_opt
  %type <node>  type_specifier_seq_opt
  %type <node>  direct_abstract_declarator_opt
  %type <node>  ctor_initializer_opt
  %type <node>  COMMA_opt
  %type <node>  member_specification_opt
  %type <node>  SEMICOLON_opt
  %type <node>  conversion_declarator_opt
  %type <node>  EXPORT_opt
  %type <node>  handler_seq_opt
  %type <node>  assignment_expression_opt
  %type <node>  type_id_list_opt

%start translation_unit



%%

/*----------------------------------------------------------------------
 * Context-dependent identifiers.
 *----------------------------------------------------------------------*/
 typedef_name:
      TYPEDEF_NAME           { $$ = createTreeNode(typedef_name, 1, $1); }
    ;


 namespace_name:
      original_namespace_name           { $$ = createTreeNode(namespace_name, 1, $1); }
    ;


 original_namespace_name:
      NAMESPACE_NAME           { $$ = createTreeNode(original_namespace_name, 1, $1); }
    ;


 class_name:
      CLASS_NAME           { $$ = createTreeNode(class_name, 1, $1); }
    | template_id           { $$ = createTreeNode(class_name, 1, $1); }
    ;


 enum_name:
      ENUM_NAME           { $$ = createTreeNode(enum_name, 1, $1); }
    ;


 template_name:
      TEMPLATE_NAME           { $$ = createTreeNode(template_name, 1, $1); }
    ;


 identifier:
      IDENTIFIER            { $$ = createTreeNode(identifier, 1 , $1); }
    ;


 literal:
      integer_literal           { $$ = createTreeNode(literal, 1, $1); }
    | character_literal           { $$ = createTreeNode(literal, 1, $1); }
    | floating_literal           { $$ = createTreeNode(literal, 1, $1); }
    | string_literal           { $$ = createTreeNode(literal, 1, $1); }
    | boolean_literal           { $$ = createTreeNode(literal, 1, $1); }
    ;


 integer_literal:
      ICON           { $$ = createTreeNode(integer_literal, 1, $1); }
    ;


 character_literal:
      CCON           { $$ = createTreeNode(character_literal, 1, $1); }
    ;


 floating_literal:
      FCON           { $$ = createTreeNode(floating_literal, 1, $1); }
    ;


 string_literal:
      STRING           { $$ = createTreeNode(string_literal, 1, $1); }
    ;


 boolean_literal:
      TRUE           { $$ = createTreeNode(boolean_literal, 1, $1); }
    | FALSE           { $$ = createTreeNode(boolean_literal, 1, $1); }
    ;


 translation_unit:
      declaration_seq_opt           { //root = (struct tree *) calloc(1, sizeof(struct tree));
                                      $$ = createTreeNode(translation_unit, 1, $1);
                                      root = $$; }
    ;


 primary_expression:
      literal           { $$ = createTreeNode(primary_expression, 1, $1); }
    | THIS           { $$ = createTreeNode(primary_expression, 1, $1); }
    | LP expression RP           { $$ = createTreeNode(primary_expression, 3, $1, $2, $3); }
    | id_expression           { $$ = createTreeNode(primary_expression, 1, $1); }
    ;


 id_expression:
      unqualified_id           { $$ = createTreeNode(id_expression, 1, $1); }
    | qualified_id           { $$ = createTreeNode(id_expression, 1, $1); }
    ;


 unqualified_id:
      identifier           { $$ = createTreeNode(unqualified_id, 1, $1); }
    | operator_function_id           { $$ = createTreeNode(unqualified_id, 1, $1); }
    | conversion_function_id           { $$ = createTreeNode(unqualified_id, 1, $1); }
    | NOT class_name           { $$ = createTreeNode(unqualified_id, 2, $1, $2); }
    ;


 qualified_id:
      nested_name_specifier unqualified_id           { $$ = createTreeNode(qualified_id, 2, $1, $2); }
    | nested_name_specifier TEMPLATE unqualified_id           { $$ = createTreeNode(qualified_id, 3, $1, $2, $3); }
    ;


 nested_name_specifier:
      class_name COLONCOLON nested_name_specifier           { $$ = createTreeNode(nested_name_specifier, 3, $1, $2, $3); }
    | namespace_name COLONCOLON nested_name_specifier           { $$ = createTreeNode(nested_name_specifier, 3, $1, $2, $3); }
    | class_name COLONCOLON           { $$ = createTreeNode(nested_name_specifier, 2, $1, $2); }
    | namespace_name COLONCOLON           { $$ = createTreeNode(nested_name_specifier, 2, $1, $2); }
    ;


 postfix_expression:
      primary_expression           { $$ = createTreeNode(postfix_expression, 1, $1); }
    | postfix_expression LB expression RB           { $$ = createTreeNode(postfix_expression, 4, $1, $2, $3, $4); }
    | postfix_expression LP expression_list_opt RP           { $$ = createTreeNode(postfix_expression, 4, $1, $2, $3, $4); }
    | DOUBLE LP expression_list_opt RP           { $$ = createTreeNode(postfix_expression, 4, $1, $2, $3, $4); }
    | INT LP expression_list_opt RP           { $$ = createTreeNode(postfix_expression, 4, $1, $2, $3, $4); }
    | CHAR LP expression_list_opt RP           { $$ = createTreeNode(postfix_expression, 4, $1, $2, $3, $4); }
    | BOOL LP expression_list_opt RP           { $$ = createTreeNode(postfix_expression, 4, $1, $2, $3, $4); }
    | postfix_expression DOT TEMPLATE COLONCOLON id_expression           { $$ = createTreeNode(postfix_expression, 5, $1, $2, $3, $4, $5); }
    | postfix_expression DOT TEMPLATE id_expression           { $$ = createTreeNode(postfix_expression, 4, $1, $2, $3, $4); }
    | postfix_expression DOT COLONCOLON id_expression           { $$ = createTreeNode(postfix_expression, 4, $1, $2, $3, $4); }
    | postfix_expression DOT id_expression           { $$ = createTreeNode(postfix_expression, 3, $1, $2, $3); }
    | postfix_expression FOLLOW TEMPLATE COLONCOLON id_expression           { $$ = createTreeNode(postfix_expression, 5, $1, $2, $3, $4, $5); }
    | postfix_expression FOLLOW TEMPLATE id_expression           { $$ = createTreeNode(postfix_expression, 4, $1, $2, $3, $4); }
    | postfix_expression FOLLOW COLONCOLON id_expression           { $$ = createTreeNode(postfix_expression, 4, $1, $2, $3, $4); }
    | postfix_expression FOLLOW id_expression           { $$ = createTreeNode(postfix_expression, 3, $1, $2, $3); }
    | postfix_expression INCOP           { $$ = createTreeNode(postfix_expression, 2, $1, $2); }
    | postfix_expression DECOP           { $$ = createTreeNode(postfix_expression, 2, $1, $2); }
    | DYNAMIC_CAST LT type_id GT LP expression RP           { $$ = createTreeNode(postfix_expression, 7, $1, $2, $3, $4, $5, $6, $7); }
    | STATIC_CAST LT type_id GT LP expression RP           { $$ = createTreeNode(postfix_expression, 7, $1, $2, $3, $4, $5, $6, $7); }
    | REINTERPRET_CAST LT type_id GT LP expression RP           { $$ = createTreeNode(postfix_expression, 7, $1, $2, $3, $4, $5, $6, $7); }
    | CONST_CAST LT type_id GT LP expression RP           { $$ = createTreeNode(postfix_expression, 7, $1, $2, $3, $4, $5, $6, $7); }
    | TYPEID LP expression RP           { $$ = createTreeNode(postfix_expression, 4, $1, $2, $3, $4); }
    | TYPEID LP type_id RP           { $$ = createTreeNode(postfix_expression, 4, $1, $2, $3, $4); }
    ;


 expression_list:
      assignment_expression           { $$ = createTreeNode(expression_list, 1, $1); }
    | expression_list CM assignment_expression           { $$ = createTreeNode(expression_list, 3, $1, $2, $3); }
    ;


 unary_expression:
      postfix_expression           { $$ = createTreeNode(unary_expression, 1, $1); }
    | INCOP cast_expression           { $$ = createTreeNode(unary_expression, 2, $1, $2); }
    | DECOP cast_expression           { $$ = createTreeNode(unary_expression, 2, $1, $2); }
    | MUL cast_expression           { $$ = createTreeNode(unary_expression, 2, $1, $2); }
    | AND cast_expression           { $$ = createTreeNode(unary_expression, 2, $1, $2); }
    | unary_operator cast_expression           { $$ = createTreeNode(unary_expression, 2, $1, $2); }
    | SIZEOF unary_expression           { $$ = createTreeNode(unary_expression, 2, $1, $2); }
    | SIZEOF LP type_id RP           { $$ = createTreeNode(unary_expression, 4, $1, $2, $3, $4); }
    | new_expression           { $$ = createTreeNode(unary_expression, 1, $1); }
    | delete_expression           { $$ = createTreeNode(unary_expression, 1, $1); }
    ;


 unary_operator:
      PLUS           { $$ = createTreeNode(unary_operator, 1, $1); }
    | MINUS           { $$ = createTreeNode(unary_operator, 1, $1); }
    | BANG           { $$ = createTreeNode(unary_operator, 1, $1); }
    | NOT           { $$ = createTreeNode(unary_operator, 1, $1); }
    ;


 new_expression:
      NEW new_placement_opt new_type_id new_initializer_opt           { $$ = createTreeNode(new_expression, 4, $1, $2, $3, $4); }
    | COLONCOLON NEW new_placement_opt new_type_id new_initializer_opt           { $$ = createTreeNode(new_expression, 5, $1, $2, $3, $4, $5); }
    | NEW new_placement_opt LP type_id RP new_initializer_opt           { $$ = createTreeNode(new_expression, 6, $1, $2, $3, $4, $5, $6); }
    | COLONCOLON NEW new_placement_opt LP type_id RP new_initializer_opt           { $$ = createTreeNode(new_expression, 7, $1, $2, $3, $4, $5, $6, $7); }
    ;


 new_placement:
      LP expression_list RP           { $$ = createTreeNode(new_placement, 3, $1, $2, $3); }
    ;


 new_type_id:
      type_specifier_seq new_declarator_opt           { $$ = createTreeNode(new_type_id, 2, $1, $2); }
    ;


 new_declarator:
      ptr_operator new_declarator_opt           { $$ = createTreeNode(new_declarator, 2, $1, $2); }
    | direct_new_declarator           { $$ = createTreeNode(new_declarator, 1, $1); }
    ;


 direct_new_declarator:
      LB expression RB           { $$ = createTreeNode(direct_new_declarator, 3, $1, $2, $3); }
    | direct_new_declarator LB constant_expression RB           { $$ = createTreeNode(direct_new_declarator, 4, $1, $2, $3, $4); }
    ;


 new_initializer:
      LP expression_list_opt RP           { $$ = createTreeNode(new_initializer, 3, $1, $2, $3); }
    ;


 delete_expression:
      DELETE cast_expression           { $$ = createTreeNode(delete_expression, 2, $1, $2); }
    | COLONCOLON DELETE cast_expression           { $$ = createTreeNode(delete_expression, 3, $1, $2, $3); }
    | DELETE LB RB cast_expression           { $$ = createTreeNode(delete_expression, 4, $1, $2, $3, $4); }
    | COLONCOLON DELETE LB RB cast_expression           { $$ = createTreeNode(delete_expression, 5, $1, $2, $3, $4, $5); }
    ;


 cast_expression:
      unary_expression           { $$ = createTreeNode(cast_expression, 1, $1); }
    | LP type_id RP cast_expression           { $$ = createTreeNode(cast_expression, 4, $1, $2, $3, $4); }
    ;


 pm_expression:
      cast_expression           { $$ = createTreeNode(pm_expression, 1, $1); }
    | pm_expression DOTSTAR cast_expression           { $$ = createTreeNode(pm_expression, 3, $1, $2, $3); }
    | pm_expression ARROWSTAR cast_expression           { $$ = createTreeNode(pm_expression, 3, $1, $2, $3); }
    ;


 multiplicative_expression:
      pm_expression           { $$ = createTreeNode(multiplicative_expression, 1, $1); }
    | multiplicative_expression MUL pm_expression           { $$ = createTreeNode(multiplicative_expression, 3, $1, $2, $3); }
    | multiplicative_expression DIV pm_expression           { $$ = createTreeNode(multiplicative_expression, 3, $1, $2, $3); }
    | multiplicative_expression MOD pm_expression           { $$ = createTreeNode(multiplicative_expression, 3, $1, $2, $3); }
    ;


 additive_expression:
      multiplicative_expression           { $$ = createTreeNode(additive_expression, 1, $1); }
    | additive_expression PLUS multiplicative_expression           { $$ = createTreeNode(additive_expression, 3, $1, $2, $3); }
    | additive_expression MINUS multiplicative_expression           { $$ = createTreeNode(additive_expression, 3, $1, $2, $3); }
    ;


 shift_expression:
      additive_expression           { $$ = createTreeNode(shift_expression, 1, $1); }
    | shift_expression SHL additive_expression           { $$ = createTreeNode(shift_expression, 3, $1, $2, $3); }
    | shift_expression SHR additive_expression           { $$ = createTreeNode(shift_expression, 3, $1, $2, $3); }
    ;


 relational_expression:
      shift_expression           { $$ = createTreeNode(relational_expression, 1, $1); }
    | relational_expression LT shift_expression           { $$ = createTreeNode(relational_expression, 3, $1, $2, $3); }
    | relational_expression GT shift_expression           { $$ = createTreeNode(relational_expression, 3, $1, $2, $3); }
    | relational_expression LE shift_expression           { $$ = createTreeNode(relational_expression, 3, $1, $2, $3); }
    | relational_expression GE shift_expression           { $$ = createTreeNode(relational_expression, 3, $1, $2, $3); }
    ;


 equality_expression:
      relational_expression           { $$ = createTreeNode(equality_expression, 1, $1); }
    | equality_expression EQ relational_expression           { $$ = createTreeNode(equality_expression, 3, $1, $2, $3); }
    | equality_expression NE relational_expression           { $$ = createTreeNode(equality_expression, 3, $1, $2, $3); }
    ;


 and_expression:
      equality_expression           { $$ = createTreeNode(and_expression, 1, $1); }
    | and_expression AND equality_expression           { $$ = createTreeNode(and_expression, 3, $1, $2, $3); }
    ;


 exclusive_or_expression:
      and_expression           { $$ = createTreeNode(exclusive_or_expression, 1, $1); }
    | exclusive_or_expression ER and_expression           { $$ = createTreeNode(exclusive_or_expression, 3, $1, $2, $3); }
    ;


 inclusive_or_expression:
      exclusive_or_expression           { $$ = createTreeNode(inclusive_or_expression, 1, $1); }
    | inclusive_or_expression OR exclusive_or_expression           { $$ = createTreeNode(inclusive_or_expression, 3, $1, $2, $3); }
    ;


 logical_and_expression:
      inclusive_or_expression           { $$ = createTreeNode(logical_and_expression, 1, $1); }
    | logical_and_expression ANDAND inclusive_or_expression           { $$ = createTreeNode(logical_and_expression, 3, $1, $2, $3); }
    ;


 logical_or_expression:
      logical_and_expression           { $$ = createTreeNode(logical_or_expression, 1, $1); }
    | logical_or_expression OROR logical_and_expression           { $$ = createTreeNode(logical_or_expression, 3, $1, $2, $3); }
    ;


 conditional_expression:
      logical_or_expression           { $$ = createTreeNode(conditional_expression, 1, $1); }
    | logical_or_expression QUEST expression COLON assignment_expression  { $$ = createTreeNode(conditional_expression, 5, $1, $2, $3, $4, $5); }
    ;


 assignment_expression:
      conditional_expression           { $$ = createTreeNode(assignment_expression, 1, $1); }
    | logical_or_expression assignment_operator assignment_expression           { $$ = createTreeNode(assignment_expression, 3, $1, $2, $3); }
    | throw_expression           { $$ = createTreeNode(assignment_expression, 1, $1); }
    ;


 assignment_operator:
      ASN           { $$ = createTreeNode(assignment_operator, 1, $1); }
    | MUASN           { $$ = createTreeNode(assignment_operator, 1, $1); }
    | DIASN           { $$ = createTreeNode(assignment_operator, 1, $1); }
    | MOASN           { $$ = createTreeNode(assignment_operator, 1, $1); }
    | PLASN           { $$ = createTreeNode(assignment_operator, 1, $1); }
    | MIASN           { $$ = createTreeNode(assignment_operator, 1, $1); }
    | SRASN           { $$ = createTreeNode(assignment_operator, 1, $1); }
    | SLASN           { $$ = createTreeNode(assignment_operator, 1, $1); }
    | ANASN           { $$ = createTreeNode(assignment_operator, 1, $1); }
    | ERASN           { $$ = createTreeNode(assignment_operator, 1, $1); }
    | ORASN           { $$ = createTreeNode(assignment_operator, 1, $1); }
    ;


 expression:
      assignment_expression           { $$ = createTreeNode(expression, 1, $1); }
    | expression CM assignment_expression           { $$ = createTreeNode(expression, 3, $1, $2, $3); }
    ;


 constant_expression:
      conditional_expression           { $$ = createTreeNode(constant_expression, 1, $1); }
    ;


 statement:
      labeled_statement           { $$ = createTreeNode(statement, 1, $1); }
    | expression_statement           { $$ = createTreeNode(statement, 1, $1); }
    | compound_statement           { $$ = createTreeNode(statement, 1, $1); }
    | selection_statement           { $$ = createTreeNode(statement, 1, $1); }
    | iteration_statement           { $$ = createTreeNode(statement, 1, $1); }
    | jump_statement           { $$ = createTreeNode(statement, 1, $1); }
    | declaration_statement           { $$ = createTreeNode(statement, 1, $1); }
    | try_block           { $$ = createTreeNode(statement, 1, $1); }
    ;


 labeled_statement:
      identifier COLON statement           { $$ = createTreeNode(labeled_statement, 3, $1, $2, $3); }
    | CASE constant_expression COLON statement           { $$ = createTreeNode(labeled_statement, 4, $1, $2, $3, $4); }
    | DEFAULT COLON statement           { $$ = createTreeNode(labeled_statement, 3, $1, $2, $3); }
    ;


 expression_statement:
      expression_opt SM           { $$ = createTreeNode(expression_statement, 2, $1, $2); }
    ;


 compound_statement:
      LC statement_seq_opt RC           { $$ = createTreeNode(compound_statement, 3, $1, $2, $3); }
    ;


 statement_seq:
      statement           { $$ = createTreeNode(statement_seq, 1, $1); }
    | statement_seq statement           { $$ = createTreeNode(statement_seq, 2, $1, $2); }
    ;


 selection_statement:
      IF LP condition RP statement           { $$ = createTreeNode(selection_statement, 5, $1, $2, $3, $4, $5); }
    | IF LP condition RP statement ELSE statement           { $$ = createTreeNode(selection_statement, 7, $1, $2, $3, $4, $5, $6, $7); }
    | SWITCH LP condition RP statement           { $$ = createTreeNode(selection_statement, 5, $1, $2, $3, $4, $5); }
    ;


 condition:
      expression           { $$ = createTreeNode(condition, 1, $1); }
    | type_specifier_seq declarator ASN assignment_expression           { $$ = createTreeNode(condition, 4, $1, $2, $3, $4); }
    ;


 iteration_statement:
      WHILE LP condition RP statement           { $$ = createTreeNode(iteration_statement, 5, $1, $2, $3, $4, $5); }
    | DO statement WHILE LP expression RP SM           { $$ = createTreeNode(iteration_statement, 7, $1, $2, $3, $4, $5, $6, $7); }
    | FOR LP for_init_statement condition_opt SM expression_opt RP statement           { $$ = createTreeNode(iteration_statement, 8, $1, $2, $3, $4, $5, $6, $7, $8); }
    ;


 for_init_statement:
      expression_statement           { $$ = createTreeNode(for_init_statement, 1, $1); }
    | simple_declaration           { $$ = createTreeNode(for_init_statement, 1, $1); }
    ;


 jump_statement:
      BREAK SM           { $$ = createTreeNode(jump_statement, 2, $1, $2); }
    | CONTINUE SM           { $$ = createTreeNode(jump_statement, 2, $1, $2); }
    | RETURN expression_opt SM           { $$ = createTreeNode(jump_statement, 3, $1, $2, $3); }
    | GOTO identifier SM           { $$ = createTreeNode(jump_statement, 3, $1, $2, $3); }
    ;


 declaration_statement:
      block_declaration           { $$ = createTreeNode(declaration_statement, 1, $1); }
    ;


 declaration_seq:
      declaration           { $$ = createTreeNode(declaration_seq, 1, $1); }
    | declaration_seq declaration           { $$ = createTreeNode(declaration_seq, 2, $1, $2); }
    ;


 declaration:
      block_declaration           { $$ = createTreeNode(declaration, 1, $1); }
    | function_definition           { $$ = createTreeNode(declaration, 1, $1); }
    | template_declaration           { $$ = createTreeNode(declaration, 1, $1); }
    | explicit_instantiation           { $$ = createTreeNode(declaration, 1, $1); }
    | explicit_specialization           { $$ = createTreeNode(declaration, 1, $1); }
    | linkage_specification           { $$ = createTreeNode(declaration, 1, $1); }
    | namespace_definition           { $$ = createTreeNode(declaration, 1, $1); }
    ;


 block_declaration:
      simple_declaration           { $$ = createTreeNode(block_declaration, 1, $1); }
    | asm_definition           { $$ = createTreeNode(block_declaration, 1, $1); }
    | namespace_alias_definition           { $$ = createTreeNode(block_declaration, 1, $1); }
    | using_declaration           { $$ = createTreeNode(block_declaration, 1, $1); }
    | using_directive           { $$ = createTreeNode(block_declaration, 1, $1); }
    ;


 simple_declaration:
      decl_specifier_seq init_declarator_list SM           { $$ = createTreeNode(simple_declaration, 3, $1, $2, $3); }
    | decl_specifier_seq SM           { $$ = createTreeNode(simple_declaration, 2, $1, $2); }
    ;


 decl_specifier:
      storage_class_specifier           {$$ = createTreeNode(decl_specifier, 1, $1); }
    | type_specifier           { $$ = createTreeNode(decl_specifier, 1, $1); }
    | function_specifier           { $$ = createTreeNode(decl_specifier, 1, $1); }
    | FRIEND           { $$ = createTreeNode(decl_specifier, 1, $1); }
    | TYPEDEF           { $$ = createTreeNode(decl_specifier, 1, $1); }
    ;


 decl_specifier_seq:
      decl_specifier           { $$ = createTreeNode(decl_specifier_seq, 1, $1); }
    | decl_specifier_seq decl_specifier           { $$ = createTreeNode(decl_specifier_seq, 2, $1, $2); }
    ;


 storage_class_specifier:
      AUTO           { $$ = createTreeNode(storage_class_specifier, 1, $1); }
    | REGISTER           { $$ = createTreeNode(storage_class_specifier, 1, $1); }
    | STATIC           { $$ = createTreeNode(storage_class_specifier, 1, $1); }
    | EXTERN           { $$ = createTreeNode(storage_class_specifier, 1, $1); }
    | MUTABLE           { $$ = createTreeNode(storage_class_specifier, 1, $1); }
    ;


 function_specifier:
      INLINE           { $$ = createTreeNode(function_specifier, 1, $1); }
    | VIRTUAL           { $$ = createTreeNode(function_specifier, 1, $1); }
    | EXPLICIT           { $$ = createTreeNode(function_specifier, 1, $1); }
    ;


 type_specifier:
      simple_type_specifier           { $$ = createTreeNode(type_specifier, 1, $1); }
    | class_specifier           { $$ = createTreeNode(type_specifier, 1, $1); }
    | enum_specifier           { $$ = createTreeNode(type_specifier, 1, $1); }
    | elaborated_type_specifier           { $$ = createTreeNode(type_specifier, 1, $1); }
    | cv_qualifier           { $$ = createTreeNode(type_specifier, 1, $1); }
    ;


 simple_type_specifier:
      type_name           { $$ = createTreeNode(simple_type_specifier, 1, $1); }
    | nested_name_specifier type_name           { $$ = createTreeNode(simple_type_specifier, 2, $1, $2); }
    | COLONCOLON nested_name_specifier_opt type_name           { $$ = createTreeNode(simple_type_specifier, 3, $1, $2, $3); }
    | CHAR           { $$ = createTreeNode(simple_type_specifier, 1, $1); }
    | WCHAR_T           { $$ = createTreeNode(simple_type_specifier, 1, $1); }
    | BOOL           { $$ = createTreeNode(simple_type_specifier, 1, $1); }
    | SHORT           { $$ = createTreeNode(simple_type_specifier, 1, $1); }
    | INT           { $$ = createTreeNode(simple_type_specifier, 1, $1); }
    | LONG           { $$ = createTreeNode(simple_type_specifier, 1, $1); }
    | SIGNED           { $$ = createTreeNode(simple_type_specifier, 1, $1); }
    | UNSIGNED           { $$ = createTreeNode(simple_type_specifier, 1, $1); }
    | FLOAT           { $$ = createTreeNode(simple_type_specifier, 1, $1); }
    | DOUBLE           { $$ = createTreeNode(simple_type_specifier, 1, $1); }
    | VOID           { $$ = createTreeNode(simple_type_specifier, 1, $1); }
    ;


 type_name:
      class_name           { $$ = createTreeNode(type_name, 1, $1); }
    | enum_name           { $$ = createTreeNode(type_name, 1, $1); }
    | typedef_name           { $$ = createTreeNode(type_name, 1, $1); }
    ;


 elaborated_type_specifier:
      class_key COLONCOLON nested_name_specifier identifier           { $$ = createTreeNode(elaborated_type_specifier, 4, $1, $2, $3, $4); }
    | class_key COLONCOLON identifier           { $$ = createTreeNode(elaborated_type_specifier, 3, $1, $2, $3); }
    | ENUM COLONCOLON nested_name_specifier identifier           { $$ = createTreeNode(elaborated_type_specifier, 4, $1, $2, $3, $4); }
    | ENUM COLONCOLON identifier           { $$ = createTreeNode(elaborated_type_specifier, 3, $1, $2, $3); }
    | ENUM nested_name_specifier identifier           { $$ = createTreeNode(elaborated_type_specifier, 3, $1, $2, $3); }
    | TYPENAME COLONCOLON_opt nested_name_specifier identifier           { $$ = createTreeNode(elaborated_type_specifier, 4, $1, $2, $3, $4); }
    | TYPENAME COLONCOLON_opt nested_name_specifier identifier LT template_argument_list GT           { $$ = createTreeNode(elaborated_type_specifier, 7, $1, $2, $3, $4, $5, $6, $7); }
    ;


 enum_specifier:
      ENUM identifier LC enumerator_list_opt RC           { $$ = createTreeNode(enum_specifier, 5, $1, $2, $3, $4, $5); }
    ;


 enumerator_list:
      enumerator_definition           { $$ = createTreeNode(enumerator_list, 1, $1); }
    | enumerator_list CM enumerator_definition           { $$ = createTreeNode(enumerator_list, 3, $1, $2, $3); }
    ;


 enumerator_definition:
      enumerator           { $$ = createTreeNode(enumerator_definition, 1, $1); }
    | enumerator ASN constant_expression           { $$ = createTreeNode(enumerator_definition, 3, $1, $2, $3); }
    ;


 enumerator:
      identifier           { $$ = createTreeNode(enumerator, 1, $1); }
    ;


 namespace_definition:
      named_namespace_definition           { $$ = createTreeNode(namespace_definition, 1, $1); }
    | unnamed_namespace_definition           { $$ = createTreeNode(namespace_definition, 1, $1); }
    ;


 named_namespace_definition:
      original_namespace_definition           { $$ = createTreeNode(named_namespace_definition, 1, $1); }
    | extension_namespace_definition           { $$ = createTreeNode(named_namespace_definition, 1, $1); }
    ;


 original_namespace_definition:
      NAMESPACE identifier LC namespace_body RC           { $$ = createTreeNode(original_namespace_definition, 5, $1, $2, $3, $4, $5); }
    ;


 extension_namespace_definition:
      NAMESPACE original_namespace_name LC namespace_body RC           { $$ = createTreeNode(extension_namespace_definition, 5, $1, $2, $3, $4, $5); }
    ;


 unnamed_namespace_definition:
      NAMESPACE LC namespace_body RC           { $$ = createTreeNode(unnamed_namespace_definition, 4, $1, $2, $3, $4); }
    ;


 namespace_body:
      declaration_seq_opt           { $$ = createTreeNode(namespace_body, 1, $1); }
    ;


 namespace_alias_definition:
      NAMESPACE identifier ASN qualified_namespace_specifier SM           { $$ = createTreeNode(namespace_alias_definition, 5, $1, $2, $3, $4, $5); }
    ;


 qualified_namespace_specifier:
      COLONCOLON nested_name_specifier namespace_name           { $$ = createTreeNode(qualified_namespace_specifier, 3, $1, $2, $3); }
    | COLONCOLON namespace_name           { $$ = createTreeNode(qualified_namespace_specifier, 2, $1, $2); }
    | nested_name_specifier namespace_name           { $$ = createTreeNode(qualified_namespace_specifier, 2, $1, $2); }
    | namespace_name           { $$ = createTreeNode(qualified_namespace_specifier, 1, $1); }
    ;


 using_declaration:
      USING TYPENAME COLONCOLON nested_name_specifier unqualified_id SM           { $$ = createTreeNode(using_declaration, 6, $1, $2, $3, $4, $5, $6); }
    | USING TYPENAME nested_name_specifier unqualified_id SM           { $$ = createTreeNode(using_declaration, 5, $1, $2, $3, $4, $5); }
    | USING COLONCOLON nested_name_specifier unqualified_id SM           { $$ = createTreeNode(using_declaration, 5, $1, $2, $3, $4, $5); }
    | USING nested_name_specifier unqualified_id SM           { $$ = createTreeNode(using_declaration, 4, $1, $2, $3, $4); }
    | USING COLONCOLON unqualified_id SM           { $$ = createTreeNode(using_declaration, 4, $1, $2, $3, $4); }
    ;


 using_directive:
      USING NAMESPACE COLONCOLON nested_name_specifier namespace_name SM           { $$ = createTreeNode(using_directive, 6, $1, $2, $3, $4, $5, $6); }
    | USING NAMESPACE COLONCOLON namespace_name SM           { $$ = createTreeNode(using_directive, 5, $1, $2, $3, $4, $5); }
    | USING NAMESPACE nested_name_specifier namespace_name SM           { $$ = createTreeNode(using_directive, 5, $1, $2, $3, $4, $5); }
    | USING NAMESPACE namespace_name SM           { $$ = createTreeNode(using_directive, 4, $1, $2, $3, $4); }
    ;


 asm_definition:
      ASM LP string_literal RP SM           { $$ = createTreeNode(asm_definition, 5, $1, $2, $3, $4, $5); }
    ;


 linkage_specification:
      EXTERN string_literal LC declaration_seq_opt RC           { $$ = createTreeNode(linkage_specification, 5, $1, $2, $3, $4, $5); }
    | EXTERN string_literal declaration           { $$ = createTreeNode(linkage_specification, 3, $1, $2, $3); }
    ;


 init_declarator_list:
      init_declarator           { $$ = createTreeNode(init_declarator_list, 1, $1); }
    | init_declarator_list CM init_declarator           { $$ = createTreeNode(init_declarator_list, 3, $1, $2, $3); }
    ;


 init_declarator:
      declarator initializer_opt           { $$ = createTreeNode(init_declarator, 2, $1, $2); }
    ;


 declarator:
      direct_declarator           { $$ = createTreeNode(declarator, 1, $1); }
    | ptr_operator declarator           { $$ = createTreeNode(declarator, 2, $1, $2); }
    ;


 direct_declarator:
      declarator_id           { $$ = createTreeNode(direct_declarator, 1, $1); }
    | direct_declarator LP parameter_declaration_clause RP cv_qualifier_seq exception_specification           { $$ = createTreeNode(direct_declarator, 6, $1, $2, $3, $4, $5, $6); }
    | direct_declarator LP parameter_declaration_clause RP cv_qualifier_seq           { $$ = createTreeNode(direct_declarator, 5, $1, $2, $3, $4, $5); }
    | direct_declarator LP parameter_declaration_clause RP exception_specification           { $$ = createTreeNode(direct_declarator, 5, $1, $2, $3, $4, $5); }
    | direct_declarator LP parameter_declaration_clause RP           { $$ = createTreeNode(direct_declarator, 4, $1, $2, $3, $4); }
    | CLASS_NAME LP parameter_declaration_clause RP           { $$ = createTreeNode(direct_declarator, 4, $1, $2, $3, $4); }
    | CLASS_NAME COLONCOLON declarator_id LP parameter_declaration_clause RP           { $$ = createTreeNode(direct_declarator, 6, $1, $2, $3, $4, $5, $6); }
    | CLASS_NAME COLONCOLON CLASS_NAME LP parameter_declaration_clause RP           { $$ = createTreeNode(direct_declarator, 6, $1, $2, $3, $4, $5, $6); }
    | direct_declarator LB constant_expression_opt RB           { $$ = createTreeNode(direct_declarator, 4, $1, $2, $3, $4); }
    | LP declarator RP           { $$ = createTreeNode(direct_declarator, 3, $1, $2, $3); }
    ;


 ptr_operator:
      MUL           { $$ = createTreeNode(ptr_operator, 1, $1); }
    | MUL cv_qualifier_seq           { $$ = createTreeNode(ptr_operator, 2, $1, $2); }
    | AND           { $$ = createTreeNode(ptr_operator, 1, $1); }
    | nested_name_specifier MUL           { $$ = createTreeNode(ptr_operator, 2, $1, $2); }
    | nested_name_specifier MUL cv_qualifier_seq           { $$ = createTreeNode(ptr_operator, 3, $1, $2, $3); }
    | COLONCOLON nested_name_specifier MUL           { $$ = createTreeNode(ptr_operator, 3, $1, $2, $3); }
    | COLONCOLON nested_name_specifier MUL cv_qualifier_seq           { $$ = createTreeNode(ptr_operator, 4, $1, $2, $3, $4); }
    ;


 cv_qualifier_seq:
      cv_qualifier           { $$ = createTreeNode(cv_qualifier_seq, 1, $1); }
    | cv_qualifier cv_qualifier_seq           { $$ = createTreeNode(cv_qualifier_seq, 2, $1, $2); }
    ;


 cv_qualifier:
      CONST           { $$ = createTreeNode(cv_qualifier, 1, $1); }
    | VOLATILE           { $$ = createTreeNode(cv_qualifier, 1, $1); }
    ;


 declarator_id:
      id_expression           { $$ = createTreeNode(declarator_id, 1, $1); }
    | COLONCOLON id_expression           { $$ = createTreeNode(declarator_id, 2, $1, $2); }
    | COLONCOLON nested_name_specifier type_name           { $$ = createTreeNode(declarator_id, 3, $1, $2, $3); }
    | COLONCOLON type_name           { $$ = createTreeNode(declarator_id, 2, $1, $2); }
    ;


 type_id:
      type_specifier_seq abstract_declarator_opt           { $$ = createTreeNode(type_id, 2, $1, $2); }
    ;


 type_specifier_seq:
      type_specifier type_specifier_seq_opt           { $$ = createTreeNode(type_specifier_seq, 2, $1, $2); }
    ;


 abstract_declarator:
      ptr_operator abstract_declarator_opt           { $$ = createTreeNode(abstract_declarator, 2, $1, $2); }
    | direct_abstract_declarator           { $$ = createTreeNode(abstract_declarator, 1, $1); }
    ;


 direct_abstract_declarator:
      direct_abstract_declarator_opt LP parameter_declaration_clause RP cv_qualifier_seq exception_specification           { $$ = createTreeNode(direct_abstract_declarator, 6, $1, $2, $3, $4, $5, $6); }
    | direct_abstract_declarator_opt LP parameter_declaration_clause RP cv_qualifier_seq           { $$ = createTreeNode(direct_abstract_declarator, 5, $1, $2, $3, $4, $5); }
    | direct_abstract_declarator_opt LP parameter_declaration_clause RP exception_specification           { $$ = createTreeNode(direct_abstract_declarator, 5, $1, $2, $3, $4, $5); }
    | direct_abstract_declarator_opt LP parameter_declaration_clause RP           { $$ = createTreeNode(direct_abstract_declarator, 4, $1, $2, $3, $4); }
    | direct_abstract_declarator_opt LB constant_expression_opt RB           { $$ = createTreeNode(direct_abstract_declarator, 4, $1, $2, $3, $4); }
    | LP abstract_declarator RP           { $$ = createTreeNode(direct_abstract_declarator, 3, $1, $2, $3); }
    ;


 parameter_declaration_clause:
      parameter_declaration_list ELLIPSIS           { $$ = createTreeNode(parameter_declaration_clause, 2, $1, $2); }
    | parameter_declaration_list           { $$ = createTreeNode(parameter_declaration_clause, 1, $1); }
    | ELLIPSIS           { $$ = createTreeNode(parameter_declaration_clause, 1, $1); }
    |     /* epsilon */          { $$ = NULL; }
    | parameter_declaration_list CM ELLIPSIS           { $$ = createTreeNode(parameter_declaration_clause, 3, $1, $2, $3); }
    ;


 parameter_declaration_list:
      parameter_declaration           { $$ = createTreeNode(parameter_declaration_list, 1, $1); }
    | parameter_declaration_list CM parameter_declaration           { $$ = createTreeNode(parameter_declaration_list, 3, $1, $2, $3); }
    ;


 parameter_declaration:
      decl_specifier_seq declarator           { $$ = createTreeNode(parameter_declaration, 2, $1, $2); }
    | decl_specifier_seq declarator ASN assignment_expression           { $$ = createTreeNode(parameter_declaration, 4, $1, $2, $3, $4); }
    | decl_specifier_seq abstract_declarator_opt           { $$ = createTreeNode(parameter_declaration, 2, $1, $2); }
    | decl_specifier_seq abstract_declarator_opt ASN assignment_expression           { $$ = createTreeNode(parameter_declaration, 4, $1, $2, $3, $4); }
    ;


 function_definition:
      declarator ctor_initializer_opt function_body           { $$ = createTreeNode(function_definition, 3, $1, $2, $3); }
    | decl_specifier_seq declarator ctor_initializer_opt function_body           { $$ = createTreeNode(function_definition-1, 4, $1, $2, $3, $4); }
    | declarator function_try_block           { $$ = createTreeNode(function_definition, 2, $1, $2); }
    | decl_specifier_seq declarator function_try_block           { $$ = createTreeNode(function_definition, 3, $1, $2, $3); }
    ;


 function_body:
      compound_statement           { $$ = createTreeNode(function_body, 1, $1); }
    ;


 initializer:
      ASN initializer_clause           { $$ = createTreeNode(initializer, 2, $1, $2); }
    | LP expression_list RP           { $$ = createTreeNode(initializer, 3, $1, $2, $3); }
    ;


 initializer_clause:
      assignment_expression           { $$ = createTreeNode(initializer_clause, 1, $1); }
    | LC initializer_list COMMA_opt RC           { $$ = createTreeNode(initializer_clause, 4, $1, $2, $3, $4); }
    | LC RC           { $$ = createTreeNode(initializer_clause, 2, $1, $2); }
    ;


 initializer_list:
      initializer_clause           { $$ = createTreeNode(initializer_list, 1, $1); }
    | initializer_list CM initializer_clause           { $$ = createTreeNode(initializer_list, 3, $1, $2, $3); }
    ;


 class_specifier:
      class_head LC member_specification_opt RC           { $$ = createTreeNode(class_specifier, 4, $1, $2, $3, $4); }
    ;


 class_head:
      class_key identifier           { $$ = createTreeNode(class_head, 2, $1, $2); typenametable_insert($2->leaf->text, CLASS_NAME);}
    | class_key identifier base_clause           { $$ = createTreeNode(class_head, 3, $1, $2, $3); }
    | class_key nested_name_specifier identifier           { $$ = createTreeNode(class_head, 3, $1, $2, $3); }
    | class_key nested_name_specifier identifier base_clause           { $$ = createTreeNode(class_head, 4, $1, $2, $3, $4); }
    ;


 class_key:
      CLASS           { $$ = createTreeNode(class_key, 1, $1); }
    | STRUCT           { $$ = createTreeNode(class_key, 1, $1); }
    | UNION           { $$ = createTreeNode(class_key, 1, $1); }
    ;


 member_specification:
      member_declaration member_specification_opt           { $$ = createTreeNode(member_specification, 2, $1, $2); }
    | access_specifier COLON member_specification_opt           { $$ = createTreeNode(member_specification-1, 3, $1, $2, $3); }
    ;


 member_declaration:
      decl_specifier_seq member_declarator_list SM           { $$ = createTreeNode(member_declaration, 3, $1, $2, $3); }
    | decl_specifier_seq SM           { $$ = createTreeNode(member_declaration, 2, $1, $2); }
    | member_declarator_list SM           { $$ = createTreeNode(member_declaration, 2, $1, $2); }
    | SM           { $$ = createTreeNode(member_declaration, 1, $1); }
    | function_definition SEMICOLON_opt           { $$ = createTreeNode(member_declaration, 2, $1, $2); }
    | qualified_id SM           { $$ = createTreeNode(member_declaration, 2, $1, $2); }
    | using_declaration           { $$ = createTreeNode(member_declaration, 1, $1); }
    | template_declaration           { $$ = createTreeNode(member_declaration, 1, $1); }
    ;


 member_declarator_list:
      member_declarator           { $$ = createTreeNode(member_declarator_list, 1, $1); }
    | member_declarator_list CM member_declarator           { $$ = createTreeNode(member_declarator_list, 3, $1, $2, $3); }
    ;


 member_declarator:
      declarator           { $$ = createTreeNode(member_declarator, 1, $1); }
//    | declarator pure_specifier           { $$ = createTreeNode(member_declarator, 2, $1, $2); }
    | declarator constant_initializer           { $$ = createTreeNode(member_declarator, 2, $1, $2); }
    | identifier COLON constant_expression           { $$ = createTreeNode(member_declarator, 3, $1, $2, $3); }
    ;


 //pure_specifier:
//      ASN '0'           { $$ = createTreeNode(pure_specifier, 2, $1, $2); }
//    ;


 constant_initializer:
      ASN constant_expression           { $$ = createTreeNode(constant_initializer, 2, $1, $2); }
    ;


 base_clause:
      COLON base_specifier_list           { $$ = createTreeNode(base_clause, 2, $1, $2); }
    ;


 base_specifier_list:
      base_specifier           { $$ = createTreeNode(base_specifier_list, 1, $1); }
    | base_specifier_list CM base_specifier           { $$ = createTreeNode(base_specifier_list, 3, $1, $2, $3); }
    ;


 base_specifier:
      COLONCOLON nested_name_specifier class_name           { $$ = createTreeNode(base_specifier, 3, $1, $2, $3); }
    | COLONCOLON class_name           { $$ = createTreeNode(base_specifier, 2, $1, $2); }
    | nested_name_specifier class_name           { $$ = createTreeNode(base_specifier, 2, $1, $2); }
    | class_name           { $$ = createTreeNode(base_specifier, 1, $1); }
    | VIRTUAL access_specifier COLONCOLON nested_name_specifier_opt class_name           { $$ = createTreeNode(base_specifier, 5, $1, $2, $3, $4, $5); }
    | VIRTUAL access_specifier nested_name_specifier_opt class_name           { $$ = createTreeNode(base_specifier, 4, $1, $2, $3, $4); }
    | VIRTUAL COLONCOLON nested_name_specifier_opt class_name           { $$ = createTreeNode(base_specifier, 4, $1, $2, $3, $4); }
    | VIRTUAL nested_name_specifier_opt class_name           { $$ = createTreeNode(base_specifier, 3, $1, $2, $3); }
    | access_specifier VIRTUAL COLONCOLON nested_name_specifier_opt class_name           { $$ = createTreeNode(base_specifier, 5, $1, $2, $3, $4, $5); }
    | access_specifier VIRTUAL nested_name_specifier_opt class_name           { $$ = createTreeNode(base_specifier, 4, $1, $2, $3, $4); }
    | access_specifier COLONCOLON nested_name_specifier_opt class_name           { $$ = createTreeNode(base_specifier, 4, $1, $2, $3, $4); }
    | access_specifier nested_name_specifier_opt class_name           { $$ = createTreeNode(base_specifier, 3, $1, $2, $3); }
    ;


 access_specifier:
      PRIVATE           { $$ = createTreeNode(access_specifier, 1, $1); }
    | PROTECTED           { $$ = createTreeNode(access_specifier, 1, $1); }
    | PUBLIC           { $$ = createTreeNode(access_specifier, 1, $1); }
    ;


 conversion_function_id:
      OPERATOR conversion_type_id           { $$ = createTreeNode(conversion_function_id, 2, $1, $2); }
    ;


 conversion_type_id:
      type_specifier_seq conversion_declarator_opt           { $$ = createTreeNode(conversion_type_id, 2, $1, $2); }
    ;


 conversion_declarator:
      ptr_operator conversion_declarator_opt           { $$ = createTreeNode(conversion_declarator, 2, $1, $2); }
    ;


 ctor_initializer:
      COLON mem_initializer_list           { $$ = createTreeNode(ctor_initializer, 2, $1, $2); }
    ;


 mem_initializer_list:
      mem_initializer           { $$ = createTreeNode(mem_initializer_list, 1, $1); }
    | mem_initializer CM mem_initializer_list           { $$ = createTreeNode(mem_initializer_list, 3, $1, $2, $3); }
    ;


 mem_initializer:
      mem_initializer_id LP expression_list_opt RP           { $$ = createTreeNode(mem_initializer, 4, $1, $2, $3, $4); }
    ;


 mem_initializer_id:
      COLONCOLON nested_name_specifier class_name           { $$ = createTreeNode(mem_initializer_id, 3, $1, $2, $3); }
    | COLONCOLON class_name           { $$ = createTreeNode(mem_initializer_id, 2, $1, $2); }
    | nested_name_specifier class_name           { $$ = createTreeNode(mem_initializer_id, 2, $1, $2); }
    | class_name           { $$ = createTreeNode(mem_initializer_id, 1, $1); }
    | identifier           { $$ = createTreeNode(mem_initializer_id, 1, $1); }
    ;


 operator_function_id:
      OPERATOR operator           { $$ = createTreeNode(operator_function_id, 2, $1, $2); }
    ;


 operator:
      NEW           { $$ = createTreeNode(operator, 1, $1); }
    | DELETE           { $$ = createTreeNode(operator, 1, $1); }
    | NEW LB RB           { $$ = createTreeNode(operator, 3, $1, $2, $3); }
    | DELETE LB RB           { $$ = createTreeNode(operator, 3, $1, $2, $3); }
    | PLUS           { $$ = createTreeNode(operator, 1, $1); }
  //  | '_'           { $$ = createTreeNode(operator, 1, $1); }
    | MUL           { $$ = createTreeNode(operator, 1, $1); }
    | DIV           { $$ = createTreeNode(operator, 1, $1); }
    | MOD           { $$ = createTreeNode(operator, 1, $1); }
    | ER           { $$ = createTreeNode(operator, 1, $1); }
    | AND           { $$ = createTreeNode(operator, 1, $1); }
    | OR           { $$ = createTreeNode(operator, 1, $1); }
    | NOT           { $$ = createTreeNode(operator, 1, $1); }
    | BANG           { $$ = createTreeNode(operator, 1, $1); }
    | ASN           { $$ = createTreeNode(operator, 1, $1); }
    | LT           { $$ = createTreeNode(operator, 1, $1); }
    | GT           { $$ = createTreeNode(operator, 1, $1); }
    | PLASN           { $$ = createTreeNode(operator, 1, $1); }
    | MIASN           { $$ = createTreeNode(operator, 1, $1); }
    | MUASN           { $$ = createTreeNode(operator, 1, $1); }
    | DIASN           { $$ = createTreeNode(operator, 1, $1); }
    | MOASN           { $$ = createTreeNode(operator, 1, $1); }
    | ERASN           { $$ = createTreeNode(operator, 1, $1); }
    | ANASN           { $$ = createTreeNode(operator, 1, $1); }
    | ORASN           { $$ = createTreeNode(operator, 1, $1); }
    | SHL           { $$ = createTreeNode(operator, 1, $1); }
    | SHR           { $$ = createTreeNode(operator, 1, $1); }
    | SRASN           { $$ = createTreeNode(operator, 1, $1); }
    | SLASN           { $$ = createTreeNode(operator, 1, $1); }
    | EQ           { $$ = createTreeNode(operator, 1, $1); }
    | NE           { $$ = createTreeNode(operator, 1, $1); }
    | LE           { $$ = createTreeNode(operator, 1, $1); }
    | GE           { $$ = createTreeNode(operator, 1, $1); }
    | ANDAND           { $$ = createTreeNode(operator, 1, $1); }
    | OROR           { $$ = createTreeNode(operator, 1, $1); }
    | INCOP           { $$ = createTreeNode(operator, 1, $1); }
    | DECOP           { $$ = createTreeNode(operator, 1, $1); }
    | CM           { $$ = createTreeNode(operator, 1, $1); }
    | ARROWSTAR           { $$ = createTreeNode(operator, 1, $1); }
    | FOLLOW           { $$ = createTreeNode(operator, 1, $1); }
    | LP RP           { $$ = createTreeNode(operator, 2, $1, $2); }
    | LB RB           { $$ = createTreeNode(operator, 2, $1, $2); }
    ;


 template_declaration:
      EXPORT_opt TEMPLATE LT template_parameter_list GT declaration           { $$ = createTreeNode(template_declaration, 6, $1, $2, $3, $4, $5, $6); }
    ;


 template_parameter_list:
      template_parameter           { $$ = createTreeNode(template_parameter_list, 1, $1); }
    | template_parameter_list CM template_parameter           { $$ = createTreeNode(template_parameter_list, 3, $1, $2, $3); }
    ;


 template_parameter:
      type_parameter           { $$ = createTreeNode(template_parameter, 1, $1); }
    | parameter_declaration           { $$ = createTreeNode(template_parameter, 1, $1); }
    ;


 type_parameter:
      CLASS identifier           { $$ = createTreeNode(type_parameter, 2, $1, $2); }
    | CLASS identifier ASN type_id           { $$ = createTreeNode(type_parameter, 4, $1, $2, $3, $4); }
    | TYPENAME identifier           { $$ = createTreeNode(type_parameter, 2, $1, $2); }
    | TYPENAME identifier ASN type_id           { $$ = createTreeNode(type_parameter, 4, $1, $2, $3, $4); }
    | TEMPLATE LT template_parameter_list GT CLASS identifier           { $$ = createTreeNode(type_parameter, 6, $1, $2, $3, $4, $5, $6); }
    | TEMPLATE LT template_parameter_list GT CLASS identifier ASN template_name           { $$ = createTreeNode(type_parameter, 8, $1, $2, $3, $4, $5, $6, $7, $8); }
    ;


 template_id:
      template_name LT template_argument_list GT           { $$ = createTreeNode(template_id, 4, $1, $2, $3, $4); }
    ;


 template_argument_list:
      template_argument           { $$ = createTreeNode(template_argument_list, 1, $1); }
    | template_argument_list CM template_argument           { $$ = createTreeNode(template_argument_list, 3, $1, $2, $3); }
    ;


 template_argument:
      assignment_expression           { $$ = createTreeNode(template_argument, 1, $1); }
    | type_id           { $$ = createTreeNode(template_argument, 1, $1); }
    | template_name           { $$ = createTreeNode(template_argument, 1, $1); }
    ;


 explicit_instantiation:
      TEMPLATE declaration           { $$ = createTreeNode(explicit_instantiation, 2, $1, $2); }
    ;


 explicit_specialization:
      TEMPLATE LT GT declaration           { $$ = createTreeNode(explicit_specialization, 4, $1, $2, $3, $4); }
    ;


 try_block:
      TRY compound_statement handler_seq           { fprintf(stderr, "Syntax Error: 120++ doesn't allow try statements\n"); exit(2);/* $$ = createTreeNode(try_block, 3, $1, $2, $3);*/ }
    ;


 function_try_block:
      TRY ctor_initializer_opt function_body handler_seq           { fprintf(stderr, "Syntax Error: 120++ doesn't allow try statements\n"); exit(2); /*$$ = createTreeNode(function_try_block, 4, $1, $2, $3, $4);*/ }
    ;


 handler_seq:
      handler handler_seq_opt           { $$ = createTreeNode(handler_seq, 2, $1, $2); }
    ;


 handler:
      CATCH LP exception_declaration RP compound_statement           { $$ = createTreeNode(handler, 5, $1, $2, $3, $4, $5); }
    ;


 exception_declaration:
      type_specifier_seq declarator           { $$ = createTreeNode(exception_declaration, 2, $1, $2); }
    | type_specifier_seq abstract_declarator           { $$ = createTreeNode(exception_declaration, 2, $1, $2); }
    | type_specifier_seq           { $$ = createTreeNode(exception_declaration, 1, $1); }
    | ELLIPSIS           { $$ = createTreeNode(exception_declaration, 1, $1); }
    ;


 throw_expression:
      THROW assignment_expression_opt           { $$ = createTreeNode(throw_expression, 2, $1, $2); }
    ;


 exception_specification:
      THROW LP type_id_list_opt RP           { $$ = createTreeNode(exception_specification, 4, $1, $2, $3, $4); }
    ;


 type_id_list:
      type_id           { $$ = createTreeNode(type_id_list, 1, $1); }
    | type_id_list CM type_id           { $$ = createTreeNode(type_id_list, 3, $1, $2, $3); }
    ;


 declaration_seq_opt:
      /* epsilon */          { $$ = NULL; }
    | declaration_seq           { $$ = createTreeNode(declaration_seq_opt, 1, $1); }
    ;


 nested_name_specifier_opt:
      /* epsilon */          { $$ = NULL; }
    | nested_name_specifier           { $$ = createTreeNode(nested_name_specifier_opt, 1, $1); }
    ;


 expression_list_opt:
      /* epsilon */          { $$ = NULL; }
    | expression_list           { $$ = createTreeNode(expression_list_opt, 1, $1); }
    ;


 COLONCOLON_opt:
      /* epsilon */          { $$ = NULL; }
    | COLONCOLON           { $$ = createTreeNode(COLONCOLON_opt, 1, $1); }
    ;


 new_placement_opt:
      /* epsilon */          { $$ = NULL; }
    | new_placement           { $$ = createTreeNode(new_placement_opt, 1, $1); }
    ;


 new_initializer_opt:
      /* epsilon */          { $$ = NULL; }
    | new_initializer           { $$ = createTreeNode(new_initializer_opt, 1, $1); }
    ;


 new_declarator_opt:
      /* epsilon */          { $$ = NULL; }
    | new_declarator           { $$ = createTreeNode(new_declarator_opt, 1, $1); }
    ;


 expression_opt:
      /* epsilon */          { $$ = NULL; }
    | expression           { $$ = createTreeNode(expression_opt, 1, $1); }
    ;


 statement_seq_opt:
      /* epsilon */          { $$ = NULL; }
    | statement_seq           { $$ = createTreeNode(statement_seq_opt, 1, $1); }
    ;


 condition_opt:
      /* epsilon */          { $$ = NULL; }
    | condition           { $$ = createTreeNode(condition_opt, 1, $1); }
    ;


 enumerator_list_opt:
      /* epsilon */          { $$ = NULL; }
    | enumerator_list           { $$ = createTreeNode(enumerator_list_opt, 1, $1); }
    ;


 initializer_opt:
      /* epsilon */          { $$ = NULL; }
    | initializer           { $$ = createTreeNode(initializer_opt, 1, $1); }
    ;


 constant_expression_opt:
      /* epsilon */          { $$ = NULL; }
    | constant_expression           { $$ = createTreeNode(constant_expression_opt, 1, $1); }
    ;


 abstract_declarator_opt:
      /* epsilon */          { $$ = NULL; }
    | abstract_declarator           { $$ = createTreeNode(abstract_declarator_opt, 1, $1); }
    ;


 type_specifier_seq_opt:
      /* epsilon */          { $$ = NULL; }
    | type_specifier_seq           { $$ = createTreeNode(type_specifier_seq_opt, 1, $1); }
    ;


 direct_abstract_declarator_opt:
      /* epsilon */          { $$ = NULL; }
    | direct_abstract_declarator           { $$ = createTreeNode(direct_abstract_declarator_opt, 1, $1); }
    ;


 ctor_initializer_opt:
      /* epsilon */          { $$ = NULL; }
    | ctor_initializer           { $$ = createTreeNode(ctor_initializer_opt, 1, $1); }
    ;


 COMMA_opt:
      /* epsilon */          { $$ = NULL; }
    | CM           { $$ = createTreeNode(COMMA_opt, 1, $1); }
    ;


 member_specification_opt:
      /* epsilon */          { $$ = NULL; }
    | member_specification           { $$ = createTreeNode(member_specification_opt, 1, $1); }
    ;


 SEMICOLON_opt:
      /* epsilon */          { $$ = NULL; }
    | SM           { $$ = createTreeNode(SEMICOLON_opt, 1, $1); }
    ;


 conversion_declarator_opt:
      /* epsilon */          { $$ = NULL; }
    | conversion_declarator           { $$ = createTreeNode(conversion_declarator_opt, 1, $1); }
    ;


 EXPORT_opt:
      /* epsilon */          { $$ = NULL; }
    | EXPORT           { $$ = createTreeNode(EXPORT_opt, 1, $1); }
    ;


 handler_seq_opt:
      /* epsilon */          { $$ = NULL; }
    | handler_seq           { $$ = createTreeNode(handler_seq_opt, 1, $1); }
    ;


 assignment_expression_opt:
      /* epsilon */          { $$ = NULL; }
    | assignment_expression           { $$ = createTreeNode(assignment_expression_opt, 1, $1); }
    ;


 type_id_list_opt:
      /* epsilon */          { $$ = NULL; }
    | type_id_list           { $$ = createTreeNode(type_id_list_opt, 1, $1); }
    ;

%%

static void
yyerror(char *s)
{

	fprintf(stderr, "%d: %s\n", lineno, s);
  exit(2);
}
