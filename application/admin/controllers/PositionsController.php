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
	{	// TODO: get applications should limit applications for the specific exec position and terms for the user
		$application = $this->db->getApplications();
	}
}
