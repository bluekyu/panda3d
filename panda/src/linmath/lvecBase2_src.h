// Filename: lvecBase2_src.h
// Created by:  drose (08Mar00)
// 
////////////////////////////////////////////////////////////////////

//typedef struct {FLOATTYPE _0, _1} FLOATNAME(data);


////////////////////////////////////////////////////////////////////
//       Class : LVecBase2
// Description : This is the base class for all two-component
//               vectors and points.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA FLOATNAME(LVecBase2) {
PUBLISHED:
  typedef const FLOATTYPE *iterator;
  typedef const FLOATTYPE *const_iterator;

  INLINE_LINMATH FLOATNAME(LVecBase2)();
  INLINE_LINMATH FLOATNAME(LVecBase2)(const FLOATNAME(LVecBase2) &copy);
  INLINE_LINMATH FLOATNAME(LVecBase2) &operator = (const FLOATNAME(LVecBase2) &copy);
  INLINE_LINMATH FLOATNAME(LVecBase2) &operator = (FLOATTYPE fill_value);
  INLINE_LINMATH FLOATNAME(LVecBase2)(FLOATTYPE fill_value);
  INLINE_LINMATH FLOATNAME(LVecBase2)(FLOATTYPE x, FLOATTYPE y);

  INLINE_LINMATH static const FLOATNAME(LVecBase2) &zero();
  INLINE_LINMATH static const FLOATNAME(LVecBase2) &unit_x();
  INLINE_LINMATH static const FLOATNAME(LVecBase2) &unit_y();

  INLINE_LINMATH ~FLOATNAME(LVecBase2)();

  INLINE_LINMATH FLOATTYPE operator [](int i) const;
  INLINE_LINMATH FLOATTYPE &operator [](int i);

  INLINE_LINMATH bool is_nan() const;

  INLINE_LINMATH FLOATTYPE get_cell(int i) const;
  INLINE_LINMATH FLOATTYPE get_x() const;
  INLINE_LINMATH FLOATTYPE get_y() const;
  INLINE_LINMATH void set_cell(int i, FLOATTYPE value);
  INLINE_LINMATH void set_x(FLOATTYPE value);
  INLINE_LINMATH void set_y(FLOATTYPE value);

  INLINE_LINMATH const FLOATTYPE *get_data() const;
  INLINE_LINMATH int get_num_components() const;

public:
  INLINE_LINMATH iterator begin();
  INLINE_LINMATH iterator end();

  INLINE_LINMATH const_iterator begin() const;
  INLINE_LINMATH const_iterator end() const;

PUBLISHED:
  INLINE_LINMATH void fill(FLOATTYPE fill_value);
  INLINE_LINMATH void set(FLOATTYPE x, FLOATTYPE y);

  INLINE_LINMATH FLOATTYPE dot(const FLOATNAME(LVecBase2) &other) const;

  INLINE_LINMATH bool operator < (const FLOATNAME(LVecBase2) &other) const;
  INLINE_LINMATH bool operator == (const FLOATNAME(LVecBase2) &other) const;
  INLINE_LINMATH bool operator != (const FLOATNAME(LVecBase2) &other) const;

  INLINE_LINMATH int compare_to(const FLOATNAME(LVecBase2) &other) const;
  INLINE_LINMATH int compare_to(const FLOATNAME(LVecBase2) &other,
                                FLOATTYPE threshold) const;


  INLINE_LINMATH FLOATNAME(LVecBase2) operator - () const;

  INLINE_LINMATH FLOATNAME(LVecBase2)
  operator + (const FLOATNAME(LVecBase2) &other) const;
  INLINE_LINMATH FLOATNAME(LVecBase2)
  operator - (const FLOATNAME(LVecBase2) &other) const;

  INLINE_LINMATH FLOATNAME(LVecBase2) operator * (FLOATTYPE scalar) const;
  INLINE_LINMATH FLOATNAME(LVecBase2) operator / (FLOATTYPE scalar) const;

  INLINE_LINMATH void operator += (const FLOATNAME(LVecBase2) &other);
  INLINE_LINMATH void operator -= (const FLOATNAME(LVecBase2) &other);

  INLINE_LINMATH void operator *= (FLOATTYPE scalar);
  INLINE_LINMATH void operator /= (FLOATTYPE scalar);

  INLINE_LINMATH bool almost_equal(const FLOATNAME(LVecBase2) &other, 
                           FLOATTYPE threshold) const;
  INLINE_LINMATH bool almost_equal(const FLOATNAME(LVecBase2) &other) const;

  INLINE_LINMATH void output(ostream &out) const;

public:
  INLINE_LINMATH void generate_hash(ChecksumHashGenerator &hash) const;
  INLINE_LINMATH void generate_hash(ChecksumHashGenerator &hash,
                                    FLOATTYPE threshold) const;

public:
   union {
        FLOATTYPE data[2];
        struct {FLOATTYPE _0, _1;} v;
   } _v;

private:
  static const FLOATNAME(LVecBase2) _zero;
  static const FLOATNAME(LVecBase2) _unit_x;
  static const FLOATNAME(LVecBase2) _unit_y;

public:
  INLINE_LINMATH void write_datagram(Datagram &destination) const;
  INLINE_LINMATH void read_datagram(DatagramIterator &source);

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type();
 
private:
  static TypeHandle _type_handle;
};


INLINE_LINMATH ostream &operator << (ostream &out, const FLOATNAME(LVecBase2) &vec) {
  vec.output(out);
  return out;
}

#include "lvecBase2_src.I"
