// $Id: index_node.h,v 1.3 2018/03/23 15:07:58 ist426029 Exp $ -*- c++ -*-
#ifndef __GR8_INDEXNODE_H__
#define __GR8_INDEXNODE_H__

#include <cdk/ast/expression_node.h>

namespace gr8 {

  /**
   * Class for describing print nodes.
   */
  class index_node: public cdk::lvalue_node {
    cdk::expression_node *_argument;
    cdk::expression_node *_identificador;

  public:
    inline index_node(int lineno, cdk::expression_node *argument, cdk::expression_node *identificador ) :
        lvalue_node(lineno), _argument(argument), _identificador(identificador) {
    }

    inline cdk::expression_node *argument() {
      return _argument;
    }

    inline cdk::expression_node *identifier() {
      return _identificador;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_index_node(this, level);
    }

  };

} // gr8

#endif
