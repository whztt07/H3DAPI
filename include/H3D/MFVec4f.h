//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2014, SenseGraphics AB
//
//    This file is part of H3D API.
//
//    H3D API is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    H3D API is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with H3D API; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//    A commercial license is also available. Please contact us at 
//    www.sensegraphics.com for more information.
//
//
/// \file MFVec4f.h
/// \brief Contains the MFVec4f field class.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __MFVEC4F_H__
#define __MFVEC4F_H__

#include <H3D/MField.h>

namespace H3D {

  // template specializations in order to get nicer type printouts.
  template<>
  inline string MFieldBase< Vec4f >::classTypeName() { return "MFVec4f"; }

  /// \ingroup FieldTypes
  /// The MFVec4f field contains a vector of Vec4f.
  class H3DAPI_API MFVec4f: public MField< Vec4f > {
  public:
    MFVec4f(){}
    MFVec4f( size_type sz ): MField< Vec4f >( sz ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::MFVEC4F; }
  };
}

#endif

