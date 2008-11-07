// Constants

var VOTE_REQUEST_URL = "cast";

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
 * A PostRequest object allows multiple requests to a given URL using the
 * post method. It queues up requests and executes them in order.
 */
var PostRequest = Class.create();
PostRequest.prototype = {
	initialize : function( URL, callback )
	{
	    this._URL = URL;
	    this._callback = callback;
	    this._HTTPRequest = null;
	    this._request_queue = new Array();
	},

	MakeRequest : function( params )
	{
	    // Ignore any previous requests
	    if( this._HTTPRequest != null ) {
		this._request_queue.push( params );
	    } else {
		this._doRequest( params );
	    }
	},

	onReadyStateChange : function()
	{  
	    if( this._HTTPRequest.readyState == 4
	     && this._HTTPRequest.status == 200 )
	    {
		this._callback( getJSONData( this._HTTPRequest ) );
		this._HTTPRequest = null;
		if( this._request_queue.length > 0 ) {
		    this._doRequest( this._request_queue.shift() );
		}
	    }
	},

	_doRequest : function( params ) {
	    this._HTTPRequest = getHTTPRequestObj();

	    if( this._HTTPRequest != null) {
		var http = this._HTTPRequest;
		http.open("POST", this._URL, true);
		http.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
		http.setRequestHeader("Content-length", params.length);
		http.setRequestHeader("Connection", "close");
		
		http.onreadystatechange = this.onReadyStateChange.bind(this);
		http.send( params );
	    }
	}
};
