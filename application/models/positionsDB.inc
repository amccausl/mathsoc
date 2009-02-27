<?php

// Load database module

/* This is a system to be coupled with a uwdir authentication scheme

*/

class PositionsDB
{
	private $_db;

	function __construct()
	{	$this->_db = Zend_Db::factory(Zend_Registry::getInstance()->get('config')->db);
	}

	function getPosition( $alias )
	{	$query = 'SELECT positions.alias, positions.name, positions.description, positions.desired AS number, positions.email, positions.default_email 
				  FROM positions
				  WHERE positions.unitId = 1
					AND positions.alias = ?';

		$position = $this->_db->fetchRow( $query, $alias );

		if( empty( $position ) )
		{	return $position;
		}

		// This works because we restrict it to unit 1 (mathsoc)
		if( $position['email'] )
		{	$position['email'] = $position['alias'] . '@mathsoc.uwaterloo.ca';
		}else
		{	unset( $position['email'] );
		}

		$query = 'SELECT holders.term,
						GROUP_CONCAT(volunteers.name SEPARATOR \',\') AS name,
						GROUP_CONCAT(holders.email SEPARATOR \',\') AS include_email,
						GROUP_CONCAT(user_emails.email SEPARATOR \',\') AS email,
						terms.current_term
				  FROM holders, user_emails, volunteers, terms
				  WHERE holders.unitId = 1
					AND holders.position = ?
					AND holders.userId = user_emails.userId
					AND holders.userId = volunteers.userId
					AND holders.term > (terms.last_term - 30)
				  GROUP BY holders.term
				  ORDER BY holders.term DESC';
		$results = $this->_db->fetchAll($query, $alias);

		$terms = array();

		foreach( $results as $result )
		{	$current = array();
			$users = array();

			$names = split(',', $result['name']);
			$include_emails = split(',', $result['include_email']);
			$emails = split(',', $result['email']);

			for( $i = 0; $i < count( $names ); $i++ )
			{	if( $include_emails == 1 )
				{	array_push( $current, array("name" => $names[$i], "email" => $emails[$i]) );
				}else
				{	array_push( $current, array("name" => $names[$i]) );
				}
				array_push( $users, $names[$i] );
			}

			$terms[$result['term']] = $users;

			if( $result['term'] == $result['current_term'] )
			{	$position['current'] = $current;
				$position['current_count'] = count( $users );
			}
		}

		if( !isset( $position['number'] ) )
		{	$position['number'] = 1;
		}
		if( !isset( $position['current_count'] ) )
		{	$position['current_count'] = 0;
		}

		$position['holders'] = $terms;

		return $position;
	}

	function getPositions( $userId = "", $unit = "mathsoc" )
	{
		if( $userId != "" )
		{	$query = 'SELECT positions.alias, positions.name, positions.description
					  FROM holders, terms, positions
					  WHERE positions.alias = holders.position
						AND holders.term = terms.current_term
						AND holders.unitId = ?
						AND holders.userid = ?';
			return $this->_db->fetchAll($query, array($unit, $userid));
		}else
		{	$query = "SELECT positions.alias, positions.name, positions.description
					  FROM positions, units
					  WHERE units.id = positions.unitId
						AND units.alias = ?
					  ORDER BY positions.name";
			return $this->_db->fetchAll($query, $unit);
		}
	}

	function getElectionPositions()
	{	$query = 'SELECT positions.alias, positions.name, positions.description
				  FROM positions, units
				  WHERE units.id = positions.unitId
					AND units.alias = \'mathsoc\'
					AND (positions.category = \'EXC\' OR positions.category = \'REP\')
					AND positions.alias != \'\'
				  ORDER BY positions.name';
		return $this->_db->fetchAll($query);
	}

	function getPositionsByCategory()
	{	$query = 'SELECT p.category,
						p.alias,
						p.name,
						p.email,
						(SELECT GROUP_CONCAT(CONCAT_WS(\':\',volunteers.userId,volunteers.name) SEPARATOR \';\')
						 FROM volunteers, holders, terms
						 WHERE terms.current_term = holders.term
							AND holders.userId = volunteers.userId
							AND p.alias = holders.position
						 GROUP BY holders.position) AS holders
				  FROM positions p
				  WHERE p.category != \'LIST\'
				  ORDER BY p.name';
		$result = $this->_db->fetchAll($query);

		$return = array();

		foreach( $result as $row )
		{	if( !isset($return[$row['category']]) )
				$return[$row['category']] = array();

			$holders = array();
			if( $row['holders'] )
			{	$temp = explode(";", $row['holders']);
				foreach( $temp as $entry )
				{	$temp2 = explode(":", $entry);
					$holders[$temp2[0]] = $temp2[1];
				}
			}

			$row['holders'] = $holders;
			array_push( $return[$row['category']], $row );
		}

		return $return;
	}

	public function getAvailablePositions()
	{	$query = 'SELECT positions.category,
						positions.alias,
						positions.name,
	 					positions.desired,
						( SELECT COUNT(holders.userId)
						  FROM holders, terms
						  WHERE holders.unitId = 1
							AND holders.term = terms.current_term
							AND holders.position = positions.alias
						  GROUP BY holders.position
						) AS holders,
						positions.chosen
				  FROM positions
				  GROUP BY positions.alias
				  ORDER BY positions.name';
		$result = $this->_db->fetchAll($query);

		$return = array();
		foreach( $result as $row )
		{	if( !isset($return[$row['category']]) )
				$return[$row['category']] = array();

			$holders = isset( $row['holders'] ) ? $row['holders'] : 0;
			$desired = isset( $row['desired'] ) ? $row['desired'] : 1;

			if( $holders < $desired )
				array_push( $return[$row['category']], $row );
		}

		return $return;
	}

	public function getPositionQuestions( $alias )
	{	$query = 'SELECT q.key, q.type, q.type_params, q.text
				  FROM questions q, positions_questions p
				  WHERE q.id = p.question
					AND p.position = ?';

		$questions = $this->_db->fetchAll( $query, $alias );
		return $questions;
	}

	/** Return the current position application
	 */
	public function getApplications()
	{	$query = 'SELECT p_a.id,
					p_a.term,
					p_a.unit,
					p.name as position,
					p_a.user,
					p_a.applied,
					p_r.ruler_unit,
					p_r.ruler_position
				  FROM positions_applications p_a, positions_rules p_r, positions p
				  WHERE p_a.unit = p.unitId
					AND p_a.position = p.alias
					AND p_a.status IS NULL
					AND p_r.ruled_unit = p.unitId
					AND p_r.ruled_position = p.alias
				  ORDER BY p_a.term, p_a.unit, p.name';
		$results = $this->_db->fetchAll( $query );
		return $results;
	}

	/** Return a given application
	 */
	public function getApplication( $id )
	{	$query = 'SELECT p_a.id, p_a.term, p_a.unit, p.alias, p.name as position, v.name, p_a.questions, p_a.applied
				  FROM positions_applications p_a, positions_rules p_r, positions p, positions_questions p_q, questions q, volunteers v
				  WHERE p_a.unit = p.unitId
					AND p_a.position = p.alias
					AND p_r.ruled_unit = p.unitId
					AND p_r.ruled_position = p.alias
					AND v.userId = p_a.user
					AND p_a.id = ?';
		$result = $this->_db->fetchRow( $query, $id );
		$answers = unserialize( $result['questions'] );
		$result['questions'] = array();

		$query = 'SELECT q.key, q.text
				  FROM questions q, positions_questions p_q
				  WHERE p_q.question = q.id
					AND p_q.unit = ?
					AND p_q.position = ?';
		$questions = $this->_db->fetchPairs( $query, array( $result['unit'], $result['alias'] ) );

		foreach( $questions as $key => $question )
		{	$result['questions'][$question] = $answers[$key];
		}

		return $result;
	}

	/** Add an application
	 */
	public function addApplication( $application )
	{	// TODO: check that the user exists in the database
		return $this->_db->insert('positions_applications', $application);
	}

	public function selectApplication( $id )
	{
		$query = 'SELECT term, unit as unitId, position, user as userId
				  FROM positions_applications
				  WHERE id = ?';
		$row = $this->_db->fetchRow( $query, $id );

		$n = $this->_db->update( 'positions_applications', array('status' => 'selected'), "id = $id" );
		if( $n == 1 )
		{	// add the holder to the holders table
			return $this->_db->insert('holders', $row);
		}
		return false;
	}

	public function rejectApplication( $id )
	{	$n = $this->_db->update( 'positions_applications', array('status' => 'rejected'), "id = $id" );
		if( $n == 1 )
			return true;
		return false;
	}
}
