<?php

/** Generate the menu for rendering in the sidebar.
 */

function getMenu( $request = null )
{
	$menu = array(
	array(
		"title"	=> "Announcements",
		"link"	=> "",
		"sub"	=> array(
		array(
			"title" => "Events Calendar",
			"link" => $request->getBaseUrl() . "/Events"))),
	array(
		"title"	=> "Services",
		"link"	=> $request->getBaseUrl() . "/Services",
		"module" => "default",
		"controller" => "index",
		"action" => "index",
		"sub"	=> array(
		array(
			"title"	=> "Exambank",
			"link"	=> $request->getBaseUrl() . "/Exambank",
			"status"=> "selected",
			"sub"	=> array()),
		array(
			"title"	=> "Lockers",
			"link"	=> $request->getBaseUrl() . "/Lockers",
			"sub"	=> array()),
		array(
			"title"	=> "Office",
			"link"	=> $request->getBaseUrl() . "/Office",
			"sub"	=> array()),
		array(
			"title"	=> "Novelties",
			"link"	=> $request->getBaseUrl() . "/Novelties",
			"sub"	=> array()),
		//array(
		//	"title"	=> "C&D",
		//	"link"	=> "/CandD",
		//	"sub"	=> array()),
		array(
			"title"	=> "C.I.F.",
			"link"	=> $request->getBaseUrl() . "/Services/CIF",
			"sub"	=> array()),
		array(
			"title"	=> "Mathletics",
			"link"	=> $request->getBaseUrl() . "/Services/Mathletics",
			"sub"	=> array()),
		array(
			"title"	=> "Student Space",
			"link"	=> $request->getBaseUrl() . "/Services/Space",
			"sub"	=> array()))),
	array(
		"title"	=> "Academics",
		"link"	=> $request->getBaseUrl() . "/Academics",
		"sub"	=> array(
		array(
			"title" => "School Committees",
			"link" => $request->getBaseUrl() . "/Academics/Committees"),
		array(
			"title" => "Representation",
			"link" => $request->getBaseUrl() . "/Academics/Reps"),
		array(
			"title" => "Further Information",
			"link" => $request->getBaseUrl() . "/Academics/Info"))),
	array(
		"title"	=> "Council",
		"link"	=> $request->getBaseUrl() . "/Council",
		"sub"	=> array(
		array(
			"title" => "Representation",
			"link" => $request->getBaseUrl() . "/Council/Representation"),
		array(
			"title" => "Meeting Minutes",
			"link" => $request->getBaseUrl() . "/Council/minutes"),
		array(
			"title" => "Bylaws & Policies",
			"link" => $request->getBaseUrl() . "/Council/policies"),
		array(
			"title" => "Available Positions",
			"link" => $request->getBaseUrl() . "/Council/Vacancies"),
		array(
			"title" => "Join the Mailing List",
			"link" => "http://www.mathsoc.uwaterloo.ca/cgi-bin/mailman/listinfo/council"))),
	array(
		"title"	=> "Clubs",
		"link"	=> "/Clubs",
		"sub"	=> array(
		array(
			"title"	=> "Join a Club",
			"link"	=> $request->getBaseUrl() . "/Clubs/join"),
		array(
			"title"	=> "",
			"link"	=> ""))),
	array(
		"title"	=> "Volunteers",
		"link"	=> $request->getBaseUrl() . "/Volunteers",
		"sub"	=> array(
		array(
			"title" => "How to get Involved",
			"link"	=> $request->getBaseUrl() . "/Volunteers/GetInvolved"))),
	array(
		"title"	=> "Committees",
		"link"	=> $request->getBaseUrl() . "/Committees",
		"sub"	=> array(
		array(
			"title"	=> "Math Orientation",
			"link"	=> "http://orientation.math.uwaterloo.ca/"),
		array(
			"title"	=> "<i>math</i><b>NEWS</b>",
			"link"	=> "http://mathnews.uwaterloo.ca"),
		array(
			"title"	=> "Math Grad Committee",
			"link"	=> "http://www.student.math.uwaterloo.ca/"))),
	array(
		"title"	=> "Contact Info",
		"link"	=> $request->getBaseUrl() . "/Contact"));

	if( isset( $request ) )
	{	$module = $request->getModuleName();
		if( !isset( $module ) )
		{	$module = Zend_Controller_Front::getInstance()->getDispatcher()->getDefaultModule();
		}

		print( "using module '$module'\n" );

		foreach( $menu as $index1 => $menuitem1 )
		{	if( (isset( $menuitem['module'] )
					&& $menuitem['module'] == strtolower($module))
				|| strtolower($module) == 'default' )
			{
				foreach( $menuitem1['sub'] as $index2 => $menuitem2 )
				{	if( isset( $menuitem2['controller'] )
					$menu[$index]['status'] = 'active';
				}
				// TODO: recurse on the sub array
			}
		}
	}

// Todo: determine where the user is from the URL, generate specialized menu
// Todo: Switch links to call the URL generator function or use preset module, controller etc
// Todo: Add page specific cacheing
	return $menu;
}
