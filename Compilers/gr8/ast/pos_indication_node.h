// $Id: pos_indication_node.h,v 1.1 2018/04/14 15:06:54 ist426029 Exp $ -*- c++ -*-
#ifndef __GR8_POSINDICATIONNODE_H__
#define __GR8_POSINDICATIONNODE_H__

#include <cdk/ast/expression_node.h>

namespace gr8 {

  /**
   * Class for describing sufix nodes.
   */
  class pos_indication_node: public cdk::expression_node {
    cdk::lvalue_node *_argument;

  public:
    inline pos_indication_node(int lineno, cdk::lvalue_node *argument ) :
        expression_node(lineno), _argument(argument) {
    }

  public:
    inline cdk::lvalue_node *argument() {
      return _argument;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_pos_indication_node(this, level);
    }

  };

} // gr8

#endif
