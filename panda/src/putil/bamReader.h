/**
 * PANDA 3D SOFTWARE
 * Copyright (c) Carnegie Mellon University.  All rights reserved.
 *
 * All use of this software is subject to the terms of the revised BSD
 * license.  You should have received a copy of this license along
 * with this source code in a file named "LICENSE."
 *
 * @file bamReader.h
 * @author jason
 * @date 2000-06-12
 */

#ifndef __BAM_READER_
#define __BAM_READER_

#include "pandabase.h"
#include "pnotify.h"

#include "typedWritable.h"
#include "typedWritableReferenceCount.h"
#include "pointerTo.h"
#include "datagramGenerator.h"
#include "datagramIterator.h"
#include "bamReaderParam.h"
#include "bamEnums.h"
#include "subfileInfo.h"
#include "loaderOptions.h"
#include "factory.h"
#include "vector_int.h"
#include "pset.h"
#include "pmap.h"
#include "pdeque.h"
#include "dcast.h"
#include "pipelineCyclerBase.h"
#include "referenceCount.h"

#include <algorithm>


// A handy macro for reading PointerToArrays.
#define READ_PTA(Manager, source, Read_func, array)   \
{                                                     \
  void *t;                                            \
  if ((t = Manager->get_pta(source)) == (void*)NULL)  \
  {                                                   \
    array = Read_func(Manager, source);               \
    Manager->register_pta(array.get_void_ptr());      \
  }                                                   \
  else                                                \
  {                                                   \
    array.set_void_ptr(t);                            \
  }                                                   \
}

/**
 * Stores auxiliary data that may be piggybacked on the BamReader during each
 * object's read pass.  To use this, subclass BamReaderAuxData and add
 * whatever additional data you require.
 */
class EXPCL_PANDA_PUTIL BamReaderAuxData : public TypedReferenceCount {
public:
  INLINE BamReaderAuxData();

public:
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}
  static TypeHandle get_class_type() {
    return _type_handle;
  }

public:
  static void init_type() {
    TypedReferenceCount::init_type();
    register_type(_type_handle, "BamReaderAuxData",
                  TypedReferenceCount::get_class_type());
  }

private:
  static TypeHandle _type_handle;
};

/**
 * This is the fundamental interface for extracting binary objects from a Bam
 * file, as generated by a BamWriter.
 *
 * A Bam file can be thought of as a linear collection of objects.  Each
 * object is an instance of a class that inherits, directly or indirectly,
 * from TypedWritable.  The objects may include pointers to other objects
 * within the Bam file; the BamReader automatically manages these (with help
 * from code within each class) and restores the pointers correctly.
 *
 * This is the abstract interface and does not specifically deal with disk
 * files, but rather with a DatagramGenerator of some kind, which is simply a
 * linear source of Datagrams.  It is probably from a disk file, but it might
 * conceivably be streamed directly from a network or some such nonsense.
 *
 * Bam files are most often used to store scene graphs or subgraphs, and by
 * convention they are given filenames ending in the extension ".bam" when
 * they are used for this purpose.  However, a Bam file may store any
 * arbitrary list of TypedWritable objects; in this more general usage, they
 * are given filenames ending in ".boo" to differentiate them from the more
 * common scene graph files.
 *
 * See also BamFile, which defines a higher-level interface to read and write
 * Bam files on disk.
 */
class EXPCL_PANDA_PUTIL BamReader : public BamEnums {
public:
  typedef Factory<TypedWritable> WritableFactory;
  static BamReader *const Null;
  static WritableFactory *const NullFactory;

PUBLISHED:
  // The primary interface for a caller.
  explicit BamReader(DatagramGenerator *source = NULL);
  ~BamReader();

  void set_source(DatagramGenerator *source);
  INLINE DatagramGenerator *get_source();

  bool init();

  class AuxData;
  void set_aux_data(TypedWritable *obj, const string &name, AuxData *data);
  AuxData *get_aux_data(TypedWritable *obj, const string &name) const;

  INLINE const Filename &get_filename() const;

  INLINE const LoaderOptions &get_loader_options() const;
  INLINE void set_loader_options(const LoaderOptions &options);

  BLOCKING TypedWritable *read_object();
  BLOCKING bool read_object(TypedWritable *&ptr, ReferenceCount *&ref_ptr);

  INLINE bool is_eof() const;
  bool resolve();

  bool change_pointer(const TypedWritable *orig_pointer, const TypedWritable *new_pointer);

  INLINE int get_file_major_ver() const;
  INLINE int get_file_minor_ver() const;
  INLINE BamEndian get_file_endian() const;
  INLINE bool get_file_stdfloat_double() const;

  INLINE int get_current_major_ver() const;
  INLINE int get_current_minor_ver() const;

  EXTENSION(PyObject *get_file_version() const);

PUBLISHED:
  MAKE_PROPERTY(source, get_source, set_source);
  MAKE_PROPERTY(filename, get_filename);
  MAKE_PROPERTY(loader_options, get_loader_options, set_loader_options);

  MAKE_PROPERTY(file_version, get_file_version);
  MAKE_PROPERTY(file_endian, get_file_endian);
  MAKE_PROPERTY(file_stdfloat_double, get_file_stdfloat_double);

public:
  // Functions to support classes that read themselves from the Bam.

  bool read_pointer(DatagramIterator &scan);
  void read_pointers(DatagramIterator &scan, int count);
  void skip_pointer(DatagramIterator &scan);

  void read_file_data(SubfileInfo &info);

  void read_cdata(DatagramIterator &scan, PipelineCyclerBase &cycler);
  void read_cdata(DatagramIterator &scan, PipelineCyclerBase &cycler,
                  void *extra_data);

  void set_int_tag(const string &tag, int value);
  int get_int_tag(const string &tag) const;

  void set_aux_tag(const string &tag, BamReaderAuxData *value);
  BamReaderAuxData *get_aux_tag(const string &tag) const;

  void register_finalize(TypedWritable *whom);

  typedef TypedWritable *(*ChangeThisFunc)(TypedWritable *object, BamReader *manager);
  typedef PT(TypedWritableReferenceCount) (*ChangeThisRefFunc)(TypedWritableReferenceCount *object, BamReader *manager);
  void register_change_this(ChangeThisFunc func, TypedWritable *whom);
  void register_change_this(ChangeThisRefFunc func, TypedWritableReferenceCount *whom);

  void finalize_now(TypedWritable *whom);

  void *get_pta(DatagramIterator &scan);
  void register_pta(void *ptr);

  TypeHandle read_handle(DatagramIterator &scan);

  INLINE const FileReference *get_file();
  INLINE VirtualFile *get_vfile();
  INLINE streampos get_file_pos();

public:
  INLINE static void register_factory(TypeHandle type, WritableFactory::CreateFunc *func,
                                      void *user_data = NULL);
  INLINE static WritableFactory *get_factory();

PUBLISHED:
  EXTENSION(static void register_factory(TypeHandle handle, PyObject *func));

private:
  INLINE static void create_factory();

private:
  class PointerReference;

  void free_object_ids(DatagramIterator &scan);
  int read_object_id(DatagramIterator &scan);
  int read_pta_id(DatagramIterator &scan);
  int p_read_object();
  bool resolve_object_pointers(TypedWritable *object, PointerReference &pref);
  bool resolve_cycler_pointers(PipelineCyclerBase *cycler, const vector_int &pointer_ids,
                               bool require_fully_complete);
  void finalize();

  INLINE bool get_datagram(Datagram &datagram);

public:
  // Inherit from this class to piggyback additional temporary data on the
  // bamReader (via set_aux_data() and get_aux_data()) for any particular
  // objects during the bam reading process.
  class AuxData : public ReferenceCount {
  public:
    INLINE AuxData();
    virtual ~AuxData();
  };

private:
  static WritableFactory *_factory;

  DatagramGenerator *_source;
  bool _needs_init;

  bool _long_object_id;
  bool _long_pta_id;

  // This maps the type index numbers encountered within the Bam file to
  // actual TypeHandles.
  typedef phash_map<int, TypeHandle, int_hash> IndexMap;
  IndexMap _index_map;

  LoaderOptions _loader_options;

  // This maps the object ID numbers encountered within the Bam file to the
  // actual pointers of the corresponding generated objects.
  class CreatedObj {
  public:
    INLINE CreatedObj();
    INLINE ~CreatedObj();
    INLINE void set_ptr(TypedWritable *ptr, ReferenceCount *ref_ptr);

  public:
    bool _created;
    TypedWritable *_ptr;
    ReferenceCount *_ref_ptr;
    ChangeThisFunc _change_this;
    ChangeThisRefFunc _change_this_ref;
  };
  typedef phash_map<int, CreatedObj, int_hash> CreatedObjs;
  CreatedObjs _created_objs;
  // This is the iterator into the above map for the object we are currently
  // reading in p_read_object().  It is carefully maintained during recursion.
  // We need this so we can associate read_pointer() calls with the proper
  // objects.
  CreatedObjs::iterator _now_creating;
  // This is the pointer to the current PipelineCycler we are reading, if we
  // are within a read_cdata() call.
  PipelineCyclerBase *_reading_cycler;

  // This is the reverse lookup into the above map.
  typedef phash_map<const TypedWritable *, vector_int, pointer_hash> CreatedObjsByPointer;
  CreatedObjsByPointer _created_objs_by_pointer;

  // This records all the objects that still need their pointers completed,
  // along with the object ID's of the pointers they need, in the order in
  // which read_pointer() was called, so that we may call the appropriate
  // complete_pointers() later.
  typedef phash_map<PipelineCyclerBase *, vector_int, pointer_hash> CyclerPointers;
  typedef pmap<string, int> IntTags;
  typedef pmap<string, PT(BamReaderAuxData) > AuxTags;
  class PointerReference {
  public:
    vector_int _objects;
    CyclerPointers _cycler_pointers;
    IntTags _int_tags;
    AuxTags _aux_tags;
  };
  typedef phash_map<int, PointerReference, int_hash> ObjectPointers;
  ObjectPointers _object_pointers;

  // This is the number of extra objects that must still be read (and saved in
  // the _created_objs map) before returning from read_object().  It is only
  // used when read bam versions prior to 6.20.
  int _num_extra_objects;

  // The current nesting level.  We are not done reading an object until we
  // return to our starting nesting level.  It is only used when reading bam
  // versions of 6.20 or higher.
  int _nesting_level;

  // This is the set of all objects that registered themselves for
  // finalization.
  typedef phash_set<TypedWritable *, pointer_hash> Finalize;
  Finalize _finalize_list;

  // These are used by get_pta() and register_pta() to unify multiple
  // references to the same PointerToArray.
  typedef phash_map<int, void *, int_hash> PTAMap;
  PTAMap _pta_map;
  int _pta_id;

  // This is a queue of the currently-pending file data blocks that we have
  // recently encountered in the stream and still expect a subsequent object
  // to request.
  typedef pdeque<SubfileInfo> FileDataRecords;
  FileDataRecords _file_data_records;

  // This is used internally to record all of the new types created on-the-fly
  // to satisfy bam requirements.  We keep track of this just so we can
  // suppress warning messages from attempts to create objects of these types.
  typedef phash_set<TypeHandle> NewTypes;
  static NewTypes _new_types;

  // This is used in support of set_aux_data() and get_aux_data().
  typedef pmap<string, PT(AuxData)> AuxDataNames;
  typedef phash_map<TypedWritable *, AuxDataNames, pointer_hash> AuxDataTable;
  AuxDataTable _aux_data;

  int _file_major, _file_minor;
  BamEndian _file_endian;
  bool _file_stdfloat_double;
  static const int _cur_major;
  static const int _cur_minor;
};

typedef BamReader::WritableFactory WritableFactory;

// Useful function for taking apart the Factory Params in the static functions
// that need to be defined in each writable class that will be generated by a
// factory.  Sets the DatagramIterator and the BamReader pointers.
INLINE void
parse_params(const FactoryParams &params,
             DatagramIterator &scan, BamReader *&manager);

#include "bamReader.I"

#endif
