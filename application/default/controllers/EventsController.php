<?php

require_once 'MathSocAction.inc';

class EventsController extends MathSoc_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();
		//$this->db = $this->db = new AnnounceDB();
	}

	public function indexAction()
	{	//$this->view->array_push("stylesheets", "http://localhost/zend/html/phpicalendar/templates/mathsoc/default.css");
	}
}

