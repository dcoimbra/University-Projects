#ifndef __GR8_SEMANTICS_SYMBOL_H__
#define __GR8_SEMANTICS_SYMBOL_H__

#include <string>
#include <cdk/basic_type.h>
#include <vector>

namespace gr8 {

  class symbol {
    basic_type *_type;
    std::string _name;
    long _value; // hack!
    std::string _functionality;
    std::vector<basic_type> * _declare_sequence;
    int _offset = 1; //se algum simbolo tiver offset a um ou e global ou e uma funcao 

  public:
    symbol(basic_type *type, const std::string &name, long value, const std::string functionality, std::vector<basic_type> *v) :
        _type(type), _name(name), _value(value), _functionality(functionality), _declare_sequence(v) {
    }

    virtual ~symbol() {
      delete _type;
    }

    basic_type *type() const {
      return _type;
    }
    const std::string &name() const {
      return _name;
    }
    long value() const {
      return _value;
    }
    long value(long v) {
      return _value = v;
    }
    const std::string functionality() const{
      return _functionality;
    }

    std::vector<basic_type> *declare_sequence(){
      return _declare_sequence;
    }

    void insert_declare_sequence(basic_type b){
      _declare_sequence->push_back(b);
    }

    void set_offset(int offset){
      _offset = offset;
    }

    int offset(){
      return _offset;
    }


  };

} // gr8

#endif
