<?php

require_once( "smarty/SmartyValidate.class.php" );

function file_upload( $name, $destination, $filename = null )
{
	if( $_FILES[$name]['error'] === UPLOAD_ERR_OK )
	{
		if( is_dir( $destination ) )
		{	$hash = ($filename == null ? hash_file( "md5", $_FILES[$name]['tmp_name'] ) : $filename);
			if( move_uploaded_file( $_FILES[$name]['tmp_name'], $destination . $hash ) )
			{	chmod( $destination . $hash, 0575 );
				return array( UPLOAD_ERR_OK, $hash );
			}
		}else
		{	if( move_uploaded_file( $_FILES[$name]['tmp_name'], $destination ) )
			{	chmod( $destination . $hash, 0575 );
				return array( UPLOAD_ERR_OK, $hash );
			}
		}
		return array( UPLOAD_ERR_CANT_WRITE );
	}
	return array( $_FILES[$name]['error'] );
}
