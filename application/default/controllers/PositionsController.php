<?php

require_once 'MathSocAction.inc';

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

		// Access to email, clubs, sections for positions held
		$this->view->positions = array( array( 'name' => 'hello', 'holders' => array('steve', 'bill')));
	}
}

