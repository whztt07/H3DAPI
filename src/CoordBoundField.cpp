//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
//
//    This file is part of H3D API.
//
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
/// \file CoordBoundField.cpp
/// \brief CPP file for CoordBoundField, specialiced field class.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/CoordBoundField.h>
#include <H3D/Coordinate.h>
using namespace H3D;

//TODO: Fix so that it works for all X3DCoordinateNode
void CoordBoundField::update() {
  Coordinate *c = 
    dynamic_cast< Coordinate * >( static_cast< TypedSFNode< X3DCoordinateNode> * >
                                  ( routes_in[0] )->getValue() );
 
  BoxBound *bound = new BoxBound();
  if ( c )
    bound->fitAroundPoints( c->point->begin(), c->point->end() );
  value = bound;
}
