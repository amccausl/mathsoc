<?php

require_once 'Zend/Controller/Action.php';

class AuthController extends Zend_Controller_Action
{
    public function indexAction()
	{	$smarty = Zend_Registry::get('smarty');
		$smarty->assign('title', 'Mathematics Society / Authentication');
		$smarty->assign('stylesheets', array('/css/main.css'));
		$smarty->display('main.tpl');
    }

	public function loginAction()
	{	$smarty = Zend_Registry::get('smarty');
		$smarty->assign('title', 'Mathematics Society / Authentication');
		$smarty->assign('stylesheet', '/css/main.css');
	}

	public function logoutAction()
	{	$smarty = Zend_Registry::get('smarty');
		$smarty->assign('title', 'Mathematics Society / Authentication');
		$smarty->assign('stylesheet', '/css/main.css');
	}
}
