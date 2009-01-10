<?php

require_once 'MathSocAction.inc';

class Admin_IndexController extends MathSoc_Controller_Action
{
	private $db;

	public function init($secure = true)
	{	parent::init($secure);

		// TODO: set menu admin item dynamically depending on the user positions
	}

	/** /admin/ 
	 *
	 */
	public function indexAction()
	{
	}
}
