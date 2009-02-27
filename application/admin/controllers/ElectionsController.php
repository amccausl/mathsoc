<?php

require_once 'MathSocAction.inc';

// Load the database model for the election system and user management
require_once 'electionDB.inc';
require_once 'userDB.inc' ;

class Admin_ElectionsController extends MathSoc_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();

		$this->db = new ElectionsDB();
	}

	public function indexAction()
	{	// Display all the elections and their status
		// Grab the authenticated userid
		$user = Zend_Auth::getInstance()->getIdentity();
		// Present the existing elections
		$this->view->elections = $this->db->getElections( $user );
	}

	// Declare an election
	public function declareAction()
	{
		require_once( "../application/default/views/helpers/form.inc" );

		$db = new UserDB();

		//$positions is array of alias, name, description from the user management database
		$positions = $db->getElectionPositions();
		$this->view->positions = $positions;
		$position_options = array('new' => 'Create New Position');
		foreach( $positions as $position )
		{	$position_options[$position['alias']] = $position['name'];
		}
		$this->view->position_options = $position_options;

		// Set default nomination and voting days
		if( date('m') == 9 )
		{	// Set 10 day nomination
			$nominations_open = strtotime( "next Monday", mktime( 0, 0, 0, 10, 1 ) ) + 30600;
			$nominations_close = $nominations_open + 979200;
		}else
		{	$nominations_open = mktime( 8, 30 );
			if( $nominations_open < time() )
				$nominations_open += 86400;
			$nominations_close = strtotime( "next Saturday", $nominations_open );
			$nominations_close = strtotime( "next Friday", $nominations_close ) + 59400;
		}

		// Set default form fields
		$default = array(
			'nominations_open' => $nominations_open,
			'nominations_close' => $nominations_close,
			// Leave 7 days to campaign, vote for 2 days
			'polls_open' => $nominations_close + 925200,
			'polls_close' => $nominations_close + 1040400
		);

		//TODO: parse date and time to timestamp and set values for variables in the post

		// Include form validation
		$smarty = $this->view->getEngine();
		if( empty( $_POST ) )
		{	SmartyValidate::connect($smarty, true);
			SmartyValidate::register_validator();
			SmartyValidate::register_validator('prefix_element','course_prefix','notEmpty');
		}else
		{	SmartyValidate::connect($smarty);
			if(SmartyValidate::is_valid($_POST))
			{	//TODO: check the voters file and the timing for the dates
				//TODO: if problem, set error messages

				//TODO: else, disconnect and create election
				SmartyValidate::disconnect();
			}
		}

		$smarty->assign($default);
	}

	// tally the ballots in a given election, display the results
	public function tallyAction()
	{	require_once( "../application/default/models/BC-STV.inc" );

		// Grab the votes for the election
		$election = $this->db->getVotes($this->_getParam('electionId'));
		$election['vote_count'] = count( $election['votes'] );

		// Calculate the outcome of the election
		$results = BC_STV( $election['votes'], $election['position_needed'], 1 );
		$election['details'] = $results['details'];

		// Grab information for the winners
		$winners = array();
		foreach( $results['outcome'] as $winner )
		{	array_push( $winners, UserDB::lookup( $winner ) );
		}
		$election['winners'] = $winners;

		$this->view->election = $election;
		$this->view->winners = $winners;
	}
}
