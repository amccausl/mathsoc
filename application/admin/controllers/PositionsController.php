<?php

require_once 'MathSocAction.inc';
require_once 'userDB.inc';

class Admin_PositionsController extends MathSoc_Controller_Action
{
	// The database object to retrieve information from
	private $db;

	public function init()
	{	parent::init();

		$this->db = new UserDB();

		// User must be an admin to see any of these pages
		$this->admins = array('mathsoc' => array('exec' => 'current'));
		$this->secure();
	}

	/** /admin/positions
	 * Display current position applications
	 */
	public function indexAction()
	{
	}

	/** /admin/positions/new - Create a new position
	 */
	public function newAction()
	{
	}

	/** /admin/positions/edit - Edit an existing position
	 */
	public function editAction()
	{
	}

	/** /admin/positions/select - Select someone to hold the given position
	 */
	public function applicationsAction()
	{
		if( $this->_getParam('id') )
		{	$application = $this->db->getApplication( $this->_getParam('id') );
			$this->view->application = $application;
		}

		// TODO: get applications should limit applications for the specific exec position and terms for the user
		$applications = $this->db->getApplications();
		$this->view->applications = $applications;
	}
}
