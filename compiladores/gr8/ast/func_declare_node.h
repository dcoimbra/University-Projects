 // $Id: func_declare_node.h,v 1.5 2018/04/19 16:48:48 ist426029 Exp $ -*- c++ -*-
#ifndef __GR8_FUNCDECLARENODE_H__
#define __GR8_FUNCDECLARENODE_H__

#include <cdk/basic_type.h>
#include <cdk/ast/identifier_node.h>
#include <cdk/ast/expression_node.h>
#include <vector>
#include <cdk/ast/sequence_node.h>
#include <string>

namespace gr8 {

  /**
   * Class for describing function declaration nodes.
   */
  //ou deixar ou herdar de typed node mas retirar o basic type
  //identifier passa a ser string 
  class func_declare_node : public cdk::basic_node {
    int _qualificador;
    basic_type *_type;
    std::string *_identificador;
    cdk::sequence_node *_vars;

  public:
    inline func_declare_node(int lineno, int qualificador, basic_type *type, std::string *identificador, cdk::sequence_node *vars ) :
        basic_node(lineno), _qualificador(qualificador), _type(type), _identificador(identificador), _vars(vars) {
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

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_func_declare_node(this, level);
    }

  };

} // gr8

#endif
