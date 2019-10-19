#include <string>
#include "targets/xml_writer.h"
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

//---------------------------------------------------------------------------

void gr8::xml_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<" << node->label() << ">" << "</" << node->label() << ">" << std::endl;
}

void gr8::xml_writer::do_data_node(cdk::data_node * const node, int lvl) {
  //EMPTY
}

void gr8::xml_writer::do_double_node(cdk::double_node * const node, int lvl) {
  process_literal(node, lvl);
}
void gr8::xml_writer::do_not_node(cdk::not_node * const node, int lvl) {
  do_unary_expression(node, lvl);
}
void gr8::xml_writer::do_and_node(cdk::and_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_or_node(cdk::or_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<sequence_node size='" << node->size() << "'>" << std::endl;
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  process_literal(node, lvl);
}

void gr8::xml_writer::do_string_node(cdk::string_node * const node, int lvl) {
  process_literal(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_unary_expression(cdk::unary_expression_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  do_unary_expression(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_binary_expression(cdk::binary_expression_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_add_node(cdk::add_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_div_node(cdk::div_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_le_node(cdk::le_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_identifier_node(cdk::identifier_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label() << ">" << node->name() << "</" << node->label() << ">" << std::endl;
  closeTag(node, lvl);
}

void gr8::xml_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);

  node->lvalue()->accept(this, lvl);
  reset_new_symbol();

  node->rvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}


//---------------------------------------------------------------------------

void gr8::xml_writer::do_evaluation_node(gr8::evaluation_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_print_node(gr8::print_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag("argument", lvl);

  if(node->argument()!= nullptr)
    node->argument()->accept(this, lvl + 2);

  closeTag("argument", lvl + 2);
  if (node->newline()) {
    os() << std::string(lvl + 2, ' ') << "<newline/>" << std::endl;
  }
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_read_node(gr8::read_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label() << ">" << "</" << node->label() << ">" << std::endl;
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_sweeping_node(gr8::sweeping_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("begin_expression", lvl + 2);
  node->begin_expression()->accept(this, lvl + 4);
  closeTag("begin_expression", lvl + 2);
  openTag("end_expression", lvl + 2);
  node->end_expression()->accept(this, lvl + 4);
  closeTag("end_expression", lvl + 2);

  openTag("increment", lvl + 2);
  if(node->increment()!=nullptr)
    node->increment()->accept(this, lvl + 4);
  closeTag("increment", lvl + 2);

  openTag("variable", lvl + 2);
  node->variable()->accept(this, lvl + 4);
  closeTag("variable", lvl + 2);
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_return_node(gr8::return_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);

  openTag("argument", lvl + 2);
  if(node->argument()!=nullptr)
    node->argument()->accept(this, lvl + 4);
  closeTag("argument", lvl + 2);

  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_var_init_node(gr8::var_init_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<var_init_node qualificador='" << node->qualificador() << std::endl;
  os() << std::string(lvl, ' ') << "type='" << node->type()->name() << std::endl;
  os() << std::string(lvl, ' ') << "identificador='" << *(node->identifier()) << "'>" << std::endl;
  openTag("argument", lvl + 2);
  if(node->argument()!=nullptr)
    node->argument()->accept(this, lvl + 4);
  closeTag("argument", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_stop_node(gr8::stop_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<stop_init_node argument='" << node->argument() << "'>" << std::endl;
  closeTag(node, lvl);

}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_again_node(gr8::again_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label() << ">" << node->argument() << "</" << node->label() << ">" << std::endl;
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_func_declare_node(gr8::func_declare_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<func_declare_node qualificador='" << node->qualificador() << std::endl;
  if(node->type()!=nullptr)
    os() << std::string(lvl, ' ') << "type='" << node->type()->name() << std::endl;
  os() << std::string(lvl, ' ') << "identificador='" << *(node->identificador()) << "'>" << std::endl;
  openTag("vars", lvl + 2);
  if(node->vars()!=nullptr)
    node->vars()->accept(this, lvl + 4);
  closeTag("vars", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_block_node(gr8::block_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("var_declarations", lvl + 2);
  if(node->var_declarations()!=nullptr)
    node->var_declarations()->accept(this, lvl + 4);
  closeTag("var_declarations", lvl + 2);

  openTag("instructions", lvl + 2);
  if(node->instructions()!= nullptr)
    node->instructions()->accept(this, lvl + 4);
  closeTag("instructions", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_func_define_node(gr8::func_define_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<func_define_node qualificador='" << node->qualificador() << std::endl;
  if(node->type()!=nullptr)
    os() << std::string(lvl, ' ') << "type='" << node->type()->name() << std::endl;
  os() << std::string(lvl, ' ') << "identificador='" << *(node->identificador()) << "'>" << std::endl;
  openTag("vars", lvl + 2);
  if(node->vars()!= nullptr){
    node->vars()->accept(this, lvl + 4);
  }
  closeTag("vars", lvl);
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_func_call_node(gr8::func_call_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<func_call identificador='" << *(node->identificador()) << "'>" << std::endl;
  openTag("expressions", lvl + 2);
  if(node->expressions()!= nullptr)
    node->expressions()->accept(this, lvl + 4);
  closeTag("expressions", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_pos_indication_node(gr8::pos_indication_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("argument", lvl + 2);
  node->argument()->accept(this, lvl + 4);
  closeTag("argument", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_identity_node(gr8::identity_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  do_unary_expression(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_index_node(gr8::index_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("argument", lvl + 2);
  node->argument()->accept(this, lvl + 4);
  closeTag("argument", lvl + 2);
  openTag("identifier", lvl + 2);
  node->identifier()->accept(this, lvl + 4);
  closeTag("identifier", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_objects_node(gr8::objects_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  do_unary_expression(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_null_node(gr8::null_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label() << ">" << "</" << node->label() << ">" << std::endl;
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_if_node(gr8::if_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_if_else_node(gr8::if_else_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->thenblock()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  openTag("else", lvl + 2);
  node->elseblock()->accept(this, lvl + 4);
  closeTag("else", lvl + 2);
  closeTag(node, lvl);
}
