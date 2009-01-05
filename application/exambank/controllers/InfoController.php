<?php

require_once 'MathSocAction.inc';

class Exambank_InfoController extends MathSoc_Controller_Action
{	public function init()
	{	parent::init();

		$menu = $this->view->menu;
		$menu[1]['status'] = "active";
		$menu[1]['sub'][0]['status'] = "active";

		$this->view->menu = $menu;
	}

	// Add a few static pages
	public function indexAction()
	{	$menu = $this->view->menu;
		$menu[1]['sub'][0]['sub'][0]['status'] = 'selected';
	}
	
	public function examAction()
	{	$menu = $this->view->menu;
		$menu[1]['sub'][0]['sub'][1]['status'] = 'selected';
	}
	
	public function howtoAction()
	{	$menu = $this->view->menu;
		$menu[1]['sub'][0]['sub'][2]['status'] = 'selected';
	}
	
	public function findAction()
	{	$menu = $this->view->menu;
		$menu[1]['sub'][0]['sub'][3]['status'] = 'selected';
	}
	
	public function thanksAction()
	{	$menu = $this->view->menu;
		$menu[1]['sub'][0]['sub'][4]['status'] = 'selected';
	}
	
}
