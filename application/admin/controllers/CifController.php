<?php

require_once 'MathSocAction.inc';

// Load the database model for CIF information storage
require_once 'cifDB.inc';

class Admin_CifController extends MathSoc_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();

		$this->secure();

		$this->db = new CifDB();
	}

	/** /admin/cif/ - Display current status of system and list proposals
	 *
	 */
	public function indexAction()
	{	// TODO: database lookup for current proposals
	}
}
