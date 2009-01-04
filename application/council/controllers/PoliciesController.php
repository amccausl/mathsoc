<?php

require_once 'MathSocAction.inc';

require_once 'policyDB.inc';

// Include module to display version differences
require_once 'diff.php';
include_once "Text/Diff.php";
include_once "Text/Diff/Renderer.php";
include_once "Text/Diff/Renderer/inline.php";

class Council_PoliciesController extends MathSoc_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();

		set_time_limit(0);

		// User must be authenticated to see any of these pages
		$this->view->baseUrl = $this->_request->getBaseUrl();
		$stylesheets = $this->view->stylesheets;
		array_push( $stylesheets, $this->_request->getBaseUrl() . "/css/policies.css" );
		$this->view->stylesheets = $stylesheets;
		$this->initView();
		$this->db = new PolicyDB();

		$menu = $this->view->menu;
		$menu[3]['status'] = "active";
		$menu[3]['sub'][2]['status'] = "selected";
		$this->view->menu = $menu;
	}

	// Display an table of contents for the policies
	public function indexAction()
	{	// Database lookup for current policies, dump to view
		$this->view->policies = $this->db->getPolicies();
	}

	// Display a given policy
	public function displayAction()
	{	// Look up policy with the given url parameters
		$policy = $this->db->getPolicy($this->_getParam('page'));
		$this->view->policy = $policy;
	}

	// Display a diff between two different versions of a policy
	public function diffAction()
	{	// Grab the parameters from the url
		$id = $this->_getParam('page');
		$version1 = $this->_getParam('version1');
		$version2 = $this->_getParam('version2');

		// Grab the policies from the database
		$version1 = $this->db->getPolicy($id, $version1);
		$version2 = $this->db->getPolicy($id, $version2);

		/*
		print_r( $version1 );
		print_r( $version2 );

		// Render the difference in the names
		$diff_name = new diff( $version1['name'], $version2['name'] );
		$this->view->name = $diff_name->render();

		// Render the difference in the contents
		$diff_content = new diff( $version1['content'], $version2['content'] );
		$this->view->content = $diff_content->render();
		*/

		$renderer = new Text_Diff_Renderer_inline();
		$diff = new Text_Diff(split("\n", $version1['name']), split("\n", $version2['name']));
		$this->view->name = $renderer->render($diff);

		// perform diff, print output
		$diff = new Text_Diff(split("\n", $version1['content']), split("\n", $version2['content']));
		$content = $renderer->render($diff);
		$content = strtr( $content, array( "&lt;" => "<", "&gt;" => ">"));
		$this->view->content = strtr( $content, array( "<del><li>" => "<li><del>", "</li></del>" => "</del></li>" ) );
	}

	// Add a new version for a given policy
	public function updateAction()
	{
		$auth = Zend_Auth::getInstance();
		print( "auth = " . $auth->hasIdentity() );

		if (!$auth->hasIdentity())
		{	$this->_redirect('auth/login');
		}
	}
}
