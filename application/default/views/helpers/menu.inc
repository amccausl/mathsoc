<?php

/** Generate the menu for rendering in the sidebar.
 */

//TODO: if user authenticated, include custom admin section

function getMenu( $request = null )
{
	$menu = array(
	array(
		"title"	=> "Announcements",
		"link"	=> $request->getBaseUrl() . "/",
		"sub"	=> array(
		array(
			"title" => "Events Calendar",
			"link" => $request->getBaseUrl() . "/Events"))),
	array(
		"title"	=> "Services",
		"link"	=> $request->getBaseUrl() . "/Services",
		"sub"	=> array(
		array(
			"title"	=> "Exambank",
			"link"	=> $request->getBaseUrl() . "/exambank",
			"sub"	=> array(
			array(
				"title"	=> "Introduction",
				"link"	=> $request->getBaseUrl() . "/exambank/info"),
			array(
				"title"	=> "What is an Exam?",
				"link"	=> $request->getBaseUrl() . "/exambank/exam"),
			array(
				"title"	=> "How to Study for Exam?",
				"link"	=> $request->getBaseUrl() . "/exambank/howto"),
			array(
				"title"	=> "Can't Find an Exam?",
				"link"	=> $request->getBaseUrl() . "/exambank/find"),
			array(
				"title"	=> "Thanks",
				"link"	=> $request->getBaseUrl() . "/exambank/thanks"),
			array(
				"title"	=> "Submit an Exam",
				"link"	=> $request->getBaseUrl() . "/exambank/submit"))),
		array(
			"title"	=> "Lockers",
			"link"	=> $request->getBaseUrl() . "/Lockers",
			"sub"	=> array()),
		array(
			"title"	=> "Office",
			"link"	=> $request->getBaseUrl() . "/Office",
			"sub"	=> array(
			array(
				"title"	=> "Signup For Office Hour",
				"link"	=> $request->getBaseUrl() . "/Office/Signup"
			))),
		array(
			"title"	=> "Novelties",
			"link"	=> $request->getBaseUrl() . "/Novelties",
			"sub"	=> array(
			array(
				"title"	=> "Design Competition",
				"link"	=> $request->getBaseUrl() . "/Novelties/contest"
			))),
		array(
			"title"	=> "C&amp;D",
			"link"	=> $request->getBaseUrl() . "/RightAngleCafe",
			"sub"	=> array()),
		array(
			"title"	=> "C.I.F.",
			"link"	=> $request->getBaseUrl() . "/CIF",
			"sub"	=> array()),
		array(
			"title"	=> "Mathletics",
			"link"	=> $request->getBaseUrl() . "/Mathletics",
			"sub"	=> array()),
		array(
			"title"	=> "Student Space",
			"link"	=> $request->getBaseUrl() . "/Services/Space",
			"sub"	=> array()))),
	array(
		"title"	=> "Academics",
		"link"	=> $request->getBaseUrl() . "/Academics",
		"sub"	=> array(
		//array(
		//	"title" => "School Committees",
		//	"link" => $request->getBaseUrl() . "/Academics/Committees"),
		//array(
		//	"title" => "Talks",
		//	"link" => $request->getBaseUrl() . "/Academics/Talks"),
		array(
			"title" => "Representation",
			"link" => $request->getBaseUrl() . "/Academics/Representation"))),
	array(
		"title"	=> "Council",
		"link"	=> $request->getBaseUrl() . "/council",
		"sub"	=> array(
		//array(
		//	"title" => "Representation",
		//	"link" => $request->getBaseUrl() . "/council/Representation"),
		array(
			"title" => "Meeting Minutes",
			"link" => $request->getBaseUrl() . "/council/minutes"),
		array(
			"title" => "Bylaws",
			"link" => $request->getBaseUrl() . "/council/bylaws"),
		array(
			"title" => "Policies",
			"link" => $request->getBaseUrl() . "/council/policies"),
		array(
			"title" => "Available Positions",
			"link" => $request->getBaseUrl() . "/council/Vacancies"),
		array(
			"title" => "Join the Mailing List",
			"link" => "http://www.mathsoc.uwaterloo.ca/cgi-bin/mailman/listinfo/council"))),
	array(
		"title"	=> "Clubs",
		"link"	=> $request->getBaseUrl() .  "/Clubs",
		"sub"	=> array(
		array(
			"title"	=> "F.A.Q.",
			"link"	=> $request->getBaseUrl() . "/Clubs/FAQ"),
		array(
			"title"	=> "Join a Club",
			"link"	=> $request->getBaseUrl() . "/Clubs/join"))),
	array(
		"title"	=> "Volunteers",
		"link"	=> $request->getBaseUrl() . "/Volunteers",
		"sub"	=> array(
		array(
			"title" => "How to get Involved",
			"link"	=> $request->getBaseUrl() . "/Volunteers/GetInvolved"),
		array(
			"title" => "Volunteer Positions",
			"link"	=> $request->getBaseUrl() . "/Positions"))),
	array(
		"title"	=> "Committees",
		"link"	=> $request->getBaseUrl() . "/Committees",
		"module" => "default",
		"controller" => "committees",
		"action" => "index",
		"sub"	=> array(
		array(
			"title"	=> "Math Orientation",
			"link"	=> "http://orientation.math.uwaterloo.ca/"),
		array(
			"title"	=> "<i>math</i><b>NEWS</b>",
			"link"	=> "http://mathnews.uwaterloo.ca"),
		array(
			"title"	=> "WIM Undergrad Committee",
			"link"	=> "http://www.student.math.uwaterloo.ca/~wimugrad"),
		array(
			"title"	=> "Math Grad Committee",
			"link"	=> "http://www.mgc.uwaterloo.ca/"))),
	array(
		"title"	=> "Contact Info",
		"module" => "default",
		"controller" => "index",
		"action" => "index",
		"link"	=> $request->getBaseUrl() . "/Contact"));

	if( Zend_Auth::getInstance()->hasIdentity() )
	{	array_push( $menu, array(
				"title"	=> "Logout",
				"link"	=> $request->getBaseUrl() . "/User/Logout"
				) );
			
	}

	$requestUrl = $request->getBaseUrl() . "/";
	if( isset( $request ) )
	{	$module = $request->getModuleName();
		if( !isset( $module ) )
		{	$module = Zend_Controller_Front::getInstance()->getDispatcher()->getDefaultModule();
		}
		if( $module != Zend_Controller_Front::getInstance()->getDispatcher()->getDefaultModule() )
		{	$requestUrl .= $module . "/";
		}

		$controller = $request->getControllerName();
		if( !isset( $controller ) )
		{	$controller = Zend_Controller_Front::getInstance()->getDispatcher()->getDefaultController();
		}
		if( $controller != "index" )
		{	$requestUrl .= $controller . "/";
		}

		$action = $request->getActionName();
		if( !isset( $action ) )
		{	$action = Zend_Controller_Front::getInstance()->getDispatcher()->getDefaultAction();
		}else
		{	if( !isset( $action ) )
			{	$action = "index";
			}
		}
		if( $action != "index" )
		{	$requestUrl .= $action . "/";
		}
		
		$requestUrl = substr( strtolower( $requestUrl ), 0, -1 );

		//print( "using module '$module', controller '$controller', action '$action'\n" );

		$active1 = false;
		$active2 = false;
		foreach( $menu as $index1 => $menuitem1 )
		{
			if( strtolower($menuitem1['link']) == $requestUrl )
			{	$menu[$index1]['status'] = 'active selected';
			}else
			{	$menu[$index1]['status'] = 'inactive';
			}
			
			if( isset( $menuitem1['sub'] ) )
			{	foreach( $menuitem1['sub'] as $index2 => $menuitem2 )
				{	if( strtolower($menuitem2['link']) == $requestUrl )
					{	$menu[$index1]['status'] = 'active';
						$menu[$index1]['sub'][$index2]['status'] = 'active selected';
					}else
					{	$menu[$index1]['sub'][$index2]['status'] = 'inactive';
					}
					
					if( isset( $menuitem2['sub'] ) )
					{	foreach( $menuitem2['sub'] as $index3 => $menuitem3 )
						{	if( strtolower($menuitem3['link']) == $requestUrl )
							{	$menu[$index1]['status'] = 'active';
								$menu[$index1]['sub'][$index2]['status'] = 'active';
								$menu[$index1]['sub'][$index2]['sub'][$index3]['status'] = 'active selected';
							}else
							{	$menu[$index1]['sub'][$index2]['sub'][$index3]['status'] = 'inactive';
							}
						}
					}
				}
			}
		}
	}
/*
	print( "<pre>" );
	print( "requestUrl = $requestUrl\n" );
	print_r( $menu );
	print( "</pre>" );
*/
// Todo: Switch links to call the URL generator function or use preset module, controller etc
// Todo: Add page specific cacheing
// Todo: Add dynamic menu based on permissions
	return $menu;
}
