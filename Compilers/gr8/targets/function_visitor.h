#ifndef __GR8_SEMANTICS_FUNCTION_VISITOR_H__
#define __GR8_SEMANTICS_FUNCTION_VISITOR_H__

#include "targets/basic_ast_visitor.h"
#include "targets/symbol.h"

namespace gr8 {


  class function_visitor: public basic_ast_visitor {
    
  int _total_size;

  public:
    function_visitor(std::shared_ptr<cdk::compiler> compiler) :
        basic_ast_visitor(compiler), _total_size(0) {
    }

  public:
    ~function_visitor();

  public:
  	inline int get_total_size(){
  		return _total_size;
  	}

  public:
  // do not edit these lines
#define __IN_VISITOR_HEADER__
#include "ast/visitor_decls.h"       // automatically generated
#undef __IN_VISITOR_HEADER__
  // do not edit these lines: end

  };

} // gr8


//---------------------------------------------------------------------------
//     HELPER MACRO FOR CALCULATE SPACE
//---------------------------------------------------------------------------

#define INCREASE_SIZE(compiler, node) { \
  try { \
    gr8::function_visitor checker(compiler); \
    (node)->accept(&checker, 0); \
  } \
  catch (const std::string &problem) { \
    std::cerr << (node)->lineno() << ": " << problem << std::endl; \
    return; \
  } \
}

#define CALCULATE_SPACE INCREASE_SIZE(_compiler, node)

#endif


