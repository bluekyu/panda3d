// Filename: polygonOffsetAttribute.h
// Created by:  jason (12Jul00)
// 
////////////////////////////////////////////////////////////////////

#ifndef POLYGONOFFSETATTRIBUTE_H
#define POLYGONOFFSETATTRIBUTE_H

#include <pandabase.h>

#include "polygonOffsetProperty.h"

#include <onAttribute.h>

////////////////////////////////////////////////////////////////////
//       Class : PolygonOffsetAttribute
// Description : See PolygonOffsetTransition.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA PolygonOffsetAttribute : public OnAttribute {
public:
  INLINE PolygonOffsetAttribute();

  INLINE void set_units(int units);
  INLINE int get_units() const;
  INLINE void set_factor(int factor);
  INLINE int get_factor() const;

  virtual TypeHandle get_handle() const;
  virtual NodeAttribute *make_copy() const;
  virtual NodeAttribute *make_initial() const;

  virtual void issue(GraphicsStateGuardianBase *gsgbase);

protected:
  virtual void set_value_from(const OnTransition *other);
  virtual int compare_values(const OnAttribute *other) const;
  virtual void output_value(ostream &out) const;
  virtual void write_value(ostream &out, int indent_level) const;

  PolygonOffsetProperty _state;

public:
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    OnAttribute::init_type();
    register_type(_type_handle, "PolygonOffsetAttribute",
                  OnAttribute::get_class_type());
  }

private:
  static TypeHandle _type_handle;
};

#include "polygonOffsetAttribute.I"

#endif
