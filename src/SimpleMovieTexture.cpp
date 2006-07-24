//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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
/// \file SimpleMovieTexture.cpp
/// \brief CPP file for SimpleMovieTexture.
///
//
//////////////////////////////////////////////////////////////////////////////


#include "SimpleMovieTexture.h"
#include "DirectShowDecoder.h"

using namespace H3D;

H3DNodeDatabase SimpleMovieTexture::database( 
        "SimpleMovieTexture", 
        &(newInstance< SimpleMovieTexture > ),
        typeid( SimpleMovieTexture ),
        &H3DVideoTextureNode::database 
        );

namespace SimpleMovieTextureInternals {
  FIELDDB_ELEMENT( SimpleMovieTexture, play, INPUT_ONLY );
  FIELDDB_ELEMENT( SimpleMovieTexture, stop, INPUT_ONLY );
  FIELDDB_ELEMENT( SimpleMovieTexture, pause, INPUT_ONLY );
  FIELDDB_ELEMENT( SimpleMovieTexture, length, OUTPUT_ONLY );
  FIELDDB_ELEMENT( SimpleMovieTexture, frameRate, OUTPUT_ONLY );
  FIELDDB_ELEMENT( SimpleMovieTexture, width, OUTPUT_ONLY );
  FIELDDB_ELEMENT( SimpleMovieTexture, height, OUTPUT_ONLY );
  FIELDDB_ELEMENT( SimpleMovieTexture, loop, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SimpleMovieTexture, playAudio, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SimpleMovieTexture, url, INPUT_OUTPUT );
}


SimpleMovieTexture::SimpleMovieTexture( 
                                   Inst< DisplayList > _displayList,
                                   Inst< SFNode  > _metadata,
                                   Inst< SFBool  > _repeatS,
                                   Inst< SFBool  > _repeatT,
                                   Inst< SFBool  > _scaleToP2,
                                   Inst< SFImage > _image,
                                   Inst< SFBool  > _interpolate,
                                   Inst< SFBool > _play,
                                   Inst< SFBool  > _stop,
                                   Inst< SFBool > _pause,
                                   Inst< SFTime      > _length,
                                   Inst< SFFloat     > _frameRate,
                                   Inst< SFBool      > _playAudio,
                                   Inst< SFBool      > _loop,
                                   Inst< SFInt32     > _width,
                                   Inst< SFInt32     > _height,
                                   Inst< MFString >  _url ):
  H3DVideoTextureNode( _displayList, _metadata, _repeatS, _repeatT, _scaleToP2,
                       _image, _interpolate ),
  X3DUrlObject( _url ),
  play( _play ),
  stop( _stop ),
  pause( _pause ),
  length( _length ),
  frameRate( _frameRate ),
  playAudio( _playAudio ),
  loop( _loop ),
  width( _width ),
  height( _height ),
  decoderManager( new DecoderManager ) {
  type_name = "SimpleMovieTexture";
  database.initFields( this );
  HRESULT hr = S_OK;
  decoder.reset( new DirectShowDecoder() ); 
  
  decoderManager->setOwner( this );
  
  frameRate->setValue( 0, id );
  playAudio->setValue( true, id );
  loop->setValue( false, id );
  width->setValue( 0, id );
  height->setValue( 0, id );
  length->setValue( 0, id );

  play->routeNoEvent( decoderManager, id );
  stop->routeNoEvent( decoderManager, id );
  pause->routeNoEvent( decoderManager, id );
  loop->routeNoEvent( decoderManager, id );
  url->routeNoEvent( decoderManager, id );
}


void SimpleMovieTexture::DecoderManager::update() {
  SimpleMovieTexture *tex = static_cast< SimpleMovieTexture * >( getOwner() );
  if( event.ptr == routes_in[0] ) {
    // play
    if( static_cast< SFBool * >( routes_in[0] )->getValue( tex->id ) ) {
      tex->decoder->startPlaying();
    }
  } else if( event.ptr == routes_in[1] ) {
    // stop
    if( static_cast< SFBool * >( routes_in[1] )->getValue( tex->id ) ) {
       tex->decoder->stopPlaying();
    }
  } else if( event.ptr == routes_in[2] ) {
    // pause
    if( static_cast< SFBool * >( routes_in[2] )->getValue( tex->id ) ) {
       tex->decoder->pausePlaying();
    }
  } else if( event.ptr == routes_in[3] ) {
    // loop
    tex->decoder->setLooping( static_cast< SFBool * >( routes_in[2] )->getValue( tex->id ) );
  } else if( event.ptr == routes_in[4] ) {
    // url
    MFString *urls = static_cast< MFString * >( routes_in[4] );
    for( MFString::const_iterator i = urls->begin(); i != urls->end(); ++i ) {
      string url = tex->resolveURLAsFile( *i );
      if( tex->decoder->loadClip( url ) ) {
        tex->setURLUsed( *i );
        return;
      }
    }

    Console(4) << "Warning: None of the urls in SimpleMovieTexture with url [";
    for( MFString::const_iterator i = urls->begin(); i != urls->end(); ++i ) {  
      Console(4) << " \"" << *i << "\"";
    }
    Console(4) << "] could be loaded. Either they don't exist or the file format "
             << "is not supported by any H3DVideoClipDecoderNodes that is available "
               << "(in " << getOwner()->getName() << ")" << endl;

    tex->setURLUsed( "" );
  }
}
  