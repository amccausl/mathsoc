<?php

require_once 'MathSocAction.inc';

require_once 'userDB.inc';

class PositionsController extends MathSoc_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();

		$this->db = new UserDB();
	}

	/** /positions/ - Display current positions and who holds them
	 *
	 */
	public function indexAction()
	{	// Display user information

		// TODO: include switch statement for name = directors, councillors, exec, available, filled, or all
		// TODO: grab current positions holders from database, add to view.
		$this->view->positions = array( array( 'name' => 'hello', 'holders' => array('steve', 'bill')));
	}

	/** /positions/details/:name - display detailed information for a given position
	 */
	public function detailsAction()
	{	// TODO: include position description, vacancies, how to apply, past holders?, 
		$this->view->position = $this->db->getPosition( 'prez' );
	}
}

