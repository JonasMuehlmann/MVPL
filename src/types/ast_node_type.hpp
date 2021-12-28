// Copyright © 2021 Jonas Muehlmann
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
// OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#ifndef SRC_TYPES_AST_NODE_TYPE_HPP_
#define SRC_TYPES_AST_NODE_TYPE_HPP_
enum class ast_node_type
{
    EXPRESSION,               // x + 1
    FUNCTION_DECLARATION,     // function x(a,b,c){return 1;}
    FUNCTION_SIGNATURE,       // function x(a,b,c)
    PARAMETER_DECLARATION,    // a,b,c
    PRODECURE_DECLARATION,    // procedure x(){}
    VAR_DECLARATION,          // x
    VAR_INITIALIZATION,       // let x = 5;
    VAR_ASSIGNMENT,           // x = 5;
    CALL,                     // f(2, 5)
    PARAMETER_PASS,           // 2,5
    BLOCK,                    // { x = 5; y = x;}//
    CONTROL_BLOCK,            // while(x){x--;}
    CONTROL_HEAD,             // while(x)
};
#endif    // SRC_TYPES_AST_NODE_TYPE_HPP_
