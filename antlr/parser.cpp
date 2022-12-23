#include <antlr4-runtime/antlr4-common.h>
#include <antlr4-runtime/antlr4-runtime.h>

#include <iostream>
#include <sstream>
#include <string_view>
#include <vector>

#include "DiagnosticErrorListener.h"
#include "out/exprBaseListener.h"
#include "out/exprLexer.h"
#include "out/exprParser.h"

using namespace antlr4;
using namespace std::literals::string_view_literals;

// struct parameter
// {
//     std::string identifier;
//     std::string type;

//     std::string translate()
//     {
//         std::stringstream res;
//         res << identifier.data() << ": " << *type.data();

//         return res.str();
//     }
// };

// struct function
// {
//     std::string              name;
//     std::string              return_type;
//     std::vector<parameter>        parameters;
//     std::vector<std::string> body;

//     std::string translate()
//     {
//         std::stringstream target_code;

//         target_code << "fn " << name << "(";

//         target_code << parameters.front().translate();
//         for (auto it = parameters.begin() + 1; it < parameters.end() - 1; it++)
//         {
//             target_code << it->translate() << ", ";
//         }
//         target_code << parameters.back().translate();

//         target_code << ") " << return_type;

//         target_code << " {\n";
//         for (auto statement : body)
//         {
//             target_code << statement << '\n';
//         }
//         target_code << "}\n\n";

//         return target_code.str();
//     }
// };

// struct context
// {
//     std::unordered_map<std::string, function> functions;
//     std::string                              cur_function_name;
// };

// context parse_context;

// class TreeShapeListener : public exprBaseListener
// {
//  public:
//     void enterProg(exprParser::ProgContext* ctx) override
//     {
//         TreeShapeListener l;
//         for (auto def : ctx->def())
//         {
//             // def->enterRule(&l);
//         }
//     }
//     // void exitProg(exprParser::ProgContext* ctx) override
//     // {
//     //     ;
//     // }

//     void enterStat(exprParser::StatContext* ctx) override
//     {
//         parse_context.functions[parse_context.cur_function_name].body.push_back(
//             ctx->toString().data());
//     };
//     // void exitStat(exprParser::StatContext* ctx) override
//     // {
//     //     ;
//     // }
//     void enterParam(exprParser::ParamContext* ctx) override
//     {
//         parse_context.functions[parse_context.cur_function_name].parameters.push_back(
//             {ctx->ID()[0]->toString().data(), ctx->ID()[1]->toString().data()});
//     }
//     // void exitParam(exprParser::ParamContext* ctx) override
//     // {
//     //     ;
//     // }

//     void enterDef(exprParser::DefContext* ctx) override
//     {
//         std::string new_function_name                     = ctx->ID()[0]->toString().data();
//         parse_context.cur_function_name                        = new_function_name;
//         parse_context.functions[new_function_name].name        = new_function_name;
//         parse_context.functions[new_function_name].return_type = ctx->ID()[1]->toString().data();
//
//         TreeShapeListener l;
//         for (auto par : ctx->param())
//         {
//             par->enterRule(&l);
//         }

//         for (auto stat : ctx->stat())
//         {
//             stat->enterRule(&l);
//         }
//     }
//     // void exitDef(exprParser::DefContext* ctx) override
//     // {
//     //     ;
//     // }

//     // void enterExpr(exprParser::ExprContext* ctx) override
//     // {
//     //     ;
//     // }
//     // void exitExpr(exprParser::ExprContext* ctx) override
//     // {
//     //     ;
//     // }

//     // void enterFunc(exprParser::FuncContext* ctx) override
//     // {
//     //         ;
//     // }
//     // void exitFunc(exprParser::FuncContext* ctx) override
//     // {
//     //     ;
//     // }
// };


int main(int argc, const char* argv[])
{
    std::ifstream stream;
    stream.open(argv[1]);
    ANTLRInputStream  input(stream);
    exprLexer         lexer(&input);
    CommonTokenStream tokens(&lexer);
    exprParser        parser(&tokens);

    DiagnosticErrorListener l;
    parser.addErrorListener(&l);
    tree::ParseTree* tree = parser.prog();
    // parser.stat();
    // TreeShapeListener listener;
    // tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);

    // std::stringstream target_code;
    //
    // for (auto [function_name, function_decl] : parse_context.functions)
    // {
    //     target_code << function_decl.translate();
    // }

    // std::cout << target_code.str();

    return 0;
}
