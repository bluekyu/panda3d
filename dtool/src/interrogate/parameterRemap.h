// Filename: parameterRemap.h
// Created by:  drose (01Aug00)
// 
////////////////////////////////////////////////////////////////////

#ifndef PARAMETERREMAP_H
#define PARAMETERREMAP_H

#include <dtoolbase.h>

#include <interrogate_interface.h>

class CPPType;
class CPPExpression;

////////////////////////////////////////////////////////////////////
//       Class : ParameterRemap
// Description : An abstract base class for a number of different
//               kinds of ways to remap parameters for passing to
//               wrapper functions.
//
//               Certain kinds of function parameters that are legal
//               in C++ (for instance, passing by reference, or
//               passing structures as concrete values) are not legal
//               for a typical scripting language.  We map these types
//               of parameters to something equivalent (for instance,
//               a reference becomes a pointer).
//
//               For each kind of possible remapping, we define a
//               class derived from ParameterRemap that defines the
//               exact nature of the remap.
////////////////////////////////////////////////////////////////////
class ParameterRemap {
public:
  INLINE ParameterRemap(CPPType *orig_type);
  virtual ~ParameterRemap();

  INLINE bool is_valid() const;

  INLINE CPPType *get_orig_type() const;
  INLINE CPPType *get_new_type() const;
  INLINE CPPType *get_temporary_type() const;
  INLINE bool has_default_value() const;
  INLINE CPPExpression *get_default_value() const;
  INLINE void set_default_value(CPPExpression *expr);

  virtual void pass_parameter(ostream &out, const string &variable_name);
  virtual string prepare_return_expr(ostream &out, int indent_level, 
                                     const string &expression);
  virtual string get_return_expr(const string &expression);
  virtual string temporary_to_return(const string &temporary);
  virtual bool return_value_needs_management();
  virtual FunctionIndex get_return_value_destructor();
  virtual bool return_value_should_be_simple();
  virtual bool new_type_is_atomic_string();
  virtual bool is_this();

protected:
  bool _is_valid;

  CPPType *_orig_type;
  CPPType *_new_type;
  CPPType *_temporary_type;
  CPPExpression *_default_value;
};

#include "parameterRemap.I"

#endif
