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
			$query = "SELECT e.electionId as id, e.position, e.description, e.CRO, voted
					  FROM elections e, voters v
					  WHERE v.userId = ?
						AND e.electionId = v.electionId
						AND NOW() BETWEEN e.voting_open AND e.voting_close
					  ORDER BY e.electionId";
			$elections = $this->_db->fetchAll( $query, $userId );
		}else
		{
			$query = "SELECT electionId as id, position, description, CRO, 1 as voted
					  FROM elections
					  WHERE NOW() BETWEEN voting_open AND voting_close";
			$elections = $this->_db->fetchAll( $query );
		}

		$count = 0;
		foreach( $elections as $election )
		{	$candidates = array();

			if( $election['voted'] == 0 )
			{	$query = "SELECT candidateId, link
						  FROM candidates
						  WHERE electionId = ?
						  ORDER BY candidateId";
				$results = $this->_db->fetchAll( $query, $election['id'] );
				foreach( $results as $result )
				{	$user = array();
					if( $user = lookup( $result['candidateId'] ) )
					{
						$user['name'] = $result['candidateId'];
						$user['link'] = $result['link'];
					}else
					{	// USER DOESN'T EXIST, THIS IS A PROBLEM
						$user = array();
					}
					$candidates[$result['candidateId']] = $user;
				}
			}
			$elections[$count]['candidates'] = $candidates;
			$count++;
		}
		return $elections;
	}

//TODO
	function getElection( $electionId, $username="" )
	{	// Get the information about the requested election
		$query = "SELECT electionId as id, position, description, CRO
				  FROM elections
				  WHERE electionId = ?";
		$election = $this->_db->query( $query, $electionId );

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

	function lookup( $userId = "" )
	{	return false;
	}
}
