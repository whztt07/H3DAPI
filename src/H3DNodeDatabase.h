
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
/// \file H3DNodeDatabase.h
/// \brief Header file for H3DNodeDatabase
///
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __H3DNODEDATABSE_H__
#define __H3DNODEDATABSE_H__

#include "H3DApi.h"
#include "Field.h"
#include <iostream>
#include <string>
#include <map>

using namespace std;


/// Useful macro for cleaner field definitions.
#define FIELDDB_ELEMENT( base, field, access ) \
FieldDBInsert field( access( &base::database, #field, &base::field ) );

/// Additional macro to support VRML names that are different from C++
#define FIELDDB_ELEMENT_EX( base, field, access, fieldst )           \
FieldDBInsert field( access( &base::database, fieldst, &base::field ) );

namespace H3D {
    
  class Node;
  typedef Node*( *H3DCreateNodeFunc)(); 
  
  struct H3DNodeDatabase;
  
  /// A FieldDBElement is an encapsulation of a field in a H3DNodeDatabase.
  struct H3DAPI_API FieldDBElement {
    /// Constructor.
    /// \param _container The H3DNodeDatabase the FieldDBElement will be
    /// part of.
    /// \param _name The name of the field.
    /// \param _access The access type of the field.
    FieldDBElement( H3DNodeDatabase *_container,
                    const string & _name, 
                    const Field::AccessType _access );
    
    /// Destructor.
    virtual ~FieldDBElement() {};
    
    /// Get the H3DNodeDatabase this instance is part of.
    H3DNodeDatabase* getContainer() {
      return container;
    }

    /// Get the name of the field.
    const string& getName() {
      return name;
    }

    /// Get the access type of the field.
    const Field::AccessType getAccessType() {
      return access;
    }

    /// Given a Node get a pointer to the field in that node that
    /// is represented by this FieldDBElement.
    virtual Field *getField( Node *n ) const { return NULL; };
    
  protected:
    H3DNodeDatabase *container;
    string name;   
    const Field::AccessType access;
  };
  
  /// The DynamicFieldDBElement is a FieldDBElement for fields that
  /// are not actual members of a Node class, but instead are defined
  /// and added at runtime, e.g. in Script node and X3DProgrammableObject
  /// nodes. 
  struct H3DAPI_API DynamicFieldDBElement : FieldDBElement {
    DynamicFieldDBElement( H3DNodeDatabase *_container,
                           const string&_name, 
                           const Field::AccessType _access,
                           Field *_ptr ) :
      FieldDBElement( _container, _name, _access ), ptr( _ptr ) {}      
    
    /// Given a Node get a pointer to the field in that node that
    /// is represented by this FieldDBElement.
    virtual Field *getField( Node *n ) const {
      if( ptr && n == ptr->getOwner() )
        return ptr;
      else
        return NULL;
    }
  protected:
    /// The pointer to the field.
    Field *ptr;
  };


  /// Template class for specifying specialized FieldDBElement instances. 
  template< class N, class F >
  struct FieldDBTemplate : FieldDBElement {
    FieldDBTemplate( H3DNodeDatabase *_container,
                     const string& _name, 
                     const Field::AccessType _access,
                     auto_ptr<F> N:: *_ptr ) :
      FieldDBElement( _container, _name, _access ), ptr( _ptr ) {}      
    
    /// Given a Node get a pointer to the field in that node that
    /// is represented by this FieldDBElement.
    virtual Field *getField( Node *n ) const {
      N *node = dynamic_cast<N*>(n);
      if ( node ) 
        return (node->*ptr).get();
      else
        return NULL;
    }
  protected:
    /// The member pointer in the node for the field.
    const auto_ptr<F> N:: *ptr;
  };
  
  /// Specialization to handle case where access type is INITIALIZE_ONLY.
  template< class N, class F >
  inline FieldDBTemplate< N, F >* INITIALIZE_ONLY( H3DNodeDatabase *_container,
                                                   const string& _name, 
                                                   auto_ptr<F> N:: *_ptr ) {
    return new FieldDBTemplate< N, F >( _container, 
                                        _name, 
                                        Field::INITIALIZE_ONLY, 
                                        _ptr );
  }  

  /// Specialization to handle case where access type is OUTPUT_ONLY.
  template< class N, class F >
  inline FieldDBTemplate< N, F >* OUTPUT_ONLY( H3DNodeDatabase *_container,
                                               const string &_name, 
                                               auto_ptr<F> N:: *_ptr ) {
    return new FieldDBTemplate< N, F >( _container, 
                                        _name, 
                                        Field::OUTPUT_ONLY, _ptr );
  }  

  /// Specialization to handle case where access type is INPUT_ONLY.
  template< class N, class F >
  inline FieldDBTemplate< N, F >* INPUT_ONLY( H3DNodeDatabase *_container,
                                              const string &_name, 
                                              auto_ptr<F> N:: *_ptr ) {
    return new FieldDBTemplate< N, F >( _container, 
                                        _name, 
                                        Field::INPUT_ONLY, 
                                        _ptr );
  }  

  /// Specialization to handle case where access type is INPUT_OUTPUT.
  template< class N, class F >
  inline FieldDBTemplate< N, F >* INPUT_OUTPUT( H3DNodeDatabase *_container,
                                                const string &_name, 
                                                auto_ptr<F> N:: *_ptr ) {
    return new FieldDBTemplate< N, F >( _container, 
                                        _name, 
                                        Field::INPUT_OUTPUT, 
                                        _ptr );
  }  
  
  
  /// The H3DNodeDatabase contains a mapping between a name of a Node
  /// and the constructor for the Node with that name. Each node that is 
  /// to be used via X3D file input must register itself in the 
  /// database in order to be available to X3D. To register a static 
  /// H3DNodeDatabase member variable must be defined in the node class
  /// with the (nodename, constructor) pair that is wanted.
	struct H3DAPI_API H3DNodeDatabase {
    typedef map< string, FieldDBElement* > FieldDBType;
    typedef map< string, H3DNodeDatabase*> H3DNodeDatabaseType;
    typedef H3DNodeDatabaseType::const_iterator NodeDatabaseConstIterator;

    /// The FieldDBConstIterator is an iterator class used for iterating
    /// over the field instances in an H3DNodeDatabase. Dereferensing the 
    /// iterator will give the name of the field as a string.
    class H3DAPI_API FieldDBConstIterator {
    public:
      /// Constructor.
      FieldDBConstIterator( H3DNodeDatabase * _ndb , bool is_end );

      /// Copy constructor.
      FieldDBConstIterator( const FieldDBConstIterator &f ):
        status( f.status ),
        local_iterator( f.local_iterator ),
        inherited_iterator( new FieldDBConstIterator ),
        ndb( f.ndb ) {
        *inherited_iterator = *f.inherited_iterator;
      }

      /// Pre-increment operator.
      inline FieldDBConstIterator &operator++(int) {
        return operator++();
      } 
      
      /// Post-increment operator.
      FieldDBConstIterator &operator++();

      /// Equals operator.
      bool operator==( FieldDBConstIterator iter );

      /// Not equals operator.
      inline bool operator!=( FieldDBConstIterator iter ) {
        return !operator==( iter );
      }
      
      /// Dereference operator.
      inline string operator*() { 
        if( status == INHERITED )
          return *(*inherited_iterator);
        else 
          /// status == LOCAL )
          return (*local_iterator).first;
      }
      
    protected:
      FieldDBConstIterator(){}

      typedef enum {
        /// The iterator is currently iterating through the local field 
        /// definitions. 
        LOCAL,
        /// The iterator is currently iterating through the field definitions 
        /// of the inherited database fields.
        INHERITED,
        /// The iterator is at end.
        END
      } Status;
      
      /// The status of the iterator. Defines what the iterator is currently
      /// pointing at.
      Status status;
      /// Iterator used for iterating through the local fields in 
      /// the H3DNodeDatabase.Only valid if status == LOCAL.
      FieldDBType::const_iterator local_iterator;
      /// Iterator used for iterating through the fields inherited from
      /// the parent database if it exists. Only valid if status == INHERITED.
      FieldDBConstIterator* inherited_iterator;
      /// The database which fields this iterator iterates through.
      H3DNodeDatabase * ndb;
    };


    /// Constructor.
	  H3DNodeDatabase( const string&_name, 
                     H3DCreateNodeFunc _createf,
                     const type_info &_ti,
                     H3DNodeDatabase *_parent = 0 );

    /// Unnamed constructor, for nodes that cannot be instantiated but
    /// instead act as base classes for other nodes.
	  H3DNodeDatabase( const type_info &_ti,
                     H3DNodeDatabase *_parent = 0 );
    
    /// Search the node database for an entry with a matching type_info structure
    static H3DNodeDatabase *lookupTypeId( const type_info &t );

    /// Search the node database for an entry with a matching name.
    static H3DNodeDatabase *lookupName( const string &name );
    
    /// Create a new instance of a Node type.
    /// \param name The name of the node in the database.
    /// \returns A new instance of the node if it exists in the database,
    /// otherwise NULL is returned.
	  static Node *createNode( const string &name );

    /// add a field to the node database entry
    void addField( FieldDBElement *f );
    
    /// getField() will search the node's field database for a field matching
    /// the given name and returning a pointer to the field if found.
    Field *getField( Node *n, const string& f ) const;
    
	  /// initialise the given Node using the contents of the database - 
    /// initialise field names, field owner pointers and access restrictors.
    void initFields( Node* ) const;
    
	private:
    /// Help function for getField.
    Field *getFieldHelp( Node *n, const string& f ) const;

    /// The string name for this node, used by the X3D parser
    string name;

    /// Create node function
    const H3DCreateNodeFunc createf;
    
    /// The C++ type info structure for this node
    const type_info &ti;
    
    /// Link to the database entry for the parent class, if one exists
    H3DNodeDatabase *parent;
    
    /// Local map of fields that were defined in this node
    FieldDBType fields;
    
    /// Static map of nodes that have initialised themselves into the database
		static bool initialized;
    
  public:
    /// The database with all H3DNodeDatabase instances.
    static H3DNodeDatabaseType *database;

    /// Iterator to the first element in the database. Can be used to iterate 
    /// over all H3DNodeDatabase instances that have been created.
    ///
  	static NodeDatabaseConstIterator begin() {
      return database->begin();
		}
		
    /// Returns an iterator pointing at the end of the H3DNodeDatabase created.
		static NodeDatabaseConstIterator end() {
		  return database->end();
		}
 
    /// Gets an iterator to the beginning first field in the H3DNodeDatabase.
    /// Can be used to iteratate through all the fields in the database.
    FieldDBConstIterator fieldDBBegin() {
      return FieldDBConstIterator( this, 0 );
		}
		
    /// Returns an iterator pointing at the end of fields in the H3DNodeDatabase.
    FieldDBConstIterator fieldDBEnd()  {
		  return FieldDBConstIterator( this,1 );
		}
    
    /// Returns the number of fields in the H3DNodeDatabase.
    inline size_t fieldDBSize() {
      if( !parent ) {
        return fields.size();
      } else {
        return fields.size() + parent->fieldDBSize();
      }
		}
    
	};
	  
  struct FieldDBInsert {
    FieldDBInsert( FieldDBElement *f ) {
      f->getContainer()->addField( f );
    }
  };
  

}

#endif
