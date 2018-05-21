// $Id: sweeping_node.h,v 1.2 2018/03/23 15:07:58 ist426029 Exp $ -*- c++ -*-
#ifndef __GR8_SWEEPINGNODE_H__
#define __GR8_SWEEPINGNODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>

namespace gr8 {

  /**
   * Class for describing while-cycle nodes.
   */
  class sweeping_node: public cdk::basic_node {
    cdk::expression_node *_begin_expression;
    cdk::expression_node *_end_expression;
    cdk::expression_node *_increment;
    cdk::lvalue_node *_variable;
    block_node *_block;

  public:
    inline sweeping_node(int lineno, cdk::expression_node *begin_expression, cdk::expression_node *end_expression, cdk::expression_node *increment, cdk::lvalue_node *variable ,block_node *block) :
        basic_node(lineno), _begin_expression(begin_expression), _end_expression(end_expression),_increment(increment), _variable(variable), _block(block) {
    } 

  public:
    inline cdk::expression_node *begin_expression() {
      return _begin_expression;
    }

    inline cdk::expression_node *end_expression() {
      return _end_expression;
    }

    inline cdk::expression_node *increment() {
      return _increment;
    }

    inline cdk::lvalue_node *variable() {
      return _variable;
    }
    inline block_node *block() {
      return _block;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_sweeping_node(this, level);
    }

  };

} // gr8

#endif
