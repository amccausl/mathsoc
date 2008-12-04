<?php

require_once 'MathSocAction.inc';

class Admin_IndexController extends MathSoc_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();

		// User must be authenticated to see any of these pages
		$this->initView();
		//$this->view->user = Zend_Auth::getInstance()->getIdentity();
	}

    function preDispatch()
	{
		$auth = Zend_Auth::getInstance();

		if (!$auth->hasIdentity())
		{	$this->_redirect('user/login');
		}
	}

	// Browsing Functions
	public function indexAction()
	{	// List the existing exams
	}

	public function submitAction()
	{
		session_start();
		require_once( "../application/default/views/helpers/form.inc" );

		// Generate HTML elements for the form
		$this->view->prefix_options = $this->db->getCourses(null, null, true);

		$terms = array();
		$term = (date('Y') - 1900) . (1 + 4*(ceil(date('m') / 4) - 1));
		$seasons = array( "W", "S", "F" );
		for( $i = 0; $i < 15; $i++ )
		{ 
			$terms[$term] = $seasons[floor($term%10/4)] . " " . (floor($term/10) + 1900);

			if( $term % 10 == 1 )
				$term -= 2;
			else
				$term -= 4;
		}
		$this->view->term_options = $terms;

		// Include form validation
		$smarty = $this->view->getEngine();
		if(empty($_POST)) { // || empty($_SESSION) || !isset($_SESSION['SmartyValidate'])) {
			SmartyValidate::connect($smarty, true);
			SmartyValidate::register_validator('prefix_element','course_prefix','notEmpty');
			SmartyValidate::register_validator('code_element','course_code','isCourseCode');
			SmartyValidate::register_validator('term_element','course_term','isTerm');
			// TODO: find way to validate only one of the file inputs
		} else {
			SmartyValidate::connect($smarty);
			// validate after a POST
			if(SmartyValidate::is_valid($_POST)) {
				// TODO: insert entry to database

				// TODO: set success message

				// no errors, done with SmartyValidate
				SmartyValidate::disconnect();
			}
			
			$this->view->assign($_POST);
		}
	}
}
