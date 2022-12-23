
// Generated from expr.g4 by ANTLR 4.11.1

#pragma once


#include "antlr4-runtime.h"
#include "exprParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by exprParser.
 */
class  exprListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterProg(exprParser::ProgContext *ctx) = 0;
  virtual void exitProg(exprParser::ProgContext *ctx) = 0;

  virtual void enterStat(exprParser::StatContext *ctx) = 0;
  virtual void exitStat(exprParser::StatContext *ctx) = 0;

  virtual void enterParam(exprParser::ParamContext *ctx) = 0;
  virtual void exitParam(exprParser::ParamContext *ctx) = 0;

  virtual void enterDef(exprParser::DefContext *ctx) = 0;
  virtual void exitDef(exprParser::DefContext *ctx) = 0;

  virtual void enterExpr(exprParser::ExprContext *ctx) = 0;
  virtual void exitExpr(exprParser::ExprContext *ctx) = 0;

  virtual void enterFunc(exprParser::FuncContext *ctx) = 0;
  virtual void exitFunc(exprParser::FuncContext *ctx) = 0;


};

