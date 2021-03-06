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
/// \file SAIFunctions.h
/// \brief Header file for common SAI interface classes and functions
/// for use by script engine implementations such as SpiderMonkeySAI.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SAIFUNCTIONS_H__
#define __SAIFUNCTIONS_H__


#include <H3D/H3DApi.h>
#include <H3D/H3DTypes.h>
#include <H3D/Node.h>
#include <H3D/X3DGroupingNode.h>
#include <H3D/X3D.h>
#include <H3D/MFNode.h>
#include <H3D/MFString.h>

#include <vector>

namespace H3D{ 

  /// Forward declaration
  class Scene;
  namespace SAI {

    /// This section defines the error types that may be generated in response
    /// to service requests. Errors are generated as synchronous values from a
    /// service request and returned as variables of type SAIError. 
      ///

    class H3DAPI_API SAIError: public H3DUtil::Exception::H3DException {
    public:
      /// SAIERROR error types.
      typedef enum {
        /// This error type appear in the errors definition of a service 
        /// request. A language binding shall define the representation for
        /// the SAIError data type and assign values for each of the errors
        /// defined below but may also define additional error data types to
        /// these.
        SAI_BROWSER_UNAVAILABLE,

        /// An error has occurred that resulted in the connection between the
        /// browser and external application becoming non-functional. 
        /// Therefore, the service request could not be executed. This is a
        /// different error condition from SAI_BROWSER_UNAVAILABLE as it
        /// assumes that a valid reference has already been obtained and the
        /// error occurred at a later time.
        SAI_CONNECTION_ERROR,

        /// The request made of the current SAINode, SAIField or
        /// SAIBrowserRef reference is being made to an object that has
        /// already been disposed prior to this service request.
        SAI_DISPOSED,

        /// An operation was attempted that used an imported node when it
        /// is not permitted as defined in 4.4.6 Import/Export semantics
        /// in ISO/IEC 19775-1. For example, adding the imported node as a
        /// child to another node in the current scene
        SAI_IMPORTED_NODE,

        /// The user is attempting to add a node to an execution context
        /// that is greater than the capabilities defined by the profile
        /// and components definition for that scene.
        SAI_INSUFFICIENT_CAPABILITIES,

        /// The attempt to perform an operation of a field failed because
        /// it is an invalid action for that field type. For example, an 
        /// attempt made to read the value of an inputOnly field would 
        /// generate this error.
        SAI_INVALID_ACCESS_TYPE,

        /// The instance of browser data type provided as part of the
        /// parameters to the service request has been disposed of prior to
        /// this request.
        SAI_INVALID_BROWSER,
        
        /// When the user has attempted to import a W3C DOM document into
        /// an X3D scene and the document cannot be completely resolved to an
        ///  X3D scene graph. There are many cases where this error might be 
        /// generated. Examples are - an invalid document structure or not 
        /// having the correct root element.
        SAI_INVALID_DOCUMENT,

        /// The instance of ExecutionContext data type provided as part of the
        /// parameters to this service request has been disposed of prior to 
        /// this request.
        SAI_INVALID_EXECUTION_CONTEXT,

        /// The instance of field data type provided as part of the parameters
        /// to this service request has been disposed of prior to this request.
        SAI_INVALID_FIELD,

        /// The name provided to a service request is invalid or cannot be 
        /// found in the context of that object.
        SAI_INVALID_NAME,

        /// The instance of the node type provided as part of the parameters
        /// to this service request has been disposed of prior to this request.
        SAI_INVALID_NODE,

        /// The user is attempting to make a service request that is performed
        /// outside of the context that such operations are permitted in. 
        /// 4.8.3.7 User code lifecycle. Where a service defines this as being
        /// a possible error type, this shall only be thrown by internal 
        /// interactions. External interactions shall never generate this 
        /// error.
        SAI_INVALID_OPERATION_TIMING,

        /// The instance of SAIURL data type provided as part of the parameters
        /// to this service request is invalid due to a syntax error. Errors due
        /// to the requested URL not being available shall generate either an 
        /// SAI_URL_UNAVAILABLE error or an asynchronous event notifying of 
        /// such a problem.
        SAI_INVALID_URL,

        /// The SAIString or X3D file (for example as a result of the 
        /// fetching of a URL reference) passed to this service request 
        /// contains invalid syntax and cannot be parsed to produce legal 
        /// data types for use in other service requests.
        SAI_INVALID_X3D,

        /// Indication that a named node handling action has attempted to 
        /// re-use a name that is already defined elsewhere in this current 
        /// scene.
        SAI_NODE_IN_USE,
        
        /// An error condition used for IMPORTed nodes. The user has described
        /// a node that the IMPORT statement has said is valid, but the 
        /// underlying Inline has not yet been loaded to verify that it is a 
        /// correctly EXPORTed node.
        SAI_NODE_NOT_AVAILABLE,

        /// A service request was made that assumed the browser was currently
        /// participating in a shared scene graph when it was not.
        SAI_NOT_SHARED,

        /// Generalised error for when a service request is made for a
        /// capability that is not available in this browser implementation.
        /// For example, if the user requests a profile declaration for a
        /// profile that is not supported by the browser, this error may 
        /// be generated.
        SAI_NOT_SUPPORTED,

        /// The service request requiring the browser to have a world URL set
        /// cannot be completed because no URL has been set. This error is
        /// typically generated from a getWorldURL or getNode service request.
        SAI_URL_UNAVAILABLE
      } ErrorType;

      /// Constructor
      /// \param _type The error type, see ErrorType struct.
      /// \param _message optional message associtiated with the exception.
      SAIError( ErrorType _type,
        const string &_message = "" ) : 
        H3DUtil::Exception::H3DException( _message ),
        type(_type) {}

      /// Returns the SAI error type.
      inline ErrorType getErrorType() const {
        return type; 
      }

      /// Returns the error message of the SAIError.
      inline const string & getErrorMessage() const {
        return message; 
      }

    protected:
      ErrorType type;
    };

    struct H3DAPI_API DOMNode {

    };

    
    struct H3DAPI_API ExecutionContext: public H3DUtil::RefCountedClass {
      ExecutionContext( bool _clean_up = false ) :
        clean_up( _clean_up ) {
        instances.push_back( this );
      }

      virtual ~ExecutionContext() {
        instances.remove( this );
      }

      static list< ExecutionContext * > instances;

      static void cleanUp() {
        for( list< ExecutionContext * >::iterator i = instances.begin();
             i != instances.end(); i++ ) {
          if( (*i)->clean_up ) {
            (*i)->root_node.reset( NULL );
            (*i)->named_nodes.clear();
            (*i)->exported_nodes.clear();
            (*i)->protos.clear();
            (*i)->world_url = "";
          }
        }
      }
      
      //const string &getSpecificationVersion();
      //const string &getEncoding();

      // TODO: 
      //      const ProfileInfo &getProfileInfo();
      // const ComponentInfo      
      // ...

      /*      X3DRoute *addRoute( Node *from_node, const string &from_field,
        Node *to_node, const string &to_field ) { return NULL;}
      
      void deleteRoute( X3DRoute * ){}
      */
      Node *createNode( const string &node_name );
      // Returns a new MFNode object with all the root nodes. Ownership of
      // new object is given to the caller.
      MFNode *getRootNodes();
      /*
      Node *createProto( const string &node_name ){return NULL;}

      Node *getImportedNode( const string &node_name ) { return NULL;}
      void updateImportedNode( const string &node_name, Node * ){}
      void removeImportedNode( const string &node_name ){}

      Node *getNamedNode( const string &node_name ) {return NULL; }
      void updateNamedNode( const string &node_name, Node * ) {}
      void removeNamedNode( const string &node_name ) {} */

      AutoRef< X3DGroupingNode > root_node;
      X3D::DEFNodes named_nodes;
      X3D::DEFNodes exported_nodes;
      X3D::PrototypeVector protos;
      string world_url;
      bool clean_up;
    };

    struct H3DAPI_API SAIScene: public ExecutionContext {
      //      void setMetaData( const string &key, const string &value );
      //      string getMetaData( const string &key );
      
      //      const string &getSpecificationVersion();

      //      Node *getExportedNode( const string &node_name );
      //      void updateExportedNode( const string &node_name, Node * );
      //      void removeExportedNode( const string &node_name );
      
    };


    //struct H3DAPI_API X3DField {
      
    //  Field *field;
    //};
    
    /// The ComponentInfo object stores information about a particular
    /// X3D component. The object consists solely of read-only properties.
    /// It does not define any additional functions. It shall be an error 
    /// to write to any of these properties. 
    struct H3DAPI_API ComponentInfo {
      /// The formal name of the component
      string   name;
      /// The level of support of this instance.
      H3DUInt32 level;
      /// A generic, freeform title string provided by the browser manufacturer
      string title;
      /// If provided, The URL to the entity that wrote this component.
      /// Assumes that extensions to the browser may not necessarily
      /// be provided by the browser writer's themselves.
      string providerUrl;
    };

    /// This object stores information about a particular X3D profile.
    struct ProfileInfo {
      /// The formal name as defined in part 1 of ISO/IEC 19775.
      string   name;
      /// The level of support of this instance
      H3DUInt32 level;
      /// A generic, freeform title string provided by the browser 
      /// manufacturer
      string   title;
      /// If provided, the URL to the entity that wrote this component. 
      /// This assumes that extensions to the browser may not necessarily
      /// be provided by the browser writer's themselves.
      string   providerUrl;
      /// An array of the ComponentInfo object instances that make up 
      /// this profile.
      vector< ComponentInfo > components;
    };

    class H3DAPI_API Browser {
    public:
      /// Constructor.
      Browser( Scene *s );

      /// The getName service returns the name of the browser. 
      inline string getName() {
        return name;
      }

      /// The setName function sets the name of the browser. 
      inline void setName( const string &n) {
        name = n;
      }

      /// The getVersion service returns the current version of the 
      /// browser application. The version number of the browser is
      /// implementation dependent.
      inline string getVersion() {
        return version;
      }

      /// The setVersion function sets the version string of the browser. 
      inline void setVersion( const string &n) {
        version = n;
      }

    /// The getCurrentSpeed service returns the navigation speed 
    /// of the current world. The current speed is the average 
    /// navigation speed for the currently bound NavigationInfo 
    /// node of the active layer in metres per second in the coordinate
    /// system of the currently bound viewpoint.
    H3DFloat getCurrentSpeed();
    
    /// The getCurrentFrameRate service returns the current frame 
    /// display rate of the browser. If this is not supported, the
    /// value returned is zero.
    H3DFloat getCurrentFrameRate();
    
    /// The getSupportedProfiles service returns the list of all
    /// profiles that are supported by this browser. All browsers
    /// shall support at least one profile.  It shall be an error
    /// if the browser returns a declaration for a profile that it
    /// does not fully support.
    const std::vector< ProfileInfo > &getSupportedProfiles();

    /// The getProfile service returns the declaration of the named profile.
    /// The value of the string parameter is the name of a profile from which
    /// to fetch the declaration and shall conform exactly to the name 
    /// specified in ISO/IEC 19775-1. It shall be an error if the a name
    /// with wrong case, bad spelling or anything other than an exact match.
    /// The browser is only required to return an SAIProfileDeclaration value
    /// if it supports the named profile. If it does not support the named
    /// profile, SAI_NOT_SUPPORTED shall be generated.
    const ProfileInfo &getProfile( const string &profile );

    /// The getSupportedComponent service returns a list of all components
    /// that are supported by this browser. All browsers shall support at
    /// least one component, as required to support profiles.
    const std::vector< ComponentInfo > &getSupportedComponents();

    /// The getComponent service returns the declaration of the named 
    /// component. The value of the SAIComponent parameter is the name
    /// of a component and level from which to fetch the declaration and
    /// shall conform exactly to the naming conventions used in the file
    /// format. It shall be an error if the user provides a name with wrong
    /// case, bad spelling or anything other than an exact match. The
    /// browser is only required to return a SAIComponentDeclaration 
    /// value if it supports the named component and the requested level.
    /// If it does not support the component at the level desired, 
    /// SAI_NOT_SUPPORTED shall be generated
    const ComponentInfo &getComponent( const string &component,
                                       H3DUInt32 level );

    /// The getExecutionContext service returns the current execution 
    /// context. If used in an internal interaction, this service returns
    /// the execution context in which the containing node exists 
    /// (see 4.4.3 Containing Node). When used in an external interaction, 
    /// this service returns the current top-level scene. 
    ///
    /// The execution context is the base form of a scene, but only provides
    /// access to the local nodes, PROTOs and routes as defined by the X3D 
    /// name scoping rules as defined in 4.4.7 Run-time name scope in 
    /// ISO/IEC 19775-1. Depending on the place in the scene graph, the 
    /// returned type may be an instance of SAIScene allowing the user 
    /// to use the greater capabilities.
    ExecutionContext *getExecutionContext();

    /// The createScene service creates a new empty scene that conforms
    /// to the given profile and component declarations. Although the 
    /// specification does not require either be provided, it shall be
    /// an error to provide neither profile nor component definitions. 
    /// A user shall provide at least one valid profile or component 
    /// identifier to this request.
    ///
    /// A scene created this way shall always have its specification 
    /// version set to "3.0", "3.1", or "3.2" (as appropriate) and the
    /// encoding set to "Scripted".
    SAIScene *createScene( ProfileInfo *pi = NULL,
                        const vector< ComponentInfo > &cis = 
                        std::vector< ComponentInfo>() );

    /// The replaceWorld service replaces the current world with the 
    /// world specified by the SAIScene parameter. If another 
    /// replaceWorld or loadURL (see 6.3.14 loadURL) request is made
    /// during the processing of the current request, the current request
    /// is terminated and the new one started. In this case, no extra 
    /// shutdown event shall be generated. The initialize event shall be 
    /// generated at the point where the world is ready to be run. The scene
    /// is not required to contain any valid content. Setting a value of NULL
    /// shall clear the currently set scene and leave a blank browser with 
    /// no renderable content and no current scene.
    ///
    /// The SAI_Browser_Shutdown event is generated immediately upon 
    /// receiving this service request.
    ///
    /// The SAI_Browser_Initialized event is generated when the new nodes
    /// have been set and all browser specific initialization has taken
    /// place but before the first time driven event cascade has been started
    /// (event cascades may have previously resulted due to the initialization
    /// process through internal scripts).
    void replaceWorld( SAIScene *scene );

    /// The importDocument service is a utility request to import a 
    /// W3C DOM document or document fragment and convert it to an 
    /// X3D scene. The input allows any form of DOM Node as defined 
    /// by 2.[W3CDOM2]. Although all derived types are available, it shall 
    /// only be required that DOCUMENT, DOCUMENT_FRAGMENT, and ELEMENT 
    /// types are required to be supported for the conversion process.  
    /// The method only performs a conversion process and does not display
    /// the resulting scene. The scene may then be used as the argument for
    /// the replaceWorld (see 6.3.12 replaceWorld) service. When the conversion
    /// is made, there is no lasting connection between the DOM and the 
    /// generated scene. Each request shall be a single conversion attempt
    /// (the conversion may not be successful if the DOM does not match
    /// the X3D scene graph structure).
    ///
    /// Support for this method is optional and shall be dependent on
    /// the browser support for the XML encoding (see ISO/IEC 19776-1). 
    /// If the browser implementation supports the XML encoding, it shall
    /// support this service. If the browser does not support the XML
    /// encoding, the implementation may support this service. User code
    /// may check that this service is supported through the checking the
    /// browser properties with the getBrowserProperties service. If this
    /// service is not supported by the browser implementation, 
    /// SAI_NOT_SUPPORTED error shall be generated.
    SAIScene *importDocument( DOMNode *node );

    /// The loadURL service inserts the content identified by the 
    /// URL(s) in the current world under control of the contents
    /// of the SAIPropertyList instance.
    ///
    /// The SAI_Browser_Shutdown event is generated immediately upon 
    /// receiving this service request if the parameter list is such
    /// that the browser is about to be shutdown (EXAMPLE replaces an
    /// HTML Frame where the browser was embedded).
    ///
    /// The SAI_Browser_Initialized event is generated when the new
    /// nodes have been set and all browser specific initialization has
    /// taken place but before the first time driven event cascade has
    /// been started (event cascades may have previously resulted due 
    /// to the initialization process through internal scripts).
    ///
    /// The property list definition shall include at least one property
    /// that defines loading the URL supplied as a new world in the 
    /// supplied SAIBrowserRef. If the property list is empty, the action
    /// is to replace the world of the current browser with the new world
    /// if the successful URL is a X3D file.
    ///
    /// If another replaceWorld (see 6.3.12 replaceWorld) or loadURL 
    /// request is made during the processing of the current request, 
    /// the current request is terminated and the new one started. In
    /// this case, no extra shutdown event shall be generated. 
    /// The SAI_Browser_Initialized event shall be generated at the 
    /// point where the world is ready to be run if replaceWorld was called.
    void loadURL( const string &url,
                  const std::vector< string > &property_list = 
                  std::vector< string >() );

    /// If the browser supports a description title, it shall be 
    /// set to the new description. Typically, this will be the
    /// title in a window title bar. In cases where there may be
    /// multiple browsers on a single window, the result is implementation
    /// dependent.
    void setDescription( const string &description );

    /// The createX3DFromString service creates nodes from a string. The
    /// string shall contain valid X3D syntax; otherwise an error is
    /// generated. If any relative URLs are encountered in this string,
    /// the base is assumed to be the current browser location. The string
    /// is not required to contain the X3D file header. If it is present,
    /// it shall be treated as an indicator to the version of X3D contained.
    /// If absent, the default version assumed shall be that specified in 
    /// 7.2.4.2 Header statement in ISO/IEC 19775-1. A browser is not 
    /// required to support any versions prior to ISO/IEC 19775.
    ///
    /// If the string contains legal X3D statements but does not contain 
    /// any node instances,  a valid SAIScene value shall still be returned
    /// containing no root nodes, but with the appropriate declaration
    /// identifiers. For example the string may contain EXTERNPROTO 
    /// declarations but no instances of any node. If the SAIString 
    /// provides the content in an encoding format that the browser 
    /// implementation does not support, the browser shall generate an 
    /// SAI_NOT_SUPPORTED error.
    SAIScene *createX3DFromString( const string &s );

    /// The createX3DFromStream service creates nodes from an arbitrary, 
    /// user-provided stream of input data. The stream shall contain
    /// valid X3D syntax from the first character; otherwise, an error
    /// is generated. If any relative URLs are encountered in this string,
    /// the base is assumed to be the current browser location. The stream
    /// is required to include the X3D File Header in accordance with the
    /// encoding requirements for the format.
    ///
    /// If the string contains legal X3D statements but does not contain
    /// any node instances, a valid SAIScene value shall still be returned
    /// containing no root nodes, but with the appropriate declaration
    /// identifiers. For example, the string may contain EXTERNPROTO
    /// declarations but no instances of any node. If the stream identified
    /// by SAIStream provides the content in an encoding format that the
    /// browser implementation does not support, the browser shall generate
    /// an SAI_NOT_SUPPORTED error.
    SAIScene *createX3DFromStream( const istream &s );

    /// The createX3DFromURL service creates nodes from the contents 
    /// of the file represented by the URL. The URL may be a relative
    /// URL which is considered to be using the browser location as the
    /// base document. The scene described by that URL shall be identified
    /// by the returned SAIScene value.
    SAIScene *createX3DFromURL( MFString *urls );

    // TODO:
    //void updateControl( 
    //    registerBrowserInterest


    /// The getRenderingProperties service is used to query for the 
    /// rendering capabilities of the browser. This gives a list of the
    /// low-level hardware capabilities of the browser rather than what 
    /// X3D components are supported. For example, it will give the user
    /// an idea of how many multitexture units can be handled and allows
    /// the end user to customize the number of nodes to use in the 
    /// MultiTexture node. The keys and values in the property list are
    /// implementation dependent and are for informative purposes only. 
    ///Table 6.5 lists the properties that are returned by this service.

    /// Shading  String The type of shading algorithm in use. Typical values are Flat, Gouraud, Phong, Wireframe. 
    /// MaxTextureSize String The maximum texture size supported. The format shall be WIDTHxHEIGHT describing the number of pixels in each direction (for example 1024x1024).
    /// TextureUnits Integer The number of texture units supported for doing multitexture.
    /// AntiAliased Boolean True or false if the rendering is currently anti-aliased or not
    /// ColorDepth Integer The number of bits of colour depth supported by the screen. Allows for optimized selection of textures, particularly for lower colour depth screen capabilities. 
    /// TextureMemory  Float The amount of memory in megabytes available for textures to be placed on the video card.
    const vector< string > &getRenderingProperties();

    /// The getBrowserProperties service is used to query for the capabilities
    /// of the browser reference itself. This gives a list of the expanded
    /// interfaces that this browser reference is capable of supporting. For
    /// example it may be used to query for the existence of browser 
    /// implementation-specific extensions to the defined browser class or
    /// future extensions as provided by this specification.
    ///
    /// Table 6.6 defines some standard property names that are reserved by
    /// part of ISO/IEC 19775. Where a browser implementer chooses to add
    /// additional capabilities, the naming convention of these additional 
    /// properties shall follow the guidelines defined in 4.1.3 Conventions
    /// used in part 1 of of ISO/IEC 19775 (see 2.[I19775-1]).
    ///
    /// Table 6.6 — Standard properties describing extension 
    ///capabilitiesProperty Name Value data type
    /// ABSTRACT_NODES Boolean The browser implementation supports the ability to describe each node type with interfaces that correspond to the abstract node types as defined in ISO/IEC 19775-1 in addition to the basic requirement to support the X3DNode abstract type. This indicates that the browser supports at least Conformance Level 2.
    /// CONCRETE_NODES Boolean The browser implementation supports the ability to describe each node type with interfaces that correspond to the concrete node types as defined in ISO/IEC 19775-1 in addition to the requirement to support all of the abstract types. This indicates that the browser supports at least Conformance Level 3.
    /// EXTERNAL_INTERACTIONS Boolean This SAIBrowserRef supports the additional services required by external interfaces. A SAIBrowserRef provided to user code in internal interactions shall not set this property.
    /// PROTOTYPE_CREATE Boolean The browser implementation supports the ability to dynamically create PROTO and EXTERNPROTO representations through service requests. The basic service capability only allows the ability to create instances of predefined PROTO structures read from a file format.
    /// DOM_IMPORT Boolean The browser implementation supports the importDocument service request.
    /// XML_ENCODING Boolean The browser supports XML as a file format encoding.
    /// CLASSIC_VRML_ENCODING Boolean The browser supports the Classic VRML encoding.
    /// BINARY_ENCODING Boolean The browser supports the binary file format encoding.
    const vector< string > &getBrowserProperties();

    /// The changeViewpoint service changes the currently bound 
    /// X3DViewpointNode instance on the specified Layer to the 
    /// instance defined by the action. Valid action types are 
    /// previous, next, first and last. If a layer ID is not specified,
    /// the current activeLayer is used. When the viewpoint is changed
    /// using this service request, the browser shall first unbind the 
    /// current instance and then bind the new instance. That is, the
    /// number of items on the bindable stack bindable nodes shall not
    /// increase as a result of making this service request. Table 6.7 
    /// defines the actions specified in this part of ISO/IEC 19775.
    ///
    /// This service request implies that there is a standard, 
    /// well-known ordering of the X3DViewpointNode instances so 
    /// that consistent visual behaviour shall be observed. The 
    /// order shall be based on the following rules: 
    /// - The order is declared in the originally parsed file or
    /// stream, including resolution of PROTO instances, but not 
    /// including EXTERNPROTO or X3DInlineNode instances.
    /// - Dynamically created node instances are always appended.
    /// - Instances located in X3DInlineNode instances and 
    /// EXTERNPROTO instances shall be in the order in which the external
    /// scene is resolved, and appended to the list. The inclusion of these
    /// external instances is also dependent on the browser property 
    /// ListInlineViewpoints.
    /// 
    /// An invalid SAILayerID shall result in the operation being ignored. 
    /// Requests for SAILayerID values less than zero or greater one less
    /// than the number of defined layers are considered invalid shall
    /// cause error SAI_INVALID_OPERATION_TIMING to be issued.
    ///
    /// If the world only contains the default X3DViewpointNode instance, 
    /// this request has no effect in the visual output.
    void nextViewpoint( H3DInt32 layer = 0 );
    void previousViewpoint( H3DInt32 layer = 0 );
    void firstViewpoint( H3DInt32 layer = 0 );
    void lastViewpoint( H3DInt32 layer = 0 );

    /// The print service prints a message to the browser's console. 
    /// The language-specific bindings may provide overloaded variations 
    /// on this service that do not take an SAIString value, but take other
    /// data types. Other variants may include the ability to automatically
    /// add linefeed/newline characters without the need to explicitly declare
    /// them in the SAIString value. A binding shall provide at least the base
    /// SAIString variant and a variant that appends linefeed/newline 
    /// characters.
    ///
    /// User code may call this service at any time, without restriction, 
    ///unless the browser reference has been disposed of.
    void print( const string &s );

    /// The dispose service indicates that the client is about to exit this
    /// session and the browser is free to dispose of any resources that this
    /// client may have consumed. An SAI_Browser_Shutdown event is sent only
    /// to this client and may be generated internally by the language 
    /// implementation on the client machine (that is, it is not required
    /// that the browser itself generate this event, just that the event is
    /// generated). If any events have been held because BeginUpdate has been
    /// called, disposing of the browser shall also call EndUpdate to release
    /// those events to the browser.
    void dispose();

    /// TODO?:
    /// The setBrowserOption service allows setting options defined in 9.2.5 
    /// Browser options in ISO/IEC 19775-1. The name field shall be one of 
    /// the defined names in Table 9.2 in ISO/IEC 19775-1. This service shall
    /// return an SAIBoolean value indicating whether the change request 
    /// was successful. A browser is not required to support dynamic changes
    /// to any options. If a browser option is not supported, a value 
    /// of FALSE shall be returned.
    /// bool setBrowserOption( const string &

    protected:
    string name, version;
    AutoRef< SAIScene > SAI_scene; 
    Scene *scene;
    };
  }
}

#endif
