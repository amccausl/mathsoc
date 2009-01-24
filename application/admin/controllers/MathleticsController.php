<?php

require_once 'MathSocAction.inc';

class Admin_MathleticsController extends MathSoc_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();

		// User must be authenticated to see any of these pages
		$this->admins = array( 'mathsoc' => array( 'mathletics' => 'current' ) );
		$this->secure();
	}

	/** /admin/mathletics - Display current applications for funding
	 *
	 */
	public function indexAction()
	{
	}
}
