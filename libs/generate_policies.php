<?php

$baseurl = "http://localhost/code/mathsoc";
$menu = file( "menu.src", FILE_USE_INCLUDE_PATH );
$main_menu = array();

for( $i = 1; $i <= count( $menu ); $i++ )
{
	if( $menu[$i-1][0] != "#" && $menu[$i-1] )
	{	$match = false;
		if( preg_match( '/^[\s]{4}([\S]+.*[\S])[\s]+([\S]+)$/', $menu[$i-1], $matches ) )
			$match = "***";
		elseif( preg_match( '/^[\s]{2}([\S]+.*[\S])[\s]+([\S]+)$/', $menu[$i-1], $matches ) )
			$match = "**";
		elseif( preg_match( '/^([\S]+.*[\S])[\s]+([\S]+)$/', $menu[$i-1], $matches ) )
			$match = "*";
		if( $matches[2][0] == "/" )
		{	$menu[$i-1] = "$match [[$baseurl{$matches[2]}|{$matches[1]}]]";
		}else
		{	$menu[$i-1] = "$match [[{$matches[2]}|{$matches[1]}]]";
		}
		$main_menu[$i-1] = $menu[$i-1];
		if( $matches[2] == "/council/" )
		{	$menu[$i-1] = "$match %25apply=item class=active%25 [[$baseurl{$matches[2]}|{$matches[1]}]]";
			$main_menu[$i-1] = "$match %25apply=item id=selected%25 [[$baseurl{$matches[2]}|{$matches[1]}]]";
		}elseif( $matches[1] == "Bylaws &amp; Policies" )
		{	$menu[$i-1] = "$match %25apply=item id=selected%25 [[$baseurl{$matches[2]}|{$matches[1]}]]";

		}
	}else
	{	unset( $menu[$i-1] );
	}
}

foreach( array( "../wiki.d/Policies.SideBar" => $menu, "../wiki.d/Site.SideBar" => $main_menu ) as $file => $contents)
{	print( "Generating File '$file' as menu from menu.src\n" );
	$contents = implode( '%0a', $contents );
	$output = fopen( $file, "w" );
	fwrite( $output, "version=pmwiki-2.1.27 ordered=1 urlencoded=1
agent=Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.8.1.6) Gecko/20071008 Ubuntu/7.10 (gutsy) Firefox/2.0.0.6
author=amccausl
csum=
ctime=1206089498
host=127.0.0.1
name=Policies.SideBar
rev=1
targets=
text=$contents%0a" );
	fclose( $output );
}

$policies = file( "./policies.src" );

// Make some variables to output the index
$targets = array();
$text = "";

for( $i = 1; $i <= count( $policies ); $i++ )
{	$policies[$i-1] = chop( $policies[$i-1] );
	// Generate Individual Policy Files
	// Generate Policy File
	$filename = sprintf( "Policies.Policy%02d", $i );
	$policy = strtr(ucwords($policies[$i-1]), array(" " => "", "," => "") );
	$file_contents = "version=pmwiki-2.1.27 ordered=1 urlencoded=1
agent=Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.8.1.6) Gecko/20071008 Ubuntu/7.10 (gutsy) Firefox/2.0.0.6
author=amccausl
csum=
ctime=1206089498
host=127.0.0.1
name=$filename
rev=1
targets=Policies.$policy
text=(:include $policy:)%0a----%0a%0a[[Comments/Policies-{$policies[$i-1]}?action=edit|(Add Comment)]]%0a%0a(:include Comments/Policies-{$policy}:)%0a----";

	if( file_exists( "../wiki.d/Policies.$policy" ) )
		print( "File '$filename', Policy '$policy'\n" );
	else
		print( "*** Missing $filename '{$policies[$i-1]}'\n" );

	// Create Policy File
	$output = fopen( "../wiki.d/$filename", "w" );
	fwrite( $output, $file_contents );
	fclose( $output );

	// Add information to index
	array_push( $targets, $filename );
	$text .= "# [[$filename|{$policies[$i-1]}]]%0a";
}

$targets = implode( ",", $targets );
$output = fopen( "../wiki.d/Policies.Policies", "w" );
fwrite( $output, "version=pmwiki-2.1.27 ordered=1 urlencoded=1
agent=Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.8.1.6) Gecko/20071008 Ubuntu/7.10 (gutsy) Firefox/2.0.0.6
author=amccausl
csum=
ctime=1206089498
host=127.0.0.1
name=Policies.Policies
rev=1
targets=$targets
text=$text" );
fclose( $output );

?>
