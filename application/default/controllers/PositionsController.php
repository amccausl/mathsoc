<?php

require_once 'MathSocAction.inc';

require_once 'userDB.inc';

class PositionsController extends MathSoc_Controller_Action
{
	private $db;

	public function init($secure = false)
	{	parent::init($secure);

		$this->db = new UserDB();
	}

	/** /positions/ - Display current positions and who holds them
	 *
	 */
	public function indexAction()
	{	// Lookup current positions from database
		$positions = $this->db->getPositionsByCategory();

		// Add positions to the view
		$this->view->assign($positions);
	}

	/** /positions/details/:name - display detailed information for a given position
	 */
	public function detailsAction()
	{	$position = $this->db->getPosition( $this->_getParam('position') );
		$this->view->position = $position;
	}

	public function directorshipsAction()
	{	$positions = $this->db->getAvailablePositions();
		$directorships = $positions['DIR'];
		$this->view->directorships = $directorships;
	}
}

