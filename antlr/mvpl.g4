grammar mvpl;

globals: (package_decl | package_import | declaration | definition);
program: globals* EOF ;

WhiteSpace : (' '|'\t'|'\n')+ -> skip;
fragment Letter: 'a'..'z' | 'A'..'Z';
DecimalNumber: '0'..'9' ('_' '0'..'9')*;
ScientificDecimalNumber: DecimalNumber 'e' DecimalNumber;
FloatNumber: DecimalNumber '.' DecimalNumber;
ScientificFloatNumber:  FloatNumber 'e' DecimalNumber;
BinaryNumber: '0b' ('0' | '1')+;
OctalNumber: 'o' ('0'..'7')+;
HexNumber: '0x' ('0'..'9' | 'a'..'f' | 'A'..'F')+;
Identifier: Letter (Letter | DecimalNumber | '_')*;
fragment Escape : '\\' ( '"' | '\\' );

literal: int_lit
    | float_lit
    | bool_lit
    | StringLiteral
    | format_string_literal
    | array_literal
    | tuple_literal
    | char_literal
    ;

int_lit: DecimalNumber | ScientificDecimalNumber | BinaryNumber | OctalNumber | HexNumber;
float_lit: FloatNumber | ScientificFloatNumber;

bool_lit: 'true' | 'false';
// This should allow all types of symbols
fragment StringLiteralContent: ~('"' | '\\' | '\n' | '\r');
StringLiteral: '"' StringLiteralContent* '"';
format_string_literal: '$' '"' (StringLiteralContent* ('{' expression '}')+ StringLiteralContent*)* '"';
char_literal: '\'' .*? '\'';

array_literal: '$' '{' (expression? | expression (',' expression)+) '}';
tuple_literal: '$' '(' (expression? | expression (',' expression)+) ')';

ellipse: '...';

annotation: '@public'
    | '@private'
    | '@protected'
    | '@override'
    | '@is' ('(' (Identifier? | Identifier (',' Identifier)+) ')')?
    | '@static'
    | '@virtual'
    | '@const'
    | '@ctor'
    | '@assume' '(' (expression | (expression ';' expression)*) ')'
    | '@precondition' '(' Identifier Identifier? ')'
    | '@postcondition' '(' Identifier Identifier? ')'
    | '@weakpostcondition' '(' Identifier Identifier? ')'
    | '@invariant' '(' Identifier Identifier? ')'
    | '@pure'
    | '@weakly_pure'
    ;

declaration: annotation* (
      mut_decl
    | compvar_decl
    | func_decl
    | lambda_decl
    | ctor_decl
    | namespace_decl
);

definition: annotation* (
      var_def
    | compvar_def
    | mut_def
    | func_def
    | lambda_def
    | class_def
    | trait_def
    | enum_def
    | choice_def
    | ctor_def
    | dtor_def
 );

package_decl: 'package' Identifier ';';
package_import_block: 'import' '[' (Identifier ('.' Identifier)* ('.' '*')? ';')* ']';
package_import: package_import_block | 'import' Identifier ('.' Identifier)* ('.' '*')? ';';
namespace_decl: annotation* 'namespace' Identifier '{' globals* '}';

parameter_list_decl: (parameter_decl? | parameter_decl (',' parameter_decl)+);
parameter_decl: Identifier ':' Identifier ('=' expression | ellipse)?;

annotated_func_def: annotation* 'fn' Identifier '[' lambda_def+ ']' ;
func_def: annotated_func_def | 'fn' Identifier '(' parameter_list_decl ')' Identifier '{' statement* '}' ;

annotated_func_decl: annotation* 'fn' Identifier '[' lambda_decl+ ']';
func_decl: annotated_func_decl | 'fn' Identifier '(' parameter_list_decl ')' Identifier ';';

capture_clause: '[' (('&'? Identifier)? | ('&'? Identifier) ',' ('&'? Identifier)+) ']';
lambda_def: capture_clause '(' parameter_list_decl ')' Identifier '{' statement* '}';
lambda_decl: capture_clause '(' parameter_list_decl ')' Identifier ';';

statement: var_def
    | mut_decl
    | call ';'
    | return_statement
    | control_flow_statement
    | assignment
    | addition_assignment
    | subtraction_assignment
    | multiplication_assignment
    | division_assignment
    | modulo_assignment
    | and_assignment
    | or_assignment
    | xor_assignment
    | shift_right_assignment
    | shift_left_assignment
    | defer_statement
    | errdefer_statement
    |  with_clause? '{' statement* '}'
    | 'unreachable' ';'
    | 'break' ';'
    | 'continue' ';'
    ;

with_clause: 'with' '(' (
      (var_def? | var_def (';' var_def)+)
    | (mut_def? | mut_def (';' mut_def)+)
    | (mut_decl? | mut_decl (';' mut_decl)+)
) ')';

control_flow_statement: for_loop
    | repeat_loop
    | while_loop
    | match_statement
    | switch_statement
    | if_statement
    ;

defer_statement: 'defer' statement;
errdefer_statement: 'errdefer' statement;

for_loop: 'for' '(' (mut_decl | mut_def | Identifier) 'in' expression ')' '{' statement* '}' ('else' '{' statement* '}')?;
while_loop: with_clause? 'while' '(' expression ('do' statement)? ')' '{' statement* '}' ('else' '{' statement* '}')?;
repeat_loop:
      with_clause? 'repeat' '{' statement* '}' 'while' '(' expression ')' ('else' '{' statement* '}')? ';'
    | with_clause? 'repeat' '{' statement* '}' ';'
;

elif_statement: 'elif' '(' expression ')' '{' statement* '}';
else_statement: 'else' '{' statement* '}';
if_statement: with_clause? 'if' '(' expression ')' '{' statement* '}' (elif_statement)* (else_statement)?;
switch_statement: with_clause? 'switch' '(' expression ')' '{' (case_statement)* default_clause? '}';
match_statement: with_clause? 'match' '(' expression ')' '{' (case_statement)* default_clause? '}';
case_statement: 'case' expression ':' statement*;
default_clause: 'default' ':' statement*;

assignment: expression '=' expression ';';
addition_assignment: expression '+=' expression ';';
subtraction_assignment: expression '-=' expression ';';
multiplication_assignment: expression '*=' expression ';';
division_assignment: expression '/=' expression ';';
modulo_assignment: expression '%=' expression ';';
and_assignment: expression '&=' expression ';';
or_assignment: expression '|=' expression ';';
xor_assignment: expression '^=' expression ';';
shift_right_assignment: expression '>>=' expression ';';
shift_left_assignment: expression '<<=' expression ';';

var_def_block: 'var' '['( Identifier ':' Identifier '=' expression ';')+  ']';
var_def: var_def_block | 'var' Identifier ':' Identifier '=' expression ';';

mut_decl_block: 'mut' '[' (Identifier ':' Identifier ';')+ ']';
mut_decl: mut_decl_block | 'mut' Identifier ':' Identifier ';';

mut_def_block: 'mut' '[' (Identifier ':' Identifier '=' expression ';')+ ']';
mut_def: mut_def_block | 'mut' Identifier ':' Identifier '=' expression ';';

compvar_decl_block: 'compvar' '[' (Identifier ':' Identifier ';')+ ']';
compvar_decl: compvar_decl_block | 'compvar' Identifier ':' Identifier ';';

compvar_def_block: 'compvar' '[' (Identifier ':' Identifier '=' expression ';')+ ']';
compvar_def: compvar_def_block | 'compvar' Identifier ':' Identifier '=' expression ';';

return_statement: 'return' expression? ';';

trait_def: 'trait' Identifier '{' (func_decl | requires_clause)* '}';

class_def: 'class' Identifier '{' (declaration | definition | impl_clause)* '}';

impl_clause: annotation* 'impl' Identifier '{' (annotation* func_def)* '}';

annotated_ctor_def : '@ctor' '[' lambda_def* ']';
ctor_def: annotated_ctor_def |  '@ctor' lambda_def;

annotated_ctor_decl : '@ctor' '[' lambda_decl* ']';
ctor_decl: annotated_ctor_decl |  '@ctor' lambda_decl;

dtor_def: '@dtor' lambda_def;

requires_clause: 'requires' '{' expression '}';

choice_member: Identifier ('{' Identifier ':' Identifier '}')?;
choice_def: 'choice' Identifier '{' (choice_member | choice_member (',' choice_member)+) '}';

enum_member: Identifier ('=' expression)?;
enum_def: 'enum' Identifier '{' ( enum_member |  enum_member (',' enum_member)+) '}';

trailing_unary_operator: '&' | '++' | '--' ;
leading_unary_operator:  '*' | '++' | '--' ;

binary_operator: '&&'
    | '||'
    | '^'
    | '<<'
    | '>>'
    | '&&'
    | '||'
    | '+'
    | '-'
    | '*'
    | '/'
    | '%'
    | '=='
    | '!='
    | '<'
    | '<='
    | '>'
    | '>='
    | '<=>'
    | '::'
    | '.'
    ;

expression: Identifier
    | literal
    | call
    | expression trailing_unary_operator
    | leading_unary_operator expression
    | expression binary_operator expression
    | '(' expression ')'
    | lambda_def
    | 'try' expression
    | expression 'else' expression
    | expression 'elsedo' statement
    | expression 'if' expression 'else' expression
    // Tuple unpack and subscript
    | expression '[' (expression | expression (',' expression)+) ']'
    // Slicing operator
    | expression '[' (expression)? ':' (expression)? (':' expression)? ']'
    // Array comprehension
    | '$' '{' Identifier ':' Identifier 'for' expression 'in' expression '}'
    // Generator expression
    | '$' '[' Identifier ':' Identifier 'for' expression 'in' expression ']'
    ;

parameter_pass: (Identifier '=' expression | expression);
template_instantiation: ':' '<' (expression? | expression (expression ',')+) '>';
call: Identifier template_instantiation? '(' (parameter_pass? | parameter_pass ( ',' parameter_pass)+) ')' ;