
// Generated from expr.g4 by ANTLR 4.11.1


#include "exprLexer.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct ExprLexerStaticData final {
  ExprLexerStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  ExprLexerStaticData(const ExprLexerStaticData&) = delete;
  ExprLexerStaticData(ExprLexerStaticData&&) = delete;
  ExprLexerStaticData& operator=(const ExprLexerStaticData&) = delete;
  ExprLexerStaticData& operator=(ExprLexerStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> channelNames;
  const std::vector<std::string> modeNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag exprlexerLexerOnceFlag;
ExprLexerStaticData *exprlexerLexerStaticData = nullptr;

void exprlexerLexerInitialize() {
  assert(exprlexerLexerStaticData == nullptr);
  auto staticData = std::make_unique<ExprLexerStaticData>(
    std::vector<std::string>{
      "T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "T__6", "T__7", "T__8", 
      "T__9", "T__10", "T__11", "T__12", "T__13", "T__14", "ID", "INT", 
      "WhiteSpace"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
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
  	4,0,18,95,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,
  	7,14,2,15,7,15,2,16,7,16,2,17,7,17,1,0,1,0,1,1,1,1,1,2,1,2,1,2,1,2,1,
  	2,1,2,1,2,1,3,1,3,1,4,1,4,1,4,1,5,1,5,1,6,1,6,1,7,1,7,1,8,1,8,1,9,1,9,
  	1,9,1,9,1,10,1,10,1,10,1,10,1,11,1,11,1,11,1,12,1,12,1,13,1,13,1,14,1,
  	14,1,15,4,15,80,8,15,11,15,12,15,81,1,16,4,16,85,8,16,11,16,12,16,86,
  	1,17,4,17,90,8,17,11,17,12,17,91,1,17,1,17,0,0,18,1,1,3,2,5,3,7,4,9,5,
  	11,6,13,7,15,8,17,9,19,10,21,11,23,12,25,13,27,14,29,15,31,16,33,17,35,
  	18,1,0,3,1,0,97,122,1,0,48,57,2,0,9,10,32,32,97,0,1,1,0,0,0,0,3,1,0,0,
  	0,0,5,1,0,0,0,0,7,1,0,0,0,0,9,1,0,0,0,0,11,1,0,0,0,0,13,1,0,0,0,0,15,
  	1,0,0,0,0,17,1,0,0,0,0,19,1,0,0,0,0,21,1,0,0,0,0,23,1,0,0,0,0,25,1,0,
  	0,0,0,27,1,0,0,0,0,29,1,0,0,0,0,31,1,0,0,0,0,33,1,0,0,0,0,35,1,0,0,0,
  	1,37,1,0,0,0,3,39,1,0,0,0,5,41,1,0,0,0,7,48,1,0,0,0,9,50,1,0,0,0,11,53,
  	1,0,0,0,13,55,1,0,0,0,15,57,1,0,0,0,17,59,1,0,0,0,19,61,1,0,0,0,21,65,
  	1,0,0,0,23,69,1,0,0,0,25,72,1,0,0,0,27,74,1,0,0,0,29,76,1,0,0,0,31,79,
  	1,0,0,0,33,84,1,0,0,0,35,89,1,0,0,0,37,38,5,61,0,0,38,2,1,0,0,0,39,40,
  	5,59,0,0,40,4,1,0,0,0,41,42,5,114,0,0,42,43,5,101,0,0,43,44,5,116,0,0,
  	44,45,5,117,0,0,45,46,5,114,0,0,46,47,5,110,0,0,47,6,1,0,0,0,48,49,5,
  	58,0,0,49,8,1,0,0,0,50,51,5,102,0,0,51,52,5,110,0,0,52,10,1,0,0,0,53,
  	54,5,40,0,0,54,12,1,0,0,0,55,56,5,41,0,0,56,14,1,0,0,0,57,58,5,123,0,
  	0,58,16,1,0,0,0,59,60,5,125,0,0,60,18,1,0,0,0,61,62,5,110,0,0,62,63,5,
  	111,0,0,63,64,5,116,0,0,64,20,1,0,0,0,65,66,5,97,0,0,66,67,5,110,0,0,
  	67,68,5,100,0,0,68,22,1,0,0,0,69,70,5,111,0,0,70,71,5,114,0,0,71,24,1,
  	0,0,0,72,73,5,43,0,0,73,26,1,0,0,0,74,75,5,60,0,0,75,28,1,0,0,0,76,77,
  	5,62,0,0,77,30,1,0,0,0,78,80,7,0,0,0,79,78,1,0,0,0,80,81,1,0,0,0,81,79,
  	1,0,0,0,81,82,1,0,0,0,82,32,1,0,0,0,83,85,7,1,0,0,84,83,1,0,0,0,85,86,
  	1,0,0,0,86,84,1,0,0,0,86,87,1,0,0,0,87,34,1,0,0,0,88,90,7,2,0,0,89,88,
  	1,0,0,0,90,91,1,0,0,0,91,89,1,0,0,0,91,92,1,0,0,0,92,93,1,0,0,0,93,94,
  	6,17,0,0,94,36,1,0,0,0,4,0,81,86,91,1,6,0,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  exprlexerLexerStaticData = staticData.release();
}

}

exprLexer::exprLexer(CharStream *input) : Lexer(input) {
  exprLexer::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *exprlexerLexerStaticData->atn, exprlexerLexerStaticData->decisionToDFA, exprlexerLexerStaticData->sharedContextCache);
}

exprLexer::~exprLexer() {
  delete _interpreter;
}

std::string exprLexer::getGrammarFileName() const {
  return "expr.g4";
}

const std::vector<std::string>& exprLexer::getRuleNames() const {
  return exprlexerLexerStaticData->ruleNames;
}

const std::vector<std::string>& exprLexer::getChannelNames() const {
  return exprlexerLexerStaticData->channelNames;
}

const std::vector<std::string>& exprLexer::getModeNames() const {
  return exprlexerLexerStaticData->modeNames;
}

const dfa::Vocabulary& exprLexer::getVocabulary() const {
  return exprlexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView exprLexer::getSerializedATN() const {
  return exprlexerLexerStaticData->serializedATN;
}

const atn::ATN& exprLexer::getATN() const {
  return *exprlexerLexerStaticData->atn;
}




void exprLexer::initialize() {
  ::antlr4::internal::call_once(exprlexerLexerOnceFlag, exprlexerLexerInitialize);
}
