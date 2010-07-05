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
	{	$logger = Zend_Registry::get('logger');


		if( isset( $userId ) )
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
			$query = "SELECT electionId as id, position, description, CRO, 'polls open' as status
					  FROM elections
					  WHERE NOW() BETWEEN voting_open AND voting_close
						UNION
					  SELECT electionId as id, position, description, CRO, 'polls closed' as status
					  FROM elections
					  WHERE NOW() > voting_close
						UNION
					  SELECT electionId as id, position, description, CRO, 'queued' as status
					  FROM elections
					  WHERE NOW() < voting_open";
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
require_once('userDB.inc');
//					require_once( "../application/default/models/userDB.inc" );
					if( $user = UserDB::lookup( $result['candidateId'] ) )
					{	$user['link'] = $result['link'];
					}else
					{	// USER DOESN'T EXIST, THIS IS A PROBLEM
						$user['name'] = $result['candidateId'];
						$user['link'] = $result['link'];
					}
					$candidates[$result['candidateId']] = $user;
				}
			}
			$elections[$count]['candidates'] = $candidates;
			$count++;
		}
		return $elections;
	}

	/** isEligible - Determine if a user is allowed to vote in a given election */
	function isEligible( $election, $userId="" )
	{	$logger = Zend_Registry::get('logger');

		if( !isset( $userId ) )
			$userId = Zend_Auth::getInstance()->getIdentity();
		if( !isset( $userId ) )
			return false;

		// Determine if the user has already voted in the selected election
		$query = "SELECT voted
				  FROM voters
				  WHERE electionId = ?
					AND userid = ?";

		$result = $this->_db->fetchOne( $query, array( $election, $userId ) );
		if( !isset( $result ) )
		{	// The user isn't an eligible voter for this election
			$logger->log("$userId is ineligible to vote in election $election", Zend_Log::INFO);
			return false;
		}elseif( $result == 1 )
		{	// The user has already voted
			$logger->log("$userId has already voted in election $election", Zend_Log::INFO);
			return false;
		}
		return true;
	}

	/** vote - Records a users vote in a given election.
	 *
	 */
	function vote( $election, $username, $ballot )
	{	// Determine if the user is eligible for voting in this election
		if( !$this->isEligible( $election, $username ) )
		{	return false;
		}

		$logger = Zend_Registry::get('logger');

		// Create changes concurrently to ensure consistentcy
		$this->_db->beginTransaction();
		try {
			if( $this->_db->update( 'elections',
					array( "CRO_ballot" => serialize($ballot) ),
					array( "electionId = '$election'", "CRO = '$username'" )) == 1 )
			{	// User is the CRO of the election
			}else
			{	// Add ballot to the election table
				$query = "INSERT INTO votes (electionId, vote) VALUES (?, ?)";
				$this->_db->query( $query, array( $election, serialize($ballot) ) );

				// Mark the user as having voted
				if( $this->_db->update( 'voters',
						array( "voted" => "1" ),
						array( "userId = '$username'", "electionId = '$election'", "voted = '0'" ) ) == 1 )
				{
					// If all succeed, commit the transaction and all changes
					// are committed at once.
				    $this->_db->commit();
				}else
				{	$this->_db->rollBack();
					$logger->log("database has failed when $userId voting in election $election", Zend_Log::ERROR);
					return false;
				}
			}
		} catch (Exception $e)
		{	// If any of the queries failed and threw an exception,
			// we want to roll back the whole transaction, reversing
			// changes made in the transaction, even those that succeeded.
			// Thus all changes are committed together, or none are.
			$this->_db->rollBack();
			$logger->log($e->getMessage() . " has prevented $userId from voting in election $election", Zend_Log::ERROR);
			return false;
		}
		return true;
	}

	function getVotes( $electionId )
	{	$logger = Zend_Registry::get('logger');

		$query = "SELECT *
			  FROM elections
			  WHERE electionId = ?";
		$election = $this->_db->fetchAll( $query, $electionId );
		$election = $election[0];

		// Add the CRO ballot as the first ballot in the votes array
		$votes = array( unserialize( $election['CRO_ballot'] ) );

		$query = "SELECT vote
			  FROM votes
			  WHERE electionId = ?";

		// Add all the votes to the array
		foreach( $this->_db->fetchCol($query, $electionId) as $vote )
		{	array_push( $votes, unserialize( $vote ) );
		}
		$election['votes'] = $votes;

		// Compare the cardinality of the votes with the voters for that election as a sanity check
		$query = "SELECT
					(SELECT COUNT(userId) FROM voters WHERE electionId = ? AND voted = 1) AS voters,
					(SELECT COUNT(vote) FROM votes WHERE electionId = ?) AS votes";
		$validation = $this->_db->fetchAll( $query, array( $electionId, $electionId ) );
		$validation = $validation[0];

		if( $validation['votes'] != $validation['voters'] )
		{	$logger->log("$userId has already voted in election $electionId", Zend_Log::ERROR);
			return false;
		}

		// Return the votes in the election
		return $election;
	}
}
