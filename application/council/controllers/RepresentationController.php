<?php

require_once 'MathSocAction.inc';

class Council_RepresentationController extends MathSoc_Controller_Action
{
	private $db;

	public function init($secure = false)
	{	parent::init($secure);
	}

	public function indexAction()
	{
	}
}
