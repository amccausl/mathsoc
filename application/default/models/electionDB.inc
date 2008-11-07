<?php 

// Load database module

/* This is a system to be coupled with a uwdir authentication scheme

*/

class ElectionDB
{
	protected $_db;

	function __construct()
	{	$config = new Zend_Config_Ini('../config/main.ini', 'voting');
		$this->_db = Zend_Db::factory($config->db);
	}

	// Return a list of elections the given user is eligible to vote in
	function getElections( $userId, $unit = 'mathsoc' )
	{	if( isset( $userId ) )
		{
			$query = "SELECT e.electionId as id, e.position, e.description, e.CRO
					  FROM elections e, voters v
					  WHERE v.userId = ?
						AND e.electionId = v.electionId
						AND NOW() BETWEEN e.voting_open AND e.voting_close";
			$elections = $this->_db->fetchAll( $query, $userId );
		}else
		{
			$query = "SELECT electionId as id, position, description, CRO
					  FROM elections
					  WHERE NOW() BETWEEN voting_open AND voting_close";
			$elections = $this->_db->fetchAll( $query );
		}

		return $elections;
	}

//TODO export this to the user model
/** lookup - UWDir a user to determine more information
 * Borrowed from mathsoc.inc (had to copy for php4 compatibility)
 */
function lookup( $userid )
{
	// Define a user to be returned
	$user = array();

	// Run uwdir query and parse the results
	exec( "uwdir userid=" . escapeshellarg( $userid ), $output );
	array_pop( $output );
	array_shift( $output );
	foreach( $output as $line )
	{       $line = explode( ":", $line );
	        $user[trim( $line[0] )] = trim( $line[1] );
	}
	$name = explode( ", ", $user['name'] );
	$user['name'] = "{$name[1]} {$name[0]}";

	// Do an LDAP lookup of the user to determine what position they hold
	$ch = curl_init("http://kiwi.uwaterloo.ca/user/testLookup");
	curl_setopt($ch, CURLOPT_POST, true);
	curl_setopt($ch, CURLOPT_POSTFIELDS, "newLookup={$userid}&commit=Find");
	curl_setopt($ch, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows; U; Windows NT 5.0; en-US; rv:1.8.1.4) Gecko/20070515 Firefox/2.0.0.4 Test/0.1");
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
	$kiwi = curl_exec($ch);
	curl_close($ch);

	$start = strpos($kiwi, "LDAP:") + 6;
	$ldap = substr($kiwi, $start, strpos($kiwi, "</span>", $start) - $start);

	if (strpos($ldap, "CN=UWdir-HR-faculty") !== false
	 && strpos($ldap, "CN=UWdir-active") !== false)
	         $user['position'] = "professor";

	return $user;
}

//TODO
function getNominations( $userId="" )
{	
	if( !isset( $userId ) )
		$userId = Zend_Auth::getInstance()->getIdentity();
	if( !isset( $userId ) )
		return false;

	$query = "SELECT e.electionId
		  FROM elections e, voters v
		  WHERE v.userId = '$userId'
			AND e.electionId = v.electionId
			AND NOW() BETWEEN e.nomination_open AND e.nomination_close";

	// Run the given query
	if( !( $result = @ mysql_query( $query ) ) )
	{	return false;
	}

	$elections = array();

	while( $row = mysql_fetch_array( $result ) )
	{	array_push( $elections, $row[0] );
	}

	return $elections;
}

//TODO
	function getElection( $electionId, $username="" )
	{	// Get the information about the requested election
		$query = "SELECT *
				  FROM elections
				  WHERE electionId = ?";
		$election = $this->_db->query( $query, $electionId );

	$election = array( 'id' => $row['electionId'],
			'position' => $row['position'],
			'description' => $row['description'],
			'CRO' => $row['CRO'] );

	// If a user is requesting an election, only return candidates
	// if the user has not yet voted.
	if( $username )
	{	$query = "SELECT voted
			  FROM voters
			  WHERE electionId = '$electionId'
				AND userId = '{$username}'";
		if( !( $result = @ mysql_query( $query ) ) )
		{	return false;
		}
		$row = mysql_fetch_array( $result );
		if( $row[0] == 1 )
		{	$election['candidates'] = array();
			return $election;
		}
	}

	// Retrieve a list of candidates for that election
	$query = "SELECT candidateId, link
		  FROM candidates
		  WHERE electionId = '{$electionId}'";
	
	// Run the given query
	if( !( $result = @ mysql_query( $query ) ) )
	{	return false;
	}elseif( mysql_num_rows( $result ) == 0 )
	{	return false;
	}

	$candidates = array();

	while( $row = mysql_fetch_array( $result ) )
	{	$user = lookup( $row[0] );
		$candidates[$row[0]] = array('name' => $user['name'], 'link' => $row[1]);
	}

	$election['candidates'] = $candidates;

	return $election;
}

	/** isEligible - Determine if a user is allowed to vote in a given election */
	function isEligible( $election, $userId="" )
	{
		if( !isset( $userId ) )
			$userId = Zend_Auth::getInstance()->getIdentity();
		if( !isset( $userId ) )
			return false;

		$query = "SELECT *
				  FROM voters
				  WHERE electionId = ?
					AND userId = ?";

		$result = $this->_db->fetch( $query, $election, $userId );
	}

	function vote( $election, $username, $ballot )
	{
		// Determine if the user has already voted in the selected election
		$query = "SELECT voted
				  FROM voters
				  WHERE electionId = ?
					AND userid = ?";

		$result = $this->_db->fetchOne( $query, array( $election, $username ) );
		if( !isset( $result ) )
		{	// The user isn't an eligible voter for this election
			return false;
		}elseif( $result == 1 )
		{	// The user has already voted
			print( "already voted" );
			return false;
		}

		// Create changes concurrently to ensure consistentcy
		$this->_db->beginTransaction();
		try {
			// Add ballot to the election table
			$query = "INSERT INTO votes (electionId, vote) VALUES (?, ?)";
			$this->_db->query( $query, array( $election, serialize($ballot) ) );

			// Mark the user as having voted
			$query = "UPDATE voters
					  SET voted = 1
					  WHERE electionId = ?
						AND userId = ?";
			$this->_db->query( $query, array( $election, $username ) );

			// If all succeed, commit the transaction and all changes
			// are committed at once.
		    $this->_db->commit();

		} catch (Exception $e)
		{	// If any of the queries failed and threw an exception,
			// we want to roll back the whole transaction, reversing
			// changes made in the transaction, even those that succeeded.
			// Thus all changes are committed together, or none are.
			$this->_db->rollBack();
			echo $e->getMessage();
			return false;
		}
		return true;
	}

	function getVotes( $election )
	{
		$query = "SELECT CRO_ballot
			  FROM elections
			  WHERE electionId = ?";
		$result = $this->_db->query( $query, $election );
		if( !$row = $result->fetch() )
		{	return false;
		}

		// Add the CRO ballot as the first ballot in the votes array
		$votes = array( unserialize( $row[0] ) );

		$query = "SELECT vote
			  FROM votes
			  WHERE electionId = ?";

		// Add all the votes to the array
		foreach( $this->_db->fetchAll($query, $election) as $vote )
		{	array_push( $votes, unserialize( $vote[0] ) );
		}

		// Compare the cardinality of the votes with the voters for that election as a sanity check
		$query = "SELECT
					(SELECT COUNT(userId) FROM voters WHERE electionId = ? AND voted = 1) AS voters,
					(SELECT COUNT(vote) FROM votes WHERE electionId = ?) AS votes";
		$validation = $this->_db->fetch( $query, array( $election, $election ) );

		if( $validation['votes'] != $validation['voters'] )
			return false;

		// Return the votes in the election
		return $votes;
	}
}
