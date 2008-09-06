<?php

/** Generate the menu for rendering in the sidebar.
 */

$menu = array(
	array(
		"title"	=> "Announcements",
		"link"	=> "",
		"sub"	=> array(
		array(
			"title" => "Events Calendar",
			"link" => "/Events"))),
	array(
		"title"	=> "Services",
		"link"	=> "/Services",
		"status"=> "active",
		"sub"	=> array(
		array(
			"title"	=> "Exambank",
			"link"	=> "/Exambank",
			"status"=> "selected",
			"sub"	=> array()),
		array(
			"title"	=> "Lockers",
			"link"	=> "/Lockers",
			"sub"	=> array()),
		array(
			"title"	=> "Office",
			"link"	=> "/Office",
			"sub"	=> array()),
		array(
			"title"	=> "Novelties",
			"link"	=> "/Novelties",
			"sub"	=> array()),
		//array(
		//	"title"	=> "C&D",
		//	"link"	=> "/CandD",
		//	"sub"	=> array()),
		array(
			"title"	=> "C.I.F.",
			"link"	=> "/Services/CIF",
			"sub"	=> array()),
		array(
			"title"	=> "Mathletics",
			"link"	=> "/Mathletics",
			"sub"	=> array()),
		array(
			"title"	=> "Student Space",
			"link"	=> "/Services/Space",
			"sub"	=> array()))),
	array(
		"title"	=> "Academics",
		"link"	=> "/Academics",
		"sub"	=> array(
		array(
			"title" => "School Committees",
			"link" => "/Academics/Committees"),
		array(
			"title" => "Representation",
			"link" => "/Academics/Reps"),
		array(
			"title" => "Further Information",
			"link" => "/Academics/Info"))),
	array(
		"title"	=> "Council",
		"link"	=> "/Council",
		"sub"	=> array(
		array(
			"title" => "Representation",
			"link" => "/Council/Representation"),
		array(
			"title" => "Meeting Minutes",
			"link" => "/Council/minutes"),
		array(
			"title" => "Bylaws & Policies",
			"link" => "/Council/Pinkbook"),
		array(
			"title" => "Available Positions",
			"link" => "/Council/Vacancies"),
		array(
			"title" => "Join the Mailing List",
			"link" => "http://www.mathsoc.uwaterloo.ca/cgi-bin/mailman/listinfo/council"))),
	array(
		"title"	=> "Clubs",
		"link"	=> "/Clubs",
		"sub"	=> array(
		array(
			"title"	=> "Join a Club",
			"link"	=> "/Clubs/join"),
		array(
			"title"	=> "",
			"link"	=> ""))),
	array(
		"title"	=> "Volunteers",
		"link"	=> "/Volunteers",
		"sub"	=> array(
		array(
			"title" => "How to get Involved",
			"link"	=> "/Volunteers/GetInvolved"))),
	array(
		"title"	=> "Committees",
		"link"	=> "/Committees",
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
		"link"	=> "/Contact"));

// Todo: determine where the user is from the URL
// Todo: Switch links to call the URL generator function
// Todo: Add page specific cacheing
