<?php

require_once 'MathSocAction.inc';

class Admin_UserController extends MathSoc_Controller_Action
{
	// The database object to retrieve information from
	private $db;

	public function init($secure = true)
	{	parent::init($secure);

		// User must be authenticated to see any of these pages
	}

	/** /admin/user
	 *
	 */
	public function indexAction()
	{
	}
}
