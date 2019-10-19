// $Id: func_define_node.h,v 1.5 2018/04/19 16:48:48 ist426029 Exp $ -*- c++ -*-
#ifndef __GR8_FUNCDEFINENODE_H__
#define __GR8_FUNCDEFINENODE_H__

#include <cdk/ast/identifier_node.h>
#include <cdk/ast/expression_node.h>
#include <vector>
#include <cdk/ast/sequence_node.h>
#include <cdk/basic_type.h>
#include <string>

namespace gr8 {

  /**
   * Class for describing function definition nodes.
   */
  class func_define_node : public cdk::basic_node {
    int _qualificador;
    basic_type *_type;
    std::string *_identificador;
    cdk::sequence_node *_vars;
    block_node *_block;

  public:
    inline func_define_node(int lineno, int qualificador, basic_type *type, std::string *identificador, cdk::sequence_node *vars, block_node *block ) :
        basic_node(lineno), _qualificador(qualificador), _type(type), _identificador(identificador), _vars(vars) , _block(block) {
    }

  public:
    inline cdk::sequence_node *vars() {
      return _vars;
    }

    inline int qualificador() {
      return _qualificador;
    }

    inline basic_type *type() {
      return _type;
    }

    inline std::string *identificador() {
      return _identificador;
    }

    inline block_node *block() {
      return _block;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_func_define_node(this, level);
    }

  };

} // gr8

#endif
