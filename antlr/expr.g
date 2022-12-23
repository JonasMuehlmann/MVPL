grammar expr;

options {
   // Only works for antlr3
   k = 0;
}

prog: def* EOF ;

ID: 'a'..'z'+;

INT: '0'..'9'+;

stat: ID '=' expr ';'
    | expr ';'
    | 'return' expr ';'
    ;

param: ID ':' ID;
def: 'fn' ID '(' (| param | param (',' param)*) ')' ID '{' stat* '}' ;

expr: ID
    | INT
    | func
    | 'not' expr
    | expr 'and' expr
    | expr 'or' expr
    | expr '+' expr
    | expr '<' expr
    | expr '>' expr
    ;

func: ID ('<' ID '>')? '(' expr (',' expr)* ')' ;

WhiteSpace: (' '|'\t'|'\n')+ {skip();};
