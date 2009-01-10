<?php

require_once 'MathSocAction.inc';
require_once 'clubsDB.inc';

class Admin_ClubsController extends MathSoc_Controller_Action
{
	private $db;

	public function init($secure = true)
	{	parent::init($secure);

		$this->db = new ClubsDB();
	}

	public function indexAction()
	{	
	}
}
