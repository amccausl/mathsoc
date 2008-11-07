<?php

require_once 'Zend/Controller/Action.php';

// Load the database model for the election system
require_once '../application/default/models/electionDB.inc';

class ElectionController extends Zend_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();

		// User must be authenticated to see any of these pages
		$this->initView();
		$this->view->baseUrl = $this->_request->getBaseUrl();
		$this->view->user = Zend_Auth::getInstance()->getIdentity();

		$this->db = new ElectionDB();

		if( $javascripts = $this->view->javascripts )
		{	array_push( $javascripts, $this->getRequest()->getBaseUrl() . "/js/prototype" );
		}else
		{	$javascripts = array( $this->getRequest()->getBaseUrl() . "/js/prototype" );
		}
		$this->view->javascripts = $javascripts;
	}

    function preDispatch()
	{
		// User must be authenticated to use this system
		$auth = Zend_Auth::getInstance();

		if (!$auth->hasIdentity())
		{	$this->_redirect('auth/login');
		}
	}

	// Browsing Functions
	public function indexAction()
	{	// Grab the authenticated userid
		$user = Zend_Auth::getInstance()->getIdentity();

		// Present the existing elections
		$this->view->elections = $this->db->getElections( $user );
	}

	// Cast a ballot in an election
	public function castAction()
	{
		// Ensure that the desired fields are set
		$electionId = substr( $this->_getParam('electionID'), 8 );

		$user = Zend_Auth::getInstance()->getIdentity();

		// Determine how the user voted
		$vote = array();
		for( $key = 0; isset( $_POST["c$key"] ); $key++ )
		{   array_push( $vote, $_POST["c$key"] );
		}

		// Add the users vote to the system
		$this->view->success = $this->db->vote( $electionId, $user, $vote );
	}

	// Declare an election
	public function declareAction()
	{
		require_once( "../application/default/views/helpers/form.inc" );

		require_once( "../application/default/models/userDB.inc" );
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
	{	// Display all the elections that are public or this user is the CRO of

		require_once '../application/default/models/BC-STV.inc';

		
	}
}
