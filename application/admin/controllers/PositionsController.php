<?php

require_once 'MathSocAction.inc';
require_once 'positionsDB.inc';

class Admin_PositionsController extends MathSoc_Controller_Action
{
	// The database object to retrieve information from
	private $db;

	public function init()
	{	parent::init();

		$this->db = new PositionsDB();
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
		// TODO: get applications should limit applications for the specific exec position and terms for the user
		$applications = $this->db->getApplications();
		$this->view->applications = $applications;

		foreach( $applications as $application )
		{	if( isset( $_POST["view_{$application['id']}"] ) )
			{	$application = $this->db->getApplication( $application['id'] );
				$this->view->application = $application;
			}elseif( isset( $_POST["select_{$application['id']}"] ) )
			{	$this->db->selectApplication( $application['id'] );
			}elseif( isset( $_POST["reject_{$application['id']}"] ) )
			{	$this->db->rejectApplication( $application['id'] );
			}
		}
	}
}
