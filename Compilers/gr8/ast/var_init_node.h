// $Id: var_init_node.h,v 1.3 2018/04/19 16:48:48 ist426029 Exp $ -*- c++ -*-
#ifndef __GR8_VARNODE_H__
#define __GR8_VARNODE_H__

#include <cdk/basic_type.h>
#include <cdk/ast/identifier_node.h>
#include <cdk/ast/expression_node.h>
#include <string>

namespace gr8 {

  /**
   * Class for describing print nodes.
   */
  //recebe um lvalue  e um rvalue
  class var_init_node: public cdk::basic_node {
    int _qualificador;
    basic_type *_type;
    std::string *_identificador;
    cdk::expression_node *_argument;


  public:
    inline var_init_node(int lineno, int qualificador, basic_type *type ,std::string *identificador, cdk::expression_node *argument) :
        cdk::basic_node(lineno), _qualificador(qualificador), _type(type), _identificador(identificador), _argument(argument)  {
    }

  public:
    inline cdk::expression_node *argument() {
      return _argument;
    }

    inline int qualificador() {
      return _qualificador;
    }

    inline basic_type *type() {
      return _type;
    }

    inline std::string *identifier() {
      return _identificador;
    }



    void accept(basic_ast_visitor *sp, int level) {
      sp->do_var_init_node(this, level);
    }

  };

} // gr8

#endif
