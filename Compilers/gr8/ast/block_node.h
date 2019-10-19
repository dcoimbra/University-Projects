// $Id: block_node.h,v 1.4 2018/04/19 20:06:40 ist426008 Exp $ -*- c++ -*-
#ifndef __GR8_BLOCKNODE_H__
#define __GR8_BLOCKNODE_H__

#include <cdk/ast/sequence_node.h>
#include <vector>

namespace gr8 {

  /**
   * Class for describing print nodes.
   */
  class block_node: public cdk::basic_node {
    cdk::sequence_node *_var_declarations;
    cdk::sequence_node *_instructions;


  public:
    inline block_node(int lineno, cdk::sequence_node *var_declarations, cdk::sequence_node *instructions):
    basic_node(lineno), _var_declarations(var_declarations), _instructions(instructions){
    }

    inline cdk::sequence_node *var_declarations() {
      return _var_declarations;
    }

    inline cdk::sequence_node *instructions() {
      return _instructions;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_block_node(this, level);
    }

  };

} // gr8

#endif
