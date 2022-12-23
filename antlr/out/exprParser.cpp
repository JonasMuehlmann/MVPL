
// Generated from expr.g4 by ANTLR 4.11.1


#include "exprListener.h"

#include "exprParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct ExprParserStaticData final {
  ExprParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  ExprParserStaticData(const ExprParserStaticData&) = delete;
  ExprParserStaticData(ExprParserStaticData&&) = delete;
  ExprParserStaticData& operator=(const ExprParserStaticData&) = delete;
  ExprParserStaticData& operator=(ExprParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag exprParserOnceFlag;
ExprParserStaticData *exprParserStaticData = nullptr;

void exprParserInitialize() {
  assert(exprParserStaticData == nullptr);
  auto staticData = std::make_unique<ExprParserStaticData>(
    std::vector<std::string>{
      "prog", "stat", "param", "def", "expr", "func"
    },
    std::vector<std::string>{
      "", "'='", "';'", "'return'", "':'", "'fn'", "'('", "')'", "'{'", 
      "'}'", "'not'", "'and'", "'or'", "'+'", "'<'", "'>'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "ID", 
      "INT", "WhiteSpace"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,18,96,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,1,0,5,0,14,
  	8,0,10,0,12,0,17,9,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  	1,1,1,1,3,1,33,8,1,1,2,1,2,1,2,1,2,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,5,
  	3,47,8,3,10,3,12,3,50,9,3,1,3,1,3,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,
  	1,4,3,4,64,8,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,
  	4,1,4,5,4,81,8,4,10,4,12,4,84,9,4,1,5,1,5,1,5,1,5,3,5,90,8,5,1,5,1,5,
  	1,5,1,5,1,5,0,1,8,6,0,2,4,6,8,10,0,0,103,0,15,1,0,0,0,2,32,1,0,0,0,4,
  	34,1,0,0,0,6,38,1,0,0,0,8,63,1,0,0,0,10,85,1,0,0,0,12,14,3,6,3,0,13,12,
  	1,0,0,0,14,17,1,0,0,0,15,13,1,0,0,0,15,16,1,0,0,0,16,18,1,0,0,0,17,15,
  	1,0,0,0,18,19,5,0,0,1,19,1,1,0,0,0,20,21,5,16,0,0,21,22,5,1,0,0,22,23,
  	3,8,4,0,23,24,5,2,0,0,24,33,1,0,0,0,25,26,3,8,4,0,26,27,5,2,0,0,27,33,
  	1,0,0,0,28,29,5,3,0,0,29,30,3,8,4,0,30,31,5,2,0,0,31,33,1,0,0,0,32,20,
  	1,0,0,0,32,25,1,0,0,0,32,28,1,0,0,0,33,3,1,0,0,0,34,35,5,16,0,0,35,36,
  	5,4,0,0,36,37,5,16,0,0,37,5,1,0,0,0,38,39,5,5,0,0,39,40,5,16,0,0,40,41,
  	5,6,0,0,41,42,3,4,2,0,42,43,5,7,0,0,43,44,5,16,0,0,44,48,5,8,0,0,45,47,
  	3,2,1,0,46,45,1,0,0,0,47,50,1,0,0,0,48,46,1,0,0,0,48,49,1,0,0,0,49,51,
  	1,0,0,0,50,48,1,0,0,0,51,52,5,9,0,0,52,7,1,0,0,0,53,54,6,4,-1,0,54,64,
  	5,16,0,0,55,64,5,17,0,0,56,64,3,10,5,0,57,58,5,10,0,0,58,64,3,8,4,7,59,
  	60,5,6,0,0,60,61,3,8,4,0,61,62,5,7,0,0,62,64,1,0,0,0,63,53,1,0,0,0,63,
  	55,1,0,0,0,63,56,1,0,0,0,63,57,1,0,0,0,63,59,1,0,0,0,64,82,1,0,0,0,65,
  	66,10,6,0,0,66,67,5,11,0,0,67,81,3,8,4,7,68,69,10,5,0,0,69,70,5,12,0,
  	0,70,81,3,8,4,6,71,72,10,4,0,0,72,73,5,13,0,0,73,81,3,8,4,5,74,75,10,
  	3,0,0,75,76,5,14,0,0,76,81,3,8,4,4,77,78,10,2,0,0,78,79,5,15,0,0,79,81,
  	3,8,4,3,80,65,1,0,0,0,80,68,1,0,0,0,80,71,1,0,0,0,80,74,1,0,0,0,80,77,
  	1,0,0,0,81,84,1,0,0,0,82,80,1,0,0,0,82,83,1,0,0,0,83,9,1,0,0,0,84,82,
  	1,0,0,0,85,89,5,16,0,0,86,87,5,14,0,0,87,88,5,16,0,0,88,90,5,15,0,0,89,
  	86,1,0,0,0,89,90,1,0,0,0,90,91,1,0,0,0,91,92,5,6,0,0,92,93,3,8,4,0,93,
  	94,5,7,0,0,94,11,1,0,0,0,7,15,32,48,63,80,82,89
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  exprParserStaticData = staticData.release();
}

}

exprParser::exprParser(TokenStream *input) : exprParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

exprParser::exprParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  exprParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *exprParserStaticData->atn, exprParserStaticData->decisionToDFA, exprParserStaticData->sharedContextCache, options);
}

exprParser::~exprParser() {
  delete _interpreter;
}

const atn::ATN& exprParser::getATN() const {
  return *exprParserStaticData->atn;
}

std::string exprParser::getGrammarFileName() const {
  return "expr.g4";
}

const std::vector<std::string>& exprParser::getRuleNames() const {
  return exprParserStaticData->ruleNames;
}

const dfa::Vocabulary& exprParser::getVocabulary() const {
  return exprParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView exprParser::getSerializedATN() const {
  return exprParserStaticData->serializedATN;
}


//----------------- ProgContext ------------------------------------------------------------------

exprParser::ProgContext::ProgContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* exprParser::ProgContext::EOF() {
  return getToken(exprParser::EOF, 0);
}

std::vector<exprParser::DefContext *> exprParser::ProgContext::def() {
  return getRuleContexts<exprParser::DefContext>();
}

exprParser::DefContext* exprParser::ProgContext::def(size_t i) {
  return getRuleContext<exprParser::DefContext>(i);
}


size_t exprParser::ProgContext::getRuleIndex() const {
  return exprParser::RuleProg;
}

void exprParser::ProgContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<exprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterProg(this);
}

void exprParser::ProgContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<exprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitProg(this);
}

exprParser::ProgContext* exprParser::prog() {
  ProgContext *_localctx = _tracker.createInstance<ProgContext>(_ctx, getState());
  enterRule(_localctx, 0, exprParser::RuleProg);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(15);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == exprParser::T__4) {
      setState(12);
      def();
      setState(17);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(18);
    match(exprParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatContext ------------------------------------------------------------------

exprParser::StatContext::StatContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* exprParser::StatContext::ID() {
  return getToken(exprParser::ID, 0);
}

exprParser::ExprContext* exprParser::StatContext::expr() {
  return getRuleContext<exprParser::ExprContext>(0);
}


size_t exprParser::StatContext::getRuleIndex() const {
  return exprParser::RuleStat;
}

void exprParser::StatContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<exprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStat(this);
}

void exprParser::StatContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<exprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStat(this);
}

exprParser::StatContext* exprParser::stat() {
  StatContext *_localctx = _tracker.createInstance<StatContext>(_ctx, getState());
  enterRule(_localctx, 2, exprParser::RuleStat);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(32);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(20);
      match(exprParser::ID);
      setState(21);
      match(exprParser::T__0);
      setState(22);
      expr(0);
      setState(23);
      match(exprParser::T__1);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(25);
      expr(0);
      setState(26);
      match(exprParser::T__1);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(28);
      match(exprParser::T__2);
      setState(29);
      expr(0);
      setState(30);
      match(exprParser::T__1);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParamContext ------------------------------------------------------------------

exprParser::ParamContext::ParamContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> exprParser::ParamContext::ID() {
  return getTokens(exprParser::ID);
}

tree::TerminalNode* exprParser::ParamContext::ID(size_t i) {
  return getToken(exprParser::ID, i);
}


size_t exprParser::ParamContext::getRuleIndex() const {
  return exprParser::RuleParam;
}

void exprParser::ParamContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<exprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParam(this);
}

void exprParser::ParamContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<exprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParam(this);
}

exprParser::ParamContext* exprParser::param() {
  ParamContext *_localctx = _tracker.createInstance<ParamContext>(_ctx, getState());
  enterRule(_localctx, 4, exprParser::RuleParam);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(34);
    match(exprParser::ID);
    setState(35);
    match(exprParser::T__3);
    setState(36);
    match(exprParser::ID);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DefContext ------------------------------------------------------------------

exprParser::DefContext::DefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> exprParser::DefContext::ID() {
  return getTokens(exprParser::ID);
}

tree::TerminalNode* exprParser::DefContext::ID(size_t i) {
  return getToken(exprParser::ID, i);
}

exprParser::ParamContext* exprParser::DefContext::param() {
  return getRuleContext<exprParser::ParamContext>(0);
}

std::vector<exprParser::StatContext *> exprParser::DefContext::stat() {
  return getRuleContexts<exprParser::StatContext>();
}

exprParser::StatContext* exprParser::DefContext::stat(size_t i) {
  return getRuleContext<exprParser::StatContext>(i);
}


size_t exprParser::DefContext::getRuleIndex() const {
  return exprParser::RuleDef;
}

void exprParser::DefContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<exprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDef(this);
}

void exprParser::DefContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<exprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDef(this);
}

exprParser::DefContext* exprParser::def() {
  DefContext *_localctx = _tracker.createInstance<DefContext>(_ctx, getState());
  enterRule(_localctx, 6, exprParser::RuleDef);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(38);
    match(exprParser::T__4);
    setState(39);
    match(exprParser::ID);
    setState(40);
    match(exprParser::T__5);
    setState(41);
    param();
    setState(42);
    match(exprParser::T__6);
    setState(43);
    match(exprParser::ID);
    setState(44);
    match(exprParser::T__7);
    setState(48);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 197704) != 0) {
      setState(45);
      stat();
      setState(50);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(51);
    match(exprParser::T__8);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

exprParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* exprParser::ExprContext::ID() {
  return getToken(exprParser::ID, 0);
}

tree::TerminalNode* exprParser::ExprContext::INT() {
  return getToken(exprParser::INT, 0);
}

exprParser::FuncContext* exprParser::ExprContext::func() {
  return getRuleContext<exprParser::FuncContext>(0);
}

std::vector<exprParser::ExprContext *> exprParser::ExprContext::expr() {
  return getRuleContexts<exprParser::ExprContext>();
}

exprParser::ExprContext* exprParser::ExprContext::expr(size_t i) {
  return getRuleContext<exprParser::ExprContext>(i);
}


size_t exprParser::ExprContext::getRuleIndex() const {
  return exprParser::RuleExpr;
}

void exprParser::ExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<exprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExpr(this);
}

void exprParser::ExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<exprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExpr(this);
}


exprParser::ExprContext* exprParser::expr() {
   return expr(0);
}

exprParser::ExprContext* exprParser::expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  exprParser::ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, parentState);
  exprParser::ExprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 8;
  enterRecursionRule(_localctx, 8, exprParser::RuleExpr, precedence);

    

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(63);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx)) {
    case 1: {
      setState(54);
      match(exprParser::ID);
      break;
    }

    case 2: {
      setState(55);
      match(exprParser::INT);
      break;
    }

    case 3: {
      setState(56);
      func();
      break;
    }

    case 4: {
      setState(57);
      match(exprParser::T__9);
      setState(58);
      expr(7);
      break;
    }

    case 5: {
      setState(59);
      match(exprParser::T__5);
      setState(60);
      expr(0);
      setState(61);
      match(exprParser::T__6);
      break;
    }

    default:
      break;
    }
    _ctx->stop = _input->LT(-1);
    setState(82);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(80);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
        case 1: {
          _localctx = _tracker.createInstance<ExprContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleExpr);
          setState(65);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(66);
          match(exprParser::T__10);
          setState(67);
          expr(7);
          break;
        }

        case 2: {
          _localctx = _tracker.createInstance<ExprContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleExpr);
          setState(68);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(69);
          match(exprParser::T__11);
          setState(70);
          expr(6);
          break;
        }

        case 3: {
          _localctx = _tracker.createInstance<ExprContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleExpr);
          setState(71);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(72);
          match(exprParser::T__12);
          setState(73);
          expr(5);
          break;
        }

        case 4: {
          _localctx = _tracker.createInstance<ExprContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleExpr);
          setState(74);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(75);
          match(exprParser::T__13);
          setState(76);
          expr(4);
          break;
        }

        case 5: {
          _localctx = _tracker.createInstance<ExprContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleExpr);
          setState(77);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(78);
          match(exprParser::T__14);
          setState(79);
          expr(3);
          break;
        }

        default:
          break;
        } 
      }
      setState(84);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- FuncContext ------------------------------------------------------------------

exprParser::FuncContext::FuncContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> exprParser::FuncContext::ID() {
  return getTokens(exprParser::ID);
}

tree::TerminalNode* exprParser::FuncContext::ID(size_t i) {
  return getToken(exprParser::ID, i);
}

exprParser::ExprContext* exprParser::FuncContext::expr() {
  return getRuleContext<exprParser::ExprContext>(0);
}


size_t exprParser::FuncContext::getRuleIndex() const {
  return exprParser::RuleFunc;
}

void exprParser::FuncContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<exprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunc(this);
}

void exprParser::FuncContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<exprListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunc(this);
}

exprParser::FuncContext* exprParser::func() {
  FuncContext *_localctx = _tracker.createInstance<FuncContext>(_ctx, getState());
  enterRule(_localctx, 10, exprParser::RuleFunc);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(85);
    match(exprParser::ID);
    setState(89);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == exprParser::T__13) {
      setState(86);
      match(exprParser::T__13);
      setState(87);
      match(exprParser::ID);
      setState(88);
      match(exprParser::T__14);
    }
    setState(91);
    match(exprParser::T__5);
    setState(92);
    expr(0);
    setState(93);
    match(exprParser::T__6);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool exprParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 4: return exprSempred(antlrcpp::downCast<ExprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool exprParser::exprSempred(ExprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 6);
    case 1: return precpred(_ctx, 5);
    case 2: return precpred(_ctx, 4);
    case 3: return precpred(_ctx, 3);
    case 4: return precpred(_ctx, 2);

  default:
    break;
  }
  return true;
}

void exprParser::initialize() {
  ::antlr4::internal::call_once(exprParserOnceFlag, exprParserInitialize);
}
