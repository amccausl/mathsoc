<?php

function makeSelectElement( $id, $name, $options = array(), $selected = null )
{
	if( !empty( $options ) )
	{	$html_options = "";

		foreach($options as $key => $label)
		{
			if( $key == $selected )
				$html_options .= "<option selected value='{$key}'>{$label}</option>";
			else
				$html_options .= "<option value='{$key}'>{$label}</option>";
		}
		$html_select = "<select id='{$id}' name='{$name}'>{$html_options}</select>";
	}else
	{   $html_select = "<input type='text' name='course_prefix' value='{$_POST['course_prefix']}' />";
	}

	return $html_select;
}
