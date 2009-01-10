<?php

require_once 'MathSocAction.inc';

class Admin_MathleticsController extends MathSoc_Controller_Action
{
	private $db;

	public function init($secure = true)
	{	parent::init($secure);

		// User must be authenticated to see any of these pages
		// TODO: check that the user is mathletics director or above
	}

	/** /admin/mathletics - Display current applications for funding
	 *
	 */
	public function indexAction()
	{
	}
}
