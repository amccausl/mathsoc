<?php

require_once 'MathSocAction.inc';
require_once 'clubsDB.inc';

class Admin_ClubsController extends MathSoc_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();

		$this->db = new ClubsDB();
	}

	public function indexAction()
	{	
	}
}
