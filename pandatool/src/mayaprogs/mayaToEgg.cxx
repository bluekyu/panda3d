// Filename: mayaToEgg.cxx
// Created by:  drose (15Feb00)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) 2001, Disney Enterprises, Inc.  All rights reserved
//
// All use of this software is subject to the terms of the Panda 3d
// Software license.  You should have received a copy of this license
// along with this source code; you will also find a current copy of
// the license at http://www.panda3d.org/license.txt .
//
// To contact the maintainers of this program write to
// panda3d@yahoogroups.com .
//
////////////////////////////////////////////////////////////////////

#include "mayaToEgg.h"
#include "mayaToEggConverter.h"
#include "config_mayaegg.h"
#include "config_maya.h"  // for maya_cat

////////////////////////////////////////////////////////////////////
//     Function: MayaToEgg::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
MayaToEgg::
MayaToEgg() :
  SomethingToEgg("Maya", ".mb")
{
  add_path_replace_options();
  add_path_store_options();
  add_animation_options();
  add_units_options();
  add_normals_options();
  add_transform_options();

  set_program_description
    ("This program converts Maya model files to egg.  Nothing fancy yet.");

  add_option
    ("p", "", 0,
     "Generate polygon output only.  Tesselate all NURBS surfaces to "
     "polygons via the built-in Maya tesselator.  The tesselation will "
     "be based on the tolerance factor given by -ptol.",
     &MayaToEgg::dispatch_none, &_polygon_output);

  add_option
    ("ptol", "tolerance", 0,
     "Specify the fit tolerance for Maya polygon tesselation.  The smaller "
     "the number, the more polygons will be generated.  The default is "
     "0.01.",
     &MayaToEgg::dispatch_double, NULL, &_polygon_tolerance);

  add_option
    ("bface", "", 0,
     "Respect the Maya \"double sided\" rendering flag to indicate whether "
     "polygons should be double-sided or single-sided.  Since this flag "
     "is set to double-sided by default in Maya, it is often better to "
     "ignore this flag (unless your modelers are diligent in turning it "
     "off where it is not desired).  If this flag is not specified, the "
     "default is to treat all polygons as single-sided, unless an "
     "egg object type of \"double-sided\" is set.",
     &MayaToEgg::dispatch_none, &_respect_maya_double_sided);

  add_option
    ("trans", "type", 0,
     "Specifies which transforms in the Maya file should be converted to "
     "transforms in the egg file.  The option may be one of all, model, "
     "dcs, or none.  The default is model, which means only transforms on "
     "nodes that have the model flag or the dcs flag are preserved.",
     &MayaToEgg::dispatch_transform_type, NULL, &_transform_type);

  add_option
    ("v", "", 0,
     "Increase verbosity.  More v's means more verbose.",
     &MayaToEgg::dispatch_count, NULL, &_verbose);

  _verbose = 0;
  _polygon_tolerance = 0.01;
  _transform_type = MayaToEggConverter::TT_model;
}

////////////////////////////////////////////////////////////////////
//     Function: MayaToEgg::run
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
void MayaToEgg::
run() {
  // Set the verbose level by using Notify.
  if (_verbose >= 3) {
    maya_cat->set_severity(NS_spam);
    mayaegg_cat->set_severity(NS_spam);
  } else if (_verbose >= 2) {
    maya_cat->set_severity(NS_debug);
    mayaegg_cat->set_severity(NS_debug);
  } else if (_verbose >= 1) {
    maya_cat->set_severity(NS_info);
    mayaegg_cat->set_severity(NS_info);
  }

  // Let's convert the output file to a full path before we initialize
  // Maya, since Maya now has a nasty habit of changing the current
  // directory.
  if (_got_output_filename) {
    _output_filename.make_absolute();
  }

  nout << "Initializing Maya.\n";
  MayaToEggConverter converter(_program_name);
  if (!converter.open_api()) {
    nout << "Unable to initialize Maya.\n";
    exit(1);
  }

  // Copy in the command-line parameters.
  converter._polygon_output = _polygon_output;
  converter._polygon_tolerance = _polygon_tolerance;
  converter._respect_maya_double_sided = _respect_maya_double_sided;
  converter._transform_type = _transform_type;

  // Copy in the path and animation parameters.
  apply_parameters(converter);

  // Set the coordinate system to match Maya's.
  if (!_got_coordinate_system) {
    _coordinate_system = converter._maya->get_coordinate_system();
  }
  _data.set_coordinate_system(_coordinate_system);

  // Use the standard Maya units, if the user didn't specify
  // otherwise.  This always returns centimeters, which is the way all
  // Maya files are stored internally (and is the units returned by
  // all of the API functions called here).
  if (_input_units == DU_invalid) {
    _input_units = converter._maya->get_units();
  }

  converter.set_egg_data(&_data, false);
  apply_parameters(converter);

  if (!converter.convert_file(_input_filename)) {
    nout << "Errors in conversion.\n";
    exit(1);
  }

  write_egg_file();
  nout << "\n";
}

////////////////////////////////////////////////////////////////////
//     Function: MayaToEgg::dispatch_transform_type
//       Access: Protected, Static
//  Description: Dispatches a parameter that expects a
//               MayaToEggConverter::TransformType option.
////////////////////////////////////////////////////////////////////
bool MayaToEgg::
dispatch_transform_type(const string &opt, const string &arg, void *var) {
  MayaToEggConverter::TransformType *ip = (MayaToEggConverter::TransformType *)var;
  (*ip) = MayaToEggConverter::string_transform_type(arg);

  if ((*ip) == MayaToEggConverter::TT_invalid) {
    nout << "Invalid type for -" << opt << ": " << arg << "\n"
         << "Valid types are all, model, dcs, and none.\n";
    return false;
  }

  return true;
}

int main(int argc, char *argv[]) {
  MayaToEgg prog;
  prog.parse_command_line(argc, argv);
  prog.run();
  return 0;
}
