
//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
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
/// \file ImportLibrary.cpp
/// \brief Header file for ImportLibrary, H3D scene-graph node.
///
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ImportLibrary.h>
#include <H3DUtil/DynamicLibrary.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ImportLibrary::database( 
                                    "ImportLibrary", 
                                    &(newInstance<ImportLibrary>), 
                                    typeid( ImportLibrary ),
                                    &X3DChildNode::database );

namespace TransformInternals {
  FIELDDB_ELEMENT( ImportLibrary, url, INITIALIZE_ONLY );
  // for backwards compability
  FIELDDB_ELEMENT_EX( ImportLibrary, url, INITIALIZE_ONLY, library  ); 
}

ImportLibrary::ImportLibrary( Inst< SFNode >  _metadata,
                              Inst< MFString > _url ):
  X3DChildNode( _metadata ),
  X3DUrlObject( _url ) {
  
  type_name = "ImportLibrary";
  database.initFields( this );
}

void ImportLibrary::initialize() {
  for( MFString::const_iterator i = url->begin();
       i != url->end();
       i++ ) {
    DynamicLibrary::LIBHANDLE handle;
#ifdef WIN32
    bool ends_in_dll = (*i).find( ".dll" ) != string::npos;

#ifdef _DEBUG

    if( !ends_in_dll ) {
      // try relative path first
      handle = DynamicLibrary::load( url_base + (*i) + "_d" );
      if( handle ) return;
    }

    // try absolute path.
    handle = DynamicLibrary::load( (*i) + "_d" );
    if( handle ) return;
    
#endif // _DEBUG

    if( !ends_in_dll ) {
      // try as relative path first
      handle = DynamicLibrary::load( url_base + (*i) );
      if( handle ) return;
    }

#endif // WIN32

    // test the given name directly.
    string filename = resolveURLAsFile( *i);
    if( filename == "" ) filename = (*i);
    handle =  DynamicLibrary::load( filename );

    if( !handle )
      Console(4) << "WARNING: Could not load dynamic library \"" 
                 << *i << "\" specified in " << getName() 
                 << " (" << DynamicLibrary::getLastError() << ")." << endl;
  }
}
