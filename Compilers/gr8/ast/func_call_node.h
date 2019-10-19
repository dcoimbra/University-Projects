// $Id: func_call_node.h,v 1.3 2018/04/14 15:06:54 ist426029 Exp $ -*- c++ -*-
#ifndef __GR8_FUNCCALLNODE_H__
#define __GR8_FUNCCALLNODE_H__

#include <cdk/ast/identifier_node.h>
#include <cdk/ast/expression_node.h>
#include <vector>
#include <cdk/ast/sequence_node.h>
#include <string>

namespace gr8 {

  /**
   * Class for describing function declaration nodes.
   */
  //herdar de expression
  //identifier passa a ser string
  class func_call_node : public cdk::expression_node {
    cdk::sequence_node *_expressions;
    std::string *_identificador;

  public:
    inline func_call_node(int lineno, std::string *identificador, cdk::sequence_node *expressions) :
        cdk::expression_node(lineno),  _expressions(expressions), _identificador(identificador) {
    }

    inline cdk::sequence_node *expressions() {
      return _expressions;
    }

    inline std::string *identificador() {
      return _identificador;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_func_call_node(this, level);
    }

  };

} // gr8

#endif
