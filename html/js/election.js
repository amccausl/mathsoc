/**********************************************************
 History:
 
 Adapted from the sortable lists example by Tim Taylor
 http://tool-man.org/examples/sorting.html

 Modified by http://neb.net/playground/dragdrop/

 Further modified by Anton Markov for Mathsoc (June 2007)
 
 **********************************************************/

// Election class
function Election( cID, candidates, hasVoted, isExpired ) {
    // Get the container
    this.electionID = cID;
    var container = document.getElementById( cID );
    if( !container ) return;
    container.className = "election";
    
    if( hasVoted ) {
	this.doneVoting();
	return;
    } else if( isExpired ) {
	this.expired();
	return;
    }
    // Create ballot box
    var ballot = document.createElement( "OL" );
    ballot.className = "sortable ballot";
    ballot.innerHTML = '<h3>Ballot</h3>';

    // Create candidates box
    var candidatesBox = document.createElement( "UL" );
    candidatesBox.className = "sortable candidates";
    candidatesBox.innerHTML = '<h3>Candidates</h3>';
    for( var i = 0; i < candidates.length; ++i ) {
	var candid = document.createElement( "LI" );
	var content = '<table width="100%" cellspacing="0" cellpadding="0" border="0"><tr>' +
	    '<td>' + candidates[i].display + '</td>';
	if( candidates[i].link != null ) {
	    content += 
		'<td class="candidInfo">' + 
		'<a href="' + candidates[i].link + '" title="More information about ' +
		candidates[i].display + '" target="_blank">Info</a></td>';
	}
	content += "</tr></table>";
	candid.innerHTML = content;
	candid.cName = candidates[i].display;
	candid.cID = candidates[i].id;
	candidatesBox.appendChild(candid);
    }

    // create submission box
    var submitter = document.createElement( "DIV" );
    submitter.className = "clear submitter";
    var sButton = document.createElement( "INPUT" );
    sButton.type = "button";
    sButton.value = "Vote!";
    var dragdrop = this;
    sButton.onclick = function() { dragdrop.submitBallot.apply( dragdrop ); }
    submitter.appendChild( sButton );

    // Populate container box
    var centerBox = document.createElement( 'DIV' );
    centerBox.className = "centerInterface";

    centerBox.appendChild( ballot );
    centerBox.appendChild( candidatesBox );
    container.appendChild( centerBox );
    container.appendChild( submitter );
    
    // Make ballot box as large as candidates box
    var width = centerBox.clientWidth / 2 - 10;
    var height = candidatesBox.clientHeight;
    ballot.style.width = width + "px";
    ballot.style.height = height + "px";
    candidatesBox.style.width = width + "px";
    candidatesBox.style.marginLeft = width + 10 + "px";
//	ballot.offsetLeft + ballot.offsetWidth + "px";
    candidatesBox.style.height = height + "px";
    
    
    // Initialize ballots container
    this.ballotContainer = ballot;
    this.makeListContainer( ballot );

    // initialize candidates container
    this.candidatesContainer = candidatesBox;
    this.makeListContainer( candidatesBox );

    // initialize submission mechanism
    this.postRequest = new PostRequest( VOTE_REQUEST_URL, this.checkSuccess.bind( this ) );
}


Election.prototype = {
	ballotContainer : null,
	candidatesContainer : null,

	doneVoting : function() {
	    var container = document.getElementById( this.electionID );
	    for( var node = container.firstChild; 
		 node != null;
		 node = node.nextSibling )
	    {
		if( node.nodeName == "DIV" && node.className != "electionHead" ) {
		    node.style.display = "none";
		}
	    }
	    container.className = container.className + " votedBox";
	    container.innerHTML += 
		'<div class="voted">Thank you for voting!</div>';
	},

	expired : function() {
	    var container = document.getElementById( this.electionID );
	    for( var node = container.firstChild; 
		 node != null;
		 node = node.nextSibling )
	    {
		if( node.nodeName == "DIV" && node.className != "electionHead" ) {
		    node.style.display = "none";
		}
	    }
	    container.className = container.className + " expiredBox";
	    container.innerHTML += 
		'<div class="expired">Election closed.</div>';
	},

	makeListContainer : function(list) {
		// these functions are called when an item is draged over
		// a container or out of a container bounds.  onDragOut
		// is also called when the drag ends with an item having
		// been added to the container
		list.onDragOver = new Function();
		list.onDragOut = new Function();
		
		var items = list.getElementsByTagName( "li" );

		var coordNW = Coordinates.northwestPosition( list );
		coordNW.x = 0;
		var coordSE = Coordinates.southeastPosition( list );
		coordSE.x = 100000;
    	
		for (var i = 0; i < items.length; i++) {
			this.makeItemDragable(items[i]);
			//items[i].constrain( coordNW, coordSE );
		}
	},
	
	submitBallot : function() {
		var list = this.ballotContainer;
		var ballotList = new Array;
		for( child = list.firstChild; child != null; child = child.nextSibling ) {
		    if( child.nodeName == "LI" ) {
			ballotList[ballotList.length] = {name : child.cName, ID : child.cID};
		    }
		}
		var confirmList = new Array();
		for( var i = 0; i < ballotList.length; ++ i ) {
		    confirmList[i] = i + 1 + ". " + ballotList[i].name;
		}
		var confirmMsg;
		if( confirmList.length > 0 ) {
		    confirmMsg = "Are you sure you want to vote for the following candidates in this order: \n\n" + confirmList.join( "\n" )
		} else {
		    confirmMsg = "Are you sure you don't want to vote for any candidates?";
		}
		if( confirm( confirmMsg ) ) {
		    var paramList = new Array();
		    paramList[0] = "electionID=" + this.electionID;
		    for( var i = 0; i < ballotList.length; ++i ) {
			paramList[paramList.length] = 
			    "c" + i + "=" + ballotList[i].ID;
		    }
		    this.postRequest.MakeRequest( paramList.join( "&" ) );
		}
	},

	checkSuccess : function( data ) {
		if( data && data.success ) {
		    this.doneVoting();
		} else {
		    alert( "Failed to submit your vote; please try again!" );
		}
	},

	makeItemDragable : function(item) {
		Drag.makeDraggable(item);
		item.setDragThreshold(5);
		
		item.onDragStart = this.onDragStart;
		item.onDrag = this.onDrag;
		item.onDragEnd = this.onDragEnd;
		item.ondblclick = this.forceSwap;

		item.dragdrop = this;

	},

	onDragStart : function(nwPosition, sePosition, nwOffset, seOffset) {
		// Update coordinates of the ballot container
		this.dragdrop.ballotContainer.northwest = 
		    Coordinates.northwestOffset( this.dragdrop.ballotContainer, true );
		this.dragdrop.ballotContainer.southeast = 
		    Coordinates.southeastOffset( this.dragdrop.ballotContainer, true );

		// item starts out over current parent
		this.parentNode.onDragOver();
	},

	onDrag : function(nwPosition, sePosition, nwOffset, seOffset) {
		// Check if we moved into the ballot box
		var center = new Coordinate( (nwOffset.x + seOffset.x) / 2,
					 (nwOffset.y + seOffset.y) / 2 );
		var ballotBox = this.dragdrop.ballotContainer;
		var candidBox = this.dragdrop.candidatesContainer;
		if (center.inside( ballotBox.northwest, ballotBox.southeast ) ||
			center.inside( ballotBox.northwest, ballotBox.southeast ) ) {
		    if( this.parentNode != ballotBox ) {
			this.parentNode.onDragOut();
			ballotBox.onDragOver();
			ballotBox.appendChild( this );
			this.style["top"] = "-1000px";
			this.style["left"] = "-1000px";
		    }
		    this.dragdrop.fixPositions.apply( this );
		} else {
		    if( this.parentNode != candidBox ) {
			this.parentNode.onDragOut();
			candidBox.onDragOver();
			candidBox.appendChild( this );
			this.style["top"] = "-1000px";
			this.style["left"] = "-1000px";
		    }
		    this.dragdrop.fixPositions.apply( this );
		}
	},
	
	fixPositions : function() {
		// correct position
		
		var parent = this.parentNode;
		var predicate = null;

		if( parent.className.indexOf( "ballot" ) >= 0 ) {
		    predicate = function( first, second ) {
			return first.offsetTop + first.clientHeight / 2 
				    >= second.offsetTop + second.clientHeight / 2 - 2;
		    };
		} else {
		    predicate = function( first, second ) {
			return first.innerHTML.toLowerCase() >= 
			       second.innerHTML.toLowerCase();
		    };
		}
				
		var item = this;
		var next = DragUtils.nextItem(item);
		while (next != null && predicate( this, next )) {
			var item = next;
			var next = DragUtils.nextItem(item);
		}
		if (this != item) {
			DragUtils.swap(this, next);
			return;
		}

		var item = this;
		var previous = DragUtils.previousItem(item);
		while (previous != null && predicate( previous, this )) {
			var item = previous;
			var previous = DragUtils.previousItem(item);
		}
		if (this != item) {
			DragUtils.swap(this, item);
			return;
		}
	},

	onDragEnd : function(nwPosition, sePosition, nwOffset, seOffset) {
		this.parentNode.onDragOut();
		this.style["top"] = "0px";
		this.style["left"] = "0px";
	},

	forceSwap : function() {
	    var targetNode;
	    var ballotBox = this.dragdrop.ballotContainer;
	    var candidBox = this.dragdrop.candidatesContainer;
	    if( this.parentNode == ballotBox ) {
		targetNode = candidBox;
	    } else {
		targetNode = ballotBox;
	    } 
	    this.parentNode.onDragOut();
	    targetNode.onDragOver();
	    targetNode.appendChild( this );
	    this.dragdrop.fixPositions.apply( this );
	}
};

var DragUtils = {
	swap : function(item1, item2) {
		var parent = item1.parentNode;
		parent.removeChild(item1);
		parent.insertBefore(item1, item2);

		item1.style["top"] = "0px";
		item1.style["left"] = "0px";
	},

	nextItem : function(item) {
		var sibling = item.nextSibling;
		while (sibling != null) {
			if (sibling.nodeName == item.nodeName) return sibling;
			sibling = sibling.nextSibling;
		}
		return null;
	},

	previousItem : function(item) {
		var sibling = item.previousSibling;
		while (sibling != null) {
			if (sibling.nodeName == item.nodeName) return sibling;
			sibling = sibling.previousSibling;
		}
		return null;
	}		
};
