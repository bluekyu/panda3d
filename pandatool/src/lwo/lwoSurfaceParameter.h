// Filename: lwoSurfaceParameter.h
// Created by:  drose (24Apr01)
// 
////////////////////////////////////////////////////////////////////

#ifndef LWOSURFACEPARAMETER_H
#define LWOSURFACEPARAMETER_H

#include <pandatoolbase.h>

#include "lwoChunk.h"

////////////////////////////////////////////////////////////////////
//       Class : LwoSurfaceParameter
// Description : Records some parameter value of a surface material,
//               as an entry within a LwoSurface chunk.  The meaning
//               of the value is determined by the id of this chunk.
////////////////////////////////////////////////////////////////////
class LwoSurfaceParameter : public LwoChunk {
public:
  float _value;
  int _envelope;

public:
  virtual bool read_iff(IffInputFile *in, size_t stop_at);
  virtual void write(ostream &out, int indent_level = 0) const;
  
public:
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    LwoChunk::init_type();
    register_type(_type_handle, "LwoSurfaceParameter",
                  LwoChunk::get_class_type());
  }

private:
  static TypeHandle _type_handle;
};

#endif

  
