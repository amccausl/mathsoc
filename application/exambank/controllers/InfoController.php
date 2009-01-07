<?php

require_once 'MathSocAction.inc';

class Exambank_InfoController extends MathSoc_Controller_Action
{	public function init($secure = false)
	{	parent::init($secure);

		$menu = $this->view->menu;
		$menu[1]['status'] = "active";
		$menu[1]['sub'][0]['status'] = "active";

		$this->view->menu = $menu;
	}

	// Add a few static pages
	public function indexAction()
	{	$menu = $this->view->menu;
		$menu[1]['sub'][0]['sub'][0]['status'] = 'active selected';
		$this->view->menu = $menu;
	}
	
	public function examAction()
	{
	}
	
	public function howtoAction()
	{
	}
	
	public function findAction()
	{
	}
	
	public function thanksAction()
	{
	}
	
}
