#ifndef __GR8_OBJECTSNODE_H__
#define __GR8_OBJECTSNODE_H__

#include <cdk/ast/expression_node.h>


namespace gr8 {

  /**
   * Class for describing the objects operator
   */
  //herdar de unary expression
  class objects_node: public cdk::unary_expression_node {

  public:
    objects_node(int lineno, cdk::expression_node *arg) :
        unary_expression_node(lineno, arg) {
    }
    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_objects_node(this, level);
    }

  };

} // cdk

#endif
