#include <string>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

#define ASSERT_UNSPEC \
    { if (node->type() != nullptr && \
          node->type()->name() != basic_type::TYPE_UNSPEC) return; } //Verifica se o node ja foi visitado 

std::shared_ptr<gr8::symbol> aux_symbol = nullptr;

//---------------------------------------------------------------------------

void gr8::type_checker::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void gr8::type_checker::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}
void gr8::type_checker::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}
void gr8::type_checker::do_double_node(cdk::double_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
}
void gr8::type_checker::do_not_node(cdk::not_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() != basic_type::TYPE_INT){
    throw std::string("wrong type in argument of not node");
  }

  node->type(new basic_type(4, basic_type::TYPE_INT));
}
void gr8::type_checker::do_and_node(cdk::and_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void gr8::type_checker::do_or_node(cdk::or_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}


//---------------------------------------------------------------------------

void gr8::type_checker::do_integer_node(cdk::integer_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void gr8::type_checker::do_string_node(cdk::string_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_STRING));
}

//---------------------------------------------------------------------------

void gr8::type_checker::processUnaryExpression(cdk::unary_expression_node * const node, int lvl) {

  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);

  if ((node->argument()->type()->name() != basic_type::TYPE_INT) && (node->argument()->type()->name() != basic_type::TYPE_DOUBLE))
    throw std::string("wrong type in argument of unary expression");

  node->type(node->argument()->type());
}

void gr8::type_checker::do_neg_node(cdk::neg_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::type_checker::processBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  //ONLY TO AND, OR, MOD, NODES
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() != basic_type::TYPE_INT) throw std::string("wrong type in left argument of binary expression");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() != basic_type::TYPE_INT) throw std::string("wrong type in right argument of binary expression");

  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void gr8::type_checker::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if(node->left()->type()->name() == basic_type::TYPE_STRING)
    throw std::string("wrong type in left argument of ADD expression");
  if(node->right()->type()->name() == basic_type::TYPE_STRING)
    throw std::string("wrong type in right argument of ADD expression");
  
  if(node->left()->type()->name() == basic_type::TYPE_POINTER){
    if(node->right()->type()->name() != basic_type::TYPE_INT)
      throw std::string("wrong types in pointer addition");

    node->type(new basic_type(4, basic_type::TYPE_POINTER));
  }

  else if(node->right()->type()->name() == basic_type::TYPE_POINTER){
    if(node->left()->type()->name() != basic_type::TYPE_INT)
      throw std::string("wrong types in pointer addition");
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
  }

  else if((node->left()->type()->name() == basic_type::TYPE_DOUBLE) || (node->right()->type()->name() == basic_type::TYPE_DOUBLE)) {
    if((node->left()->type()->name() != basic_type::TYPE_INT) && (node->left()->type()->name() != basic_type::TYPE_DOUBLE))
      throw std::string("wrong type in left value of mul node");
    if((node->right()->type()->name() != basic_type::TYPE_INT) && (node->right()->type()->name() != basic_type::TYPE_DOUBLE))
      throw std::string("wrong type in right value of mul node");
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  }

  else{
    node->type(new basic_type(4, basic_type::TYPE_INT));
  }
}
void gr8::type_checker::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if(node->left()->type()->name() == basic_type::TYPE_STRING)
    throw std::string("wrong type in left argument of ADD expression");
  if(node->right()->type()->name() == basic_type::TYPE_STRING)
    throw std::string("wrong type in right argument of ADD expression");
  
  if(node->left()->type()->name() == basic_type::TYPE_POINTER){
    if((node->right()->type()->name() != basic_type::TYPE_INT) || (node->right()->type()->name() != basic_type::TYPE_POINTER))
      throw std::string("wrong types in pointer addition");
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
  }

  else if(node->right()->type()->name() == basic_type::TYPE_POINTER){
    if(node->left()->type()->name() != basic_type::TYPE_INT)
      throw std::string("wrong types in pointer addition");
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
  }

  else if((node->left()->type()->name() == basic_type::TYPE_DOUBLE) || (node->right()->type()->name() == basic_type::TYPE_DOUBLE)) {
    if((node->left()->type()->name() != basic_type::TYPE_INT) && (node->left()->type()->name() != basic_type::TYPE_DOUBLE))
      throw std::string("wrong type in left value of mul node");
    if((node->right()->type()->name() != basic_type::TYPE_INT) && (node->right()->type()->name() != basic_type::TYPE_DOUBLE))
      throw std::string("wrong type in right value of mul node");
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  }

  else{
    node->type(new basic_type(4, basic_type::TYPE_INT));
  }
}
void gr8::type_checker::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if((node->left()->type()->name() != basic_type::TYPE_INT) && (node->left()->type()->name() != basic_type::TYPE_DOUBLE))
    throw std::string("wrong type in left value of mul node");
  if((node->right()->type()->name() != basic_type::TYPE_INT) && (node->right()->type()->name() != basic_type::TYPE_DOUBLE))
    throw std::string("wrong type in right value of mul node");

  else if((node->left()->type()->name() == basic_type::TYPE_DOUBLE) || (node->right()->type()->name() == basic_type::TYPE_DOUBLE)) {
    if((node->left()->type()->name() != basic_type::TYPE_INT) && (node->left()->type()->name() != basic_type::TYPE_DOUBLE))
      throw std::string("wrong type in left value of mul node");
    if((node->right()->type()->name() != basic_type::TYPE_INT) && (node->right()->type()->name() != basic_type::TYPE_DOUBLE))
      throw std::string("wrong type in right value of mul node");
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  }

  else
    node->type(new basic_type(4, basic_type::TYPE_INT));

}

void gr8::type_checker::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if((node->left()->type()->name() != basic_type::TYPE_INT) && (node->left()->type()->name() != basic_type::TYPE_DOUBLE))
    throw std::string("wrong type in left value of mul node");
  if((node->right()->type()->name() != basic_type::TYPE_INT) && (node->right()->type()->name() != basic_type::TYPE_DOUBLE))
    throw std::string("wrong type in right value of mul node");

  else if((node->left()->type()->name() == basic_type::TYPE_DOUBLE) || (node->right()->type()->name() == basic_type::TYPE_DOUBLE)) {
    if((node->left()->type()->name() != basic_type::TYPE_INT) && (node->left()->type()->name() != basic_type::TYPE_DOUBLE))
      throw std::string("wrong type in left value of mul node");
    if((node->right()->type()->name() != basic_type::TYPE_INT) && (node->right()->type()->name() != basic_type::TYPE_DOUBLE))
      throw std::string("wrong type in right value of mul node");
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  }

  else
    node->type(new basic_type(4, basic_type::TYPE_INT));
}

void gr8::type_checker::do_mod_node(cdk::mod_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}

void gr8::type_checker::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_UNSPEC;

  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if ((node->left()->type()->name() != basic_type::TYPE_INT) && (node->left()->type()->name() != basic_type::TYPE_DOUBLE) )
    throw std::string("wrong type in left argument of lt expression");
  if ((node->right()->type()->name() != basic_type::TYPE_INT) && (node->left()->type()->name() != basic_type::TYPE_DOUBLE) )
    throw std::string("wrong type in right argument of lt expression");
  if((node->right()->type()->name() != node->left()->type()->name()))
    throw std::string("different types in eq node");
  
  else if((node->left()->type()->name() == basic_type::TYPE_DOUBLE) || (node->right()->type()->name() == basic_type::TYPE_DOUBLE)) {
    if((node->left()->type()->name() != basic_type::TYPE_INT) && (node->left()->type()->name() != basic_type::TYPE_DOUBLE))
      throw std::string("wrong type in left value of mul node");
    if((node->right()->type()->name() != basic_type::TYPE_INT) && (node->right()->type()->name() != basic_type::TYPE_DOUBLE))
      throw std::string("wrong type in right value of mul node");
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  }
  else
    node->type(new basic_type(4, basic_type::TYPE_INT));
}

void gr8::type_checker::do_le_node(cdk::le_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}

void gr8::type_checker::do_ge_node(cdk::ge_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}

void gr8::type_checker::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if ((node->left()->type()->name() != basic_type::TYPE_INT) && (node->left()->type()->name() != basic_type::TYPE_DOUBLE))
    throw std::string("wrong type in left argument of gt expression");
  if ((node->right()->type()->name() != basic_type::TYPE_INT) && (node->left()->type()->name() != basic_type::TYPE_DOUBLE))
    throw std::string("wrong type in right argument of gt expression");
  if((node->right()->type()->name() != node->left()->type()->name()))
    throw std::string("different types in eq node");
  
  else if((node->left()->type()->name() == basic_type::TYPE_DOUBLE) || (node->right()->type()->name() == basic_type::TYPE_DOUBLE)) {
    if((node->left()->type()->name() != basic_type::TYPE_INT) && (node->left()->type()->name() != basic_type::TYPE_DOUBLE))
      throw std::string("wrong type in left value of mul node");
    if((node->right()->type()->name() != basic_type::TYPE_INT) && (node->right()->type()->name() != basic_type::TYPE_DOUBLE))
      throw std::string("wrong type in right value of mul node");
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  }
  else
    node->type(new basic_type(4, basic_type::TYPE_INT));
}

void gr8::type_checker::do_ne_node(cdk::ne_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}

void gr8::type_checker::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_UNSPEC;
   
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if ((node->left()->type()->name() != basic_type::TYPE_INT) && (node->left()->type()->name() != basic_type::TYPE_DOUBLE) && (node->right()->type()->name() != basic_type::TYPE_POINTER))
    throw std::string("wrong type in left argument of eq expression");
  if ((node->right()->type()->name() != basic_type::TYPE_INT) && (node->left()->type()->name() != basic_type::TYPE_DOUBLE) && (node->right()->type()->name() != basic_type::TYPE_POINTER))
    throw std::string("wrong type in right argument of eq expression");
  if((node->right()->type()->name() != node->left()->type()->name()))
    throw std::string("different types in eq node");

  if(node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  else
    node->type(new basic_type(4, basic_type::TYPE_INT));

}
//---------------------------------------------------------------------------

void gr8::type_checker::do_identifier_node(cdk::identifier_node * const node, int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->name();
  aux_symbol = _symtab.find(id);

  if (aux_symbol != nullptr) {
    node->type(aux_symbol->type());
    _parent->set_new_symbol(aux_symbol); //apenas para me dar jeito aquando de um assign
  } else {
    throw id;
  }
}

void gr8::type_checker::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_UNSPEC;
  try {
    node->lvalue()->accept(this, lvl);
    node->type(node->lvalue()->type());
  } catch (const std::string &id) {
    throw id;
  }
}

void gr8::type_checker::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_UNSPEC;

  try {
    node->lvalue()->accept(this, lvl);
  } catch (const std::string &id) {
    throw std::string("LVAL not declared in ASSIGNMENT NODE");
  }

  node->rvalue()->accept(this, lvl + 2);

  if((node->lvalue()->type()->name() == basic_type::TYPE_STRING) && (node->rvalue()->type()->name() != basic_type::TYPE_STRING))
    throw new std::string("WRONG TYPES IN ASSIGNMENT NODE");

  if((node->rvalue()->type()->name() == basic_type::TYPE_INT) && (node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE)){
    node->rvalue()->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  }
  /*if(node->rvalue()->type()->name() == basic_type::TYPE_STRUCT)
    node->rvalue()->type(new basic_type(4, basic_type::TYPE_INT));*/

  else if(node->lvalue()->type()->name() == basic_type::TYPE_POINTER){
    basic_type * aux1 = node->lvalue()->type()->subtype();

    while(aux1->name() == basic_type::TYPE_POINTER){
      aux1 = aux1->subtype();
    }

    if(node->rvalue()->type()->name() == basic_type::TYPE_POINTER){
      if(aux1->name() != node->rvalue()->type()->subtype()->name()){
        throw new std::string("WRONG TYPES IN ASSIGNMENT NODE");
      }
    }
    else if(node->rvalue()->type()->name() == basic_type::TYPE_STRUCT)
      node->rvalue()->type(new basic_type(aux1->size(), basic_type::TYPE_STRUCT));
    else
      node->rvalue()->type(aux1);
  }

  node->type(node->lvalue()->type());
}

//---------------------------------------------------------------------------

void gr8::type_checker::do_index_node(gr8::index_node * const node, int lvl) {
  ASSERT_UNSPEC;
  
  node->argument()->accept(this, lvl + 2);
  if(node->argument()->type()->name() != basic_type::TYPE_INT)
    throw std::string("left argument in index node is not a int");

  node->identifier()->accept(this, lvl +2);
  if(node->identifier()->type()->name() != basic_type::TYPE_POINTER)
    throw std::string("right argument in index node is not a pointer");
  
  if(aux_symbol != nullptr){
    basic_type * aux1 = aux_symbol->type()->subtype();

    while(aux1->name() == basic_type::TYPE_POINTER){
      aux1 = aux1->subtype();
    }
    node->type(aux1);
  }
}

//---------------------------------------------------------------------------

void gr8::type_checker::do_evaluation_node(gr8::evaluation_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  

}

void gr8::type_checker::do_print_node(gr8::print_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void gr8::type_checker::do_read_node(gr8::read_node * const node, int lvl) {
  ASSERT_UNSPEC; 
  node->type(new basic_type(4, basic_type::TYPE_STRUCT));
}

//---------------------------------------------------------------------------

void gr8::type_checker::do_sweeping_node(gr8::sweeping_node * const node, int lvl) {
  node->begin_expression()->accept(this, lvl + 2);
  if (node->begin_expression()->type()->name() != basic_type::TYPE_INT) throw std::string("wrong type in begin expression of sweeping node");

  node->end_expression()->accept(this, lvl + 2);
  if (node->end_expression()->type()->name() != basic_type::TYPE_INT) throw std::string("wrong type in end expression of sweeping node");
}

//---------------------------------------------------------------------------

void gr8::type_checker::do_again_node(gr8::again_node * const node, int lvl) {
  //
}

//---------------------------------------------------------------------------

void gr8::type_checker::do_stop_node(gr8::stop_node * const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void gr8::type_checker::do_return_node(gr8::return_node * const node, int lvl) {
  if(node->argument() != nullptr){
    node->argument()->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void gr8::type_checker::do_var_init_node(gr8::var_init_node * const node, int lvl) {
  std::string &id = *(node->identifier());
  std::shared_ptr<gr8::symbol> symbol = std::make_shared<gr8::symbol>(node->type(), id, 0, "var", nullptr);
  if (!_symtab.insert(id, symbol))
    throw std::string(id + " redeclared");

  if (node->argument() != nullptr) {
    node->argument()->accept(this, lvl+2);

    if(node->argument()->type()->name() != basic_type::TYPE_STRUCT){
      if (node->type()->name() != node->argument()->type()->name()) {
         if ((node->type()->name() == basic_type::TYPE_DOUBLE)
          && ((node->argument()->type()->name() != basic_type::TYPE_INT))) {
            throw std::string("wrong type for initializer");
          }
        }
    } 
  }
  _parent->set_new_symbol(symbol);
} 

//---------------------------------------------------------------------------

void gr8::type_checker::do_identity_node(gr8::identity_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::type_checker::do_objects_node(gr8::objects_node * const node, int lvl) {
  node->argument()->accept(this, lvl);
  if (node->argument()->type()->name() != basic_type::TYPE_INT){
    throw std::string("wrong type in argument of objects node");
  }
  node->type(new basic_type(4, basic_type::TYPE_STRUCT)); //PROTOCOLO PARA O ASSIGNMENT SABER QUE SE TRATA DE UM OBJECTS
}

//---------------------------------------------------------------------------

void gr8::type_checker::do_func_declare_node(gr8::func_declare_node * const node, int lvl) {
  std::string &id = *(node->identificador());
  std::vector<basic_type> * aux1 = new std::vector<basic_type>();
  std::shared_ptr<gr8::symbol> symbol;

  if(node->vars() != nullptr){
    for(unsigned int i = 0; i < node->vars()->size(); i++){
      gr8::var_init_node * aux2 = dynamic_cast<gr8::var_init_node*>(node->vars()->node(i));
      if(aux2 != nullptr){
        aux1->push_back(*(aux2->type()));
      }
    }
    symbol = std::make_shared<gr8::symbol>(node->type(), id, 0, "func", aux1);
  }
  else{
    symbol = std::make_shared<gr8::symbol>(node->type(), id, 0, "func", nullptr);
  }

  if (!_symtab.insert(id, symbol))
    throw std::string(id + " redeclared");
  _parent->set_new_symbol(symbol);

}

void gr8::type_checker::do_func_define_node(gr8::func_define_node * const node, int lvl) {
  std::string &id = *(node->identificador());
  std::vector<basic_type> * aux1 = new std::vector<basic_type>();
  std::shared_ptr<gr8::symbol> symbol;

  if(node->vars() != nullptr){
    for(unsigned int i = 0; i < node->vars()->size(); i++){
      gr8::var_init_node * aux2 = dynamic_cast<gr8::var_init_node*>(node->vars()->node(i));
      if(aux2 != nullptr){
        aux1->push_back(*(aux2->type()));
      }
    }
    symbol = std::make_shared<gr8::symbol>(node->type(), id, 0, "func", aux1);
  }
  else{
    symbol = std::make_shared<gr8::symbol>(node->type(), id, 0, "func", nullptr);
  }
  _symtab.insert(id, symbol);
  _parent->set_new_symbol(symbol);
}

//---------------------------------------------------------------------------

void gr8::type_checker::do_func_call_node(gr8::func_call_node * const node, int lvl) {
  ASSERT_UNSPEC;

  std::string &id = *(node->identificador());
  std::shared_ptr<gr8::symbol> symbol = _symtab.find(id,0);
  if(symbol == nullptr)
    throw std::string("symbol not found");
  if(symbol->functionality().compare("func") != 0)
    throw std::string("symbol not a function");
  //verificar se a sequencia de atributos é a mesma da função declarada (mesma ordem e mesmo tipos)
  if(symbol->declare_sequence() != nullptr){
    if(node->expressions()->size() != symbol->declare_sequence()->size())
      throw std::string("wrong sequence of variables in func_call_node");

    std::vector<basic_type> * aux2 = symbol->declare_sequence();

    for( unsigned int i = 0; i < node->expressions()->size(); i++){
      
      cdk::expression_node * aux1 = dynamic_cast<cdk::expression_node*>(node->expressions()->node(i));
      aux1->accept(this, lvl);
    
      if(aux1->type()->name() != aux2->at(i).name()){
        if(aux2->at(i).name() == basic_type::TYPE_DOUBLE){
          if(aux1->type()->name() != basic_type::TYPE_INT)
            throw std::string("wrong sequence of variables in func_call_node");
        }
        else
          throw std::string("wrong sequence of variables in func_call_node");
      }
    }
  }
  node->type(symbol->type());
  _parent->set_new_symbol(symbol);
}

//---------------------------------------------------------------------------

void gr8::type_checker::do_block_node(gr8::block_node * const node, int lvl) {
  // EMPTY
}


//---------------------------------------------------------------------------

void gr8::type_checker::do_pos_indication_node(gr8::pos_indication_node * const node, int lvl) {
  ASSERT_UNSPEC;
  try {
    node->argument()->accept(this, lvl);
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
    node->type()->_subtype = new basic_type(node->argument()->type()->size(),node->argument()->type()->name());

  } catch (const std::string &id) {
    throw "undeclared variable '" + id + "'";
  }
}


//---------------------------------------------------------------------------

void gr8::type_checker::do_null_node(gr8::null_node * const node, int lvl) {
  node->type(new basic_type(4, basic_type::TYPE_STRUCT));
}

//---------------------------------------------------------------------------

void gr8::type_checker::do_if_node(gr8::if_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

void gr8::type_checker::do_if_else_node(gr8::if_else_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}
