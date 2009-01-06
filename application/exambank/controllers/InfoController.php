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
	}
	
	public function examAction()
	{	$menu = $this->view->menu;
		$menu[1]['sub'][0]['sub'][1]['status'] = 'active selected';
	}
	
	public function howtoAction()
	{	$menu = $this->view->menu;
		$menu[1]['sub'][0]['sub'][2]['status'] = 'active selected';
	}
	
	public function findAction()
	{	$menu = $this->view->menu;
		$menu[1]['sub'][0]['sub'][3]['status'] = 'active selected';
	}
	
	public function thanksAction()
	{	$menu = $this->view->menu;
		$menu[1]['sub'][0]['sub'][4]['status'] = 'active selected';
	}
	
}
