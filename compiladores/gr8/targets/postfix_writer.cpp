#include <string>
#include <sstream>
#include <algorithm>    
#include <vector>       
#include <stack>
#include "targets/type_checker.h"
#include "targets/function_visitor.h"
#include "targets/postfix_writer.h"
#include "ast/all.h"  // all.h is automatically generated

int one_time_flag = 0;
std::string funcao = "";
int offset = 0;
std::vector<std::string> func_declares;
int flag_ciclo = 0;
std::stack<std::string> again_stack;
std::stack<std::string> stop_stack;
int flag_func_arguments = 0;

//guardar na tabela de simbolos o offset de cada simbolo

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}
void gr8::postfix_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}
void gr8::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _pf.DOUBLE(node->value()); 
}

void gr8::postfix_writer::do_not_node(cdk::not_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
  _pf.INT(0);
  _pf.EQ();
}
void gr8::postfix_writer::do_and_node(cdk::and_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int and_end;

  node->left()->accept(this, lvl);
  _pf.DUP32();
  _pf.JZ(mklbl(and_end = ++_lbl));
  node->right()->accept(this, lvl);
  _pf.AND();

  _pf.ALIGN();
  _pf.LABEL(mklbl(and_end));
}

void gr8::postfix_writer::do_or_node(cdk::or_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int or_first_true, or_end;

  node->left()->accept(this, lvl);
  _pf.DUP32();
  _pf.INT(0);
  _pf.JNE(mklbl(or_first_true = ++_lbl));
  node->right()->accept(this, lvl);
  _pf.OR();
  _pf.JMP(mklbl(or_end = ++_lbl));
  _pf.ALIGN();
  _pf.LABEL(mklbl(or_first_true));
  _pf.INT(1);
  _pf.ALIGN();
  _pf.LABEL(mklbl(or_end));
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  if(one_time_flag == 0){  
      // these are just a few library function imports
    _pf.EXTERN("readi");
    _pf.EXTERN("printi");
    _pf.EXTERN("prints");
    _pf.EXTERN("println");
    _pf.EXTERN("printd");
    _pf.EXTERN("readd");
    one_time_flag = 1;
  }

  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if(node->type()->name() == basic_type::TYPE_INT)
    _pf.INT(node->value()); // push an integer
  else if(node->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.DOUBLE(node->value());
}

void gr8::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;

  /* generate the string */
  _pf.RODATA(); // strings are DATA readonly
  _pf.ALIGN(); // make sure we are aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
  _pf.SSTRING(node->value()); // output string characters

  /* leave the address on the stack */
  _pf.TEXT(); // return to the TEXT segment
  _pf.ADDR(mklbl(lbl1)); // the string to be printed
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  if(node->argument()->type()->name() == basic_type::TYPE_INT)
    _pf.NEG(); // 2-complement
  if(node->argument()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.DNEG();
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  if((node->type()->name() == basic_type::TYPE_DOUBLE) && (node->left()->type()->name() == basic_type::TYPE_INT))
    _pf.I2D();

  if(node->right()->type()->name() == basic_type::TYPE_POINTER){
    basic_type * aux1 = node->right()->type()->subtype();

    while(aux1->name() == basic_type::TYPE_POINTER){
      aux1 = aux1->subtype();
    }

    if(aux1->name() == basic_type::TYPE_DOUBLE){
      _pf.INT(8);
      _pf.MUL();
    }
  }

  node->right()->accept(this, lvl);
  if((node->type()->name() == basic_type::TYPE_DOUBLE) && (node->right()->type()->name() == basic_type::TYPE_INT))
    _pf.I2D();

  if(node->left()->type()->name() == basic_type::TYPE_POINTER){
    basic_type * aux1 = node->left()->type()->subtype();

    while(aux1->name() == basic_type::TYPE_POINTER){
      aux1 = aux1->subtype();
    }

    if(aux1->name() == basic_type::TYPE_DOUBLE){
      _pf.INT(8);
      _pf.MUL();
    }
  }
  
  if((node->type()->name() == basic_type::TYPE_INT) || (node->type()->name() == basic_type::TYPE_POINTER))
    _pf.ADD();
  else if(node->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.DADD();
}

void gr8::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);
  if((node->type()->name() == basic_type::TYPE_DOUBLE) && (node->left()->type()->name() == basic_type::TYPE_INT))
    _pf.I2D();

  node->right()->accept(this, lvl);
  if((node->type()->name() == basic_type::TYPE_DOUBLE) && (node->right()->type()->name() == basic_type::TYPE_INT))
    _pf.I2D();

  if((node->type()->name() == basic_type::TYPE_INT) || (node->type()->name() == basic_type::TYPE_POINTER))
    _pf.SUB();
  else if (node->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.DSUB();
}
void gr8::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);
  if((node->type()->name() == basic_type::TYPE_DOUBLE) && (node->left()->type()->name() == basic_type::TYPE_INT))
    _pf.I2D();

  node->right()->accept(this, lvl);
  if((node->type()->name() == basic_type::TYPE_DOUBLE) && (node->right()->type()->name() == basic_type::TYPE_INT))
    _pf.I2D();

  if(node->type()->name() == basic_type::TYPE_INT)
    _pf.MUL();
  if(node->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.DMUL();
}

void gr8::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);
  if((node->type()->name() == basic_type::TYPE_DOUBLE) && (node->left()->type()->name() == basic_type::TYPE_INT))
    _pf.I2D();

  node->right()->accept(this, lvl);
  if((node->type()->name() == basic_type::TYPE_DOUBLE) && (node->right()->type()->name() == basic_type::TYPE_INT))
    _pf.I2D();

  if(node->type()->name() == basic_type::TYPE_INT)
    _pf.DIV();
  if(node->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.DDIV();

}

void gr8::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}

void gr8::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);
  if((node->type()->name() == basic_type::TYPE_DOUBLE) && (node->left()->type()->name() == basic_type::TYPE_INT))
    _pf.I2D();

  node->right()->accept(this, lvl);
  if((node->type()->name() == basic_type::TYPE_DOUBLE) && (node->right()->type()->name() == basic_type::TYPE_INT))
    _pf.I2D();

  if(node->type()->name() == basic_type::TYPE_INT)
    _pf.LT();
  if(node->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.DCMP();
    _pf.INT(0);
    _pf.LT();
  }
}

void gr8::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LE();
}

void gr8::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GE();
}

void gr8::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  if((node->type()->name() == basic_type::TYPE_DOUBLE) && (node->left()->type()->name() == basic_type::TYPE_INT))
    _pf.I2D();

  node->right()->accept(this, lvl);
  if((node->type()->name() == basic_type::TYPE_DOUBLE) && (node->right()->type()->name() == basic_type::TYPE_INT))
    _pf.I2D();

  if(node->type()->name() == basic_type::TYPE_INT)
    _pf.GT();
  if(node->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.DCMP();
    _pf.INT(0);
    _pf.GT();
  }
}

void gr8::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.NE();
}

void gr8::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->left()->accept(this, lvl);
  if((node->type()->name() == basic_type::TYPE_DOUBLE) && (node->left()->type()->name() == basic_type::TYPE_INT))
    _pf.I2D();

  node->right()->accept(this, lvl);
  if((node->type()->name() == basic_type::TYPE_DOUBLE) && (node->right()->type()->name() == basic_type::TYPE_INT))
    _pf.I2D();

  if((node->type()->name() == basic_type::TYPE_INT) || (node->type()->name() == basic_type::TYPE_POINTER))
    _pf.EQ();
  if(node->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.DCMP();
    _pf.INT(0);
    _pf.EQ();
  }
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_identifier_node(cdk::identifier_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  std::shared_ptr<gr8::symbol> symbol = _symtab.find(node->name());
  int value  = symbol->offset();

  if(value != 1){
    _pf.LOCAL(value);
  }
  else
    _pf.ADDR(node->name());
}

void gr8::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl);
  if(node->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.LDDOUBLE();
  else{
    _pf.LDINT(); // depends on type size
  }
}

void gr8::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->rvalue()->accept(this, lvl); // determine the new value
  node->lvalue()->accept(this, lvl);

  if((node->rvalue()->type()->name() == basic_type::TYPE_INT) || (node->rvalue()->type()->name() == basic_type::TYPE_POINTER) || (node->rvalue()->type()->name() == basic_type::TYPE_STRING) || (node->rvalue()->type()->name() == basic_type::TYPE_STRUCT)){
    _pf.STINT();
  }
  else if(node->rvalue()->type()->name() == basic_type::TYPE_DOUBLE){ 
    _pf.STDOUBLE();
  }
}

//---------------------------------------------------------------------------

/*void gr8::postfix_writer::do_program_node(gr8::program_node * const node, int lvl) {
  // Note that Simple didn't have functions. Thus, it didn't need
  // a function node. However, it had to start in the main function.
  // program_node (representing the whole program) doubled as a
  // main function node.

  // generate the main function (RTS mandates that its name be "_main")
  _pf.TEXT();
  _pf.ALIGN();
  _pf.GLOBAL("_main", _pf.FUNC());
  _pf.LABEL("_main");
  _pf.ENTER(0);  // Simple didn't implement local variables

  node->statements()->accept(this, lvl);

  // end the main function
  _pf.INT(0);
  _pf.STFVAL32();
  _pf.LEAVE();
  _pf.RET();

  // these are just a few library function imports
  _pf.EXTERN("readi");
  _pf.EXTERN("printi");
  _pf.EXTERN("prints");
  _pf.EXTERN("println");
}*/

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_evaluation_node(gr8::evaluation_node * const node, int lvl) { 
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.TRASH(4); // delete the evaluated value
  } else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.TRASH(4); // delete the evaluated value's address
  } else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

void gr8::postfix_writer::do_print_node(gr8::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value to print
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  } else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  } else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.CALL("printd");
    _pf.TRASH(8);
  }

  if (node->newline()) {
  	_pf.CALL("println"); // print a newline
  }
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_read_node(gr8::read_node * const node, int lvl) { 
  ASSERT_SAFE_EXPRESSIONS;
  if((node->type()->name() == basic_type::TYPE_INT) || (node->type()->name() == basic_type::TYPE_STRING)){
    _pf.CALL("readi");
    _pf.LDFVAL32();
    _pf.STINT();
  }
  else if(node->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.CALL("readd");
    _pf.LDFVAL64();
    _pf.STDOUBLE();
  }

}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_sweeping_node(gr8::sweeping_node * const node, int lvl) {
   ASSERT_SAFE_EXPRESSIONS;

   int sweeping_condition, sweeping_end, sweeping_incr, aux1, aux2;

   //initialization
   node->begin_expression()->accept(this, lvl); // determine the new value
   _pf.DUP32();
   node->variable()->accept(this, lvl); // where to store the value
   _pf.STINT(); // store the value at address
   _pf.TRASH(4);

   _pf.ALIGN();
   _pf.LABEL(mklbl(sweeping_condition = ++_lbl));

   node->variable()->accept(this, lvl);
   _pf.LDINT();
   node->end_expression()->accept(this, lvl);
   //avaliar o by, se o by for positivo a condição é LE, caso contrário é GE (by > 0 ? LE:GE)
   
   node->increment()->accept(this, lvl);
   _pf.INT(0);
   _pf.GT();
   _pf.JZ(mklbl(aux1 = ++_lbl));
   _pf.LE();
   _pf.JMP(mklbl(aux2 = ++_lbl));
   _pf.ALIGN();
   _pf.LABEL(mklbl(aux1));
   _pf.GE();

   _pf.ALIGN();
   _pf.LABEL(mklbl(aux2));

   _pf.JZ(mklbl(sweeping_end = ++_lbl));
   stop_stack.push(mklbl(sweeping_end));

   again_stack.push(mklbl(sweeping_incr = ++_lbl));
   flag_ciclo = 1;
   node->block()->accept(this, lvl + 2);

   _pf.ALIGN();
   _pf.LABEL(mklbl(sweeping_incr));
   node->increment()->accept(this, lvl);
   node->variable()->accept(this, lvl);
   _pf.LDINT();
   _pf.ADD();
   node->variable()->accept(this, lvl);
   _pf.STINT();
   _pf.JMP(mklbl(sweeping_condition));

   _pf.ALIGN();
   _pf.LABEL(mklbl(sweeping_end));
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_return_node(gr8::return_node * const node, int lvl) {
  if(funcao.compare("") == 0){
    std::cerr << "ERROR: RETURN OUT OF A FUNCTION" << std::endl;
    exit(1);
  }

  ASSERT_SAFE_EXPRESSIONS;
  std::shared_ptr<gr8::symbol> symbol = _symtab.find(funcao);

  if((node->argument() != nullptr) && (symbol->type() == nullptr)){
    std::cerr << "ERROR: RETURN TYPE DIFFERS FROM FUNCTION TYPE" << std::endl;
    exit(1);
  }
  else if(node->argument() != nullptr){
    node->argument()->accept(this, lvl);

    if(symbol != nullptr){
      if((node->argument()->type()->name() == basic_type::TYPE_INT) && (symbol->type()->name() == basic_type::TYPE_DOUBLE)){
        _pf.I2D();
      }
      else if(node->argument()->type()->name() != symbol->type()->name()){
        std::cerr << "ERROR: RETURN TYPE DIFFERS FROM FUNCTION TYPE" << std::endl;
        exit(1);
      }
    }
  }

  _pf.JMP(funcao + "end");
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_stop_node(gr8::stop_node * const node, int lvl) {
  if(flag_ciclo == 0){
    std::cerr << "ERROR: STOP OUT OF A CYCLE" << std::endl;
    exit(1);
  }

  unsigned int value = node->argument();
  if(value > stop_stack.size()){
    std::cerr << "ERROR: INVALID ARGUMENT FOR STOP" << std::endl;
    exit(1);
  }
  value = value - 1;
  while(value != 0){
    stop_stack.pop();
    value = value - 1;
  }

  std::string jump_label = stop_stack.top();
  _pf.JMP(jump_label);

}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_again_node(gr8::again_node * const node, int lvl) {
  if(flag_ciclo == 0){
    std::cerr << "ERROR: AGAIN OUT OF A CYCLE" << std::endl;
    exit(1);
  }

  unsigned int value = node->argument();
  if(value > again_stack.size()){
    std::cerr << "ERROR: INVALID ARGUMENT FOR AGAIN" << std::endl;
    exit(1);
  }
  value = value - 1;
  while(value != 0){
    again_stack.pop();
    value = value - 1;
  }

  std::string jump_label = again_stack.top();
  _pf.JMP(jump_label);

}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_var_init_node(gr8::var_init_node * const node, int lvl) {
 	ASSERT_SAFE_EXPRESSIONS;
  if(new_symbol() != nullptr){ //simbolo inserido na tabela
    if(funcao.compare("") == 0){ //contexto global
      if(node->argument() == nullptr){ // variavel nao tem inicializacao 
        _pf.BSS();
        _pf.ALIGN();
        if(node->qualificador() == 1){
          _pf.GLOBAL(*(node->identifier()), _pf.OBJ());
        }
        _pf.LABEL(*(node->identifier()));

        if (node->type()->name() == basic_type::TYPE_INT || node->type()->name() == basic_type::TYPE_STRING || node->type()->name() == basic_type::TYPE_POINTER ) {
          _pf.SALLOC(4);
        }
        if(node->type()->name() == basic_type::TYPE_DOUBLE){
          _pf.SALLOC(8);
        }
      }
      else{ //variavel com inicializacao 
        if((node->type()->name() == basic_type::TYPE_INT) || (node->type()->name() == basic_type::TYPE_DOUBLE)){
          _pf.DATA();
          _pf.ALIGN();
          if(node->qualificador() == 1){
            _pf.GLOBAL(*(node->identifier()), _pf.OBJ());
          }
          _pf.LABEL(*(node->identifier()));

          if(node->type()->name() == basic_type::TYPE_INT){
            cdk::literal_node<int> * aux1 = dynamic_cast<cdk::literal_node<int>*>(node->argument());
            cdk::integer_node * aux = dynamic_cast<cdk::integer_node*>(aux1);
            int value = aux->value();
            _pf.SINT(value);
          }
          if(node->type()->name() == basic_type::TYPE_DOUBLE){

            if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {

              cdk::literal_node<double> * aux3 = dynamic_cast<cdk::literal_node<double>*>(node->argument());
              cdk::double_node * aux = dynamic_cast<cdk::double_node*>(aux3);
              double value = aux->value();
              _pf.SDOUBLE(value);
            }

            if (node->argument()->type()->name() == basic_type::TYPE_INT) {

              cdk::literal_node<int> * aux3 = dynamic_cast<cdk::literal_node<int>*>(node->argument());
              cdk::integer_node * aux = dynamic_cast<cdk::integer_node*>(aux3);
              int value = aux->value();
              _pf.SDOUBLE(value);
            }
          }
        }
        else if(node->type()->name() == basic_type::TYPE_STRING){
          cdk::literal_node<std::string> * aux2 = dynamic_cast<cdk::literal_node<std::string>*>(node->argument());
          cdk::string_node * aux = dynamic_cast<cdk::string_node*>(aux2);
          std::string value = aux->value();
          int lbl1; 

          _pf.DATA();
          _pf.ALIGN();
          _pf.LABEL(mklbl(lbl1 = ++_lbl));
          _pf.SSTRING(value);

          _pf.DATA();
          _pf.ALIGN();
          if(node->qualificador() == 1){
            _pf.GLOBAL(*(node->identifier()), _pf.OBJ());
          }
          _pf.LABEL(*(node->identifier()));
          _pf.SADDR(mklbl(lbl1));
        }
      }
    }
    else{ //contexto local
      if(flag_func_arguments == 1){ //quando visita os argumentos de uma funcao os offset sao positivos
        std::shared_ptr<gr8::symbol> symbol = new_symbol();
        symbol->set_offset(offset);
        offset = offset + node->type()->size();
      }
      else{
        std::shared_ptr<gr8::symbol> symbol = new_symbol();
        offset = offset - node->type()->size();
        symbol->set_offset(offset);
        if(node->argument() != nullptr){ //local com inicializacao
          if(node->type()->name() != basic_type::TYPE_POINTER){
            node->argument()->accept(this, lvl + 2);
            _pf.LOCAL(offset);
            if(node->type()->name() == basic_type::TYPE_INT || node->type()->name() == basic_type::TYPE_STRING )
              _pf.STINT();
            if(node->type()->name() == basic_type::TYPE_DOUBLE)
              _pf.STDOUBLE();
          }
        }
      }
    }
 	_pf.TEXT();
  }
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_objects_node(gr8::objects_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  size_t aux1 = node->type()->size();
  _pf.INT(aux1);
  _pf.MUL();
  _pf.ALLOC();
  _pf.SP();
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_block_node(gr8::block_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _symtab.push();
  if(node->var_declarations() != nullptr)
	 node->var_declarations()->accept(this, lvl);
  if(node->instructions() != nullptr)
	 node->instructions()->accept(this, lvl);
  _symtab.pop();
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_null_node(gr8::null_node * const node, int lvl) {
  
  _pf.INT(0); //null == 0
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_func_define_node(gr8::func_define_node * const node, int lvl) {

  ASSERT_SAFE_EXPRESSIONS;

  if (new_symbol() != nullptr) {
    std::vector<std::string>::iterator it = std::find(func_declares.begin(), func_declares.end(), *(node->identificador()));
    if(it != func_declares.end()){
      func_declares.erase(it);
    }

    _pf.TEXT();
    _pf.ALIGN();
  	if ((*(node->identificador())).compare("covfefe") == 0) {
	  	_pf.GLOBAL("_main", _pf.FUNC());
	  	_pf.LABEL("_main");

      gr8::function_visitor checker(_compiler); 
      (node)->accept(&checker, 0);
      int value = checker.get_total_size();
	  	_pf.ENTER(value);

	  }
  	else {
      if(node->qualificador() == 1){
  	  	_pf.GLOBAL(*(node->identificador()), _pf.FUNC());
      }
      _pf.LABEL(*(node->identificador()));
      
      gr8::function_visitor checker(_compiler); 
      (node)->accept(&checker, 0);
      int value = checker.get_total_size();
      _pf.ENTER(value);
  	}

    _symtab.push();
    funcao = *(node->identificador());
    if(node->vars() != nullptr){
      flag_func_arguments = 1;
      offset = 8;
      node->vars()->accept(this, lvl);
      flag_func_arguments = 0;
      offset = 0;
    }

  	node->block()->accept(this, lvl);
    offset = 0;

    _pf.ALIGN();
    _pf.LABEL(*(node->identificador()) + "end");
    funcao = "";
    _symtab.pop();

    if(node->type() != nullptr){
      if(node->type()->name() == basic_type::TYPE_DOUBLE){
        _pf.STFVAL64();
        _pf.LEAVE();
        _pf.RET();
      }
      else{
        _pf.STFVAL32();
        _pf.LEAVE();
        _pf.RET();
      }
    }
    else{
      _pf.LEAVE();
      _pf.RET();
    }

    if ((*(node->identificador())).compare("covfefe") == 0) {
      for(unsigned int i = 0; i < func_declares.size(); i++){
        _pf.EXTERN(func_declares.at(i));
      }
    }
  }
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_func_declare_node(gr8::func_declare_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  func_declares.insert(func_declares.end(),*(node->identificador()));
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_func_call_node(gr8::func_call_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int size = 0;

  if(new_symbol() != nullptr){
    if(node->expressions() != nullptr){

      std::string &id = *(node->identificador());
      std::shared_ptr<gr8::symbol> symbol = _symtab.find(id);
      std::vector<basic_type> * aux2 = symbol->declare_sequence();

      for (int i = node->expressions()->size() - 1; i >= 0; i--) { 
        node->expressions()->node(i)->accept(this, lvl);
        
        cdk::expression_node * aux1 = dynamic_cast<cdk::expression_node*>(node->expressions()->node(i));
        if((aux2->at(i).name() == basic_type::TYPE_DOUBLE) && aux1->type()->name() == basic_type::TYPE_INT)
          _pf.I2D();
        int value = aux1->type()->size();
        size = size + value;
      }
      
      _pf.CALL(*(node->identificador()));
      
      _pf.TRASH(size);
      std::shared_ptr<gr8::symbol> symbol1 = new_symbol();
      if(symbol1->type() != nullptr){
        if(symbol1->type()->name() == basic_type::TYPE_DOUBLE)
          _pf.LDFVAL64();
        else
          _pf.LDFVAL32();
      }
    }
    else{

      std::vector<std::string>::iterator it = std::find(func_declares.begin(), func_declares.end(), *(node->identificador()));
      if(it != func_declares.end()){
        _pf.EXTERN(*(node->identificador()));
      }
      _pf.CALL(*(node->identificador()));

      std::shared_ptr<gr8::symbol> symbol = new_symbol();
      if(symbol->type() != nullptr){
        if(symbol->type()->name() == basic_type::TYPE_DOUBLE)
          _pf.LDFVAL64();
        else
          _pf.LDFVAL32();
      }
    }
  }
  else{
    std::cerr << "ERROR: FUNCTION NOT KNOWN" << std::endl;
    exit(1);
  }

}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_pos_indication_node(gr8::pos_indication_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_index_node(gr8::index_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  int bytes;
  if ((node->type()->name() == basic_type::TYPE_INT) || (node->type()->name() == basic_type::TYPE_STRING)){
  	bytes = 4;
  }

  else if (node->type()->name() == basic_type::TYPE_DOUBLE) {
  	bytes = 8;
  }
  else {
  	std::cerr << "ERROR: INVALID TYPE!" << std::endl;
    exit(1);
  }

  node->identifier()->accept(this, lvl + 2); //ja coloca na pilha o endereco onde ir buscar a info
  node->argument()->accept(this, lvl + 2);

  _pf.INT(bytes);
  _pf.MUL();
  _pf.ADD();
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_identity_node(gr8::identity_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_if_node(gr8::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_if_else_node(gr8::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1 = lbl2));
}