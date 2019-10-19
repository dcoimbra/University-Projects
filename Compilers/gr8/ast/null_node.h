// $Id: null_node.h,v 1.1 2018/04/14 15:06:54 ist426029 Exp $ -*- c++ -*-
#ifndef __GR8_NULLNODE_H__
#define __GR8_NULLNODE_H__

namespace gr8 {

  /**
   * Class for describing null nodes.
   */
  class null_node: public cdk::expression_node {

  public:
    inline null_node(int lineno) :
        cdk::expression_node(lineno) {
    }

  public:

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_null_node(this, level);
    }

  };

} // gr8

#endif
