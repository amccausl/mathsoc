function toggleVisibility(nr,t)
{
	if( t == 'show' )
	{	document.getElementById(nr).style.display = 'block';
	}else
	{	document.getElementById(nr).style.display = 'none';
	}
}

function toggleVisibilityOption(sender,nr)
{	if( document.getElementById(sender).value == 'show' )
	{	document.getElementById(nr).style.display = 'block';
	}else
	{	document.getElementById(nr).style.display = 'none';
	}
}
