// Constants

var COURSE_REQUEST_URL = "exams.php";

// Helper Functions

/**
 * Returns the XMLHTTPRequest object in a browser-independent manner
 */
function getHTTPRequestObj()
{
    var ret = null;
    if(window.XMLHttpRequest && !(window.ActiveXObject)) {
        try { ret = new XMLHttpRequest(); }
        catch(e) { ret = null;	}
        }
    if(ret == null) {
        try { ret = new ActiveXObject("Msxml2.XMLHTTP"); }
	catch(e) {
	   try { ret = new ActiveXObject("Microsoft.XMLHTTP"); }
	   catch(e) { ret = null;	}
	}
    }
    return ret;
}

/**
 * Returns the JSON object returned from an HTTP request
 */
function getJSONData( HTTPRequest )
{
    var ret = null;
    try {
		if( HTTPRequest != null ) {
			ret = eval( '(' +  HTTPRequest.responseText + ')' );
		}
    } catch(e) { }
    return ret;
}

/**
 * Replaces all of a select box with the strings in data
 */
function fillSelectBox( box, data, first )
{
	box.options.length = 0;
	if( first != null ) {
		box.options.add(new Option( first, 0), 0);
	}
	var i;
    for( i=0; i < data.length; ++i ) {
		box.options.add(new Option(data[i], data[i]), box.options.length);
    }
}


/**
 * Converts an array of the form [ [key1, value1], [key2, value2], ... ]
 * into "key1=value1&key2=value2&..."
 */
function PS( a )
{
	var ret = "";
	for( var i = 0; i < a.length; ++i ) {
		if(ret != "") { ret += "&"; }
		ret += escape(a[i][0]) + "=" + escape(a[i][1]);
	}
	return ret;
}



// Objects

/**
 * A SerialRequest object supports repeated requests to a single URL
 */
var SerialRequest = Class.create();
SerialRequest.prototype = {
	initialize : function( URL, callback )
	{
		this._URL = URL;
		this._callback = callback;
		this._HTTPRequest = null;
	},

	MakeRequest : function( params, callback )
	{

		// Try setting the callback
		if( typeof callback != "undefined" ) {
			this._callback = callback;
		}

		// Ignore any previous requests
		try{
			this._HTTPRequest.onreadystatechange = null;
		} catch(e) {}

		this._HTTPRequest = getHTTPRequestObj();

		if( this._HTTPRequest != null) {
			this._HTTPRequest.open("GET", this._URL + "?" + params, true);
			this._HTTPRequest.onreadystatechange = this.onReadyStateChange.bind(this);
			this._HTTPRequest.send(null);
		}
	},

	onReadyStateChange : function()
	{
		if( this._HTTPRequest.readyState == 4
	     && this._HTTPRequest.status == 200 )
	    {
			this._callback( getJSONData( this._HTTPRequest ) );
		}
	}
};


/**
 * A CourseSelector object wraps all the functionality necessary to create
 * a dynamic course selection form.
 */
var CourseSelector = Class.create();
CourseSelector.prototype = {
		// Constructor
		initialize : function( formID ) {
		// Get the main form object
		this._mainForm = document.getElementById( formID );

		// Callback functions
		this.onChange = null;

		// Initialize private data
		// NOTE: By using one request object, we ensure that the state of widgets
		//       is consistent with what the user does.
		this._request = new SerialRequest( COURSE_REQUEST_URL );
	},

	// PRIVATE Methods

	UpdatePrefix : function()
	{
		fillSelectBox( this._mainForm.prefix, [],  "Loading..." );
		this._request.MakeRequest( "", this.ProcessPrefix.bind(this) );
	},

	ProcessPrefix : function( prefixList )
	{
		if( prefixList != null ) {
			fillSelectBox( this._mainForm.prefix, prefixList );
		}
	},

	UpdateCourses : function()
	{
		// Clear the descriptions box
		this.descBox.innerHTML = "";

		// Get the selected course prefix
		var prefix = this._mainForm.prefix.options[ this._mainForm.prefix.selectedIndex ].value;
		if( prefix != 0 ) {
			fillSelectBox( this._mainForm.course, [], "Loading..." );
			this._request.MakeRequest( PS([["prefix", prefix]]), this.ProcessCourses.bind(this) );
		} else {
			fillSelectBox( this._mainForm.course, []);
			this.BroadcastChange();
		}
	},

	ProcessCourses : function( courseList )
	{
		if( courseList != null ) {
			fillSelectBox( this._mainForm.course, courseList );
			this.BroadcastChange();
		}
	},

	UpdateTitle : function()
	{
		// Clear title
		if( !this.descBox ) return;

		// Get the selected course prefix
		var prefix = this._mainForm.prefix.options[ this._mainForm.prefix.selectedIndex ].value;
		var course = this._mainForm.course.options[ this._mainForm.course.selectedIndex ].value;

		if( prefix != 0 && course != 0 ) {
			// Make HTTP request
			this.descBox.innerHTML = "<i>Loading...</i>";
			this._request.MakeRequest( PS([["prefix", prefix],["number", course]]), this.ProcessTitle.bind(this) );
		} else {
			this.descBox.innerHTML = "";
			this.BroadcastChange();
		}
	},

	ProcessTitle : function( title )
	{
		if( title != null ) {
			this.descBox.innerHTML = title;
			this.BroadcastChange();
		}
	},

	BroadcastChange : function()
	{
		var prefix = this._mainForm.prefix.options[ this._mainForm.prefix.selectedIndex ].value;
		var course = this._mainForm.course.options[ this._mainForm.course.selectedIndex ].value;
		this.onChange( prefix, course );
	},

	// PUBLIC methods (Create HTML elements)

	makePrefixSelection : function()
	{
		document.write( '<select name="prefix" size="10">' );
		document.write( '</select>' );

		this._mainForm.prefix.onchange = this.UpdateCourses.bind( this );
		this._mainForm.prefix.onkeyup = this.UpdateCourses.bind( this );
		this.UpdatePrefix();
	},
	makeCourseSelection : function()
	{
		document.write( '<select name="course" size="10">' );
		document.write( '</select>' );

		this._mainForm.course.onchange = this.UpdateTitle.bind( this );
		this._mainForm.course.onkeyup = this.UpdateTitle.bind( this );
	},
	makeTitleLine : function()
	{
		document.write( '<span id="' + this._mainForm.name + '_desc"></span>' );
		this.descBox = document.getElementById( this._mainForm.name + '_desc' );
	}
};

