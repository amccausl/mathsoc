<div class="section">
<div class='form-container'>
<form action='{$smarty.server.REQUEST_URI}' method='post'>
<fieldset>
  <legend>Member Refund Rejections</legend>
  <label for='user_id'>User Id:</label>
    {html_options name=user options=$user_options selected=$user}<br/>
  <label for='user_reason'>Reason for rejection:</label>
    <input name='user_reason' type='textbox'>{$user_reason}</textbox><br/>
</fieldset>
<div class='buttonrow'>
  <input name='renew' type='submit' value='Reject Refund Request' class='button'/> 
</div>
</form>
</div>
</div>
