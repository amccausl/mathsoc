<div class="section">
<div class='form-container'>
<form action='{$smarty.server.REQUEST_URI}' method='post'>
  <p>The system assumes no one is in classes for two consecutive terms, and thus denies
  access to students who received a refund last term.  If a student comes to you wondering
  why they don't have access, you should tell them to drop by and have them log into quest.
  make sure they've paid their fee this term, then add them to this list.
<fieldset>
  <legend>Member Allowed Access</legend>
  {$user_allowed}<br/>
  <label for='user'>User Id:</label>
    {html_options name=user values=$user_options output=$user_options selected=$user}<br/>
</fieldset>
<div class='buttonrow'>
  <input name='renew' type='submit' value='Allow User Access' class='button'/> 
</div>
</form>
</div>
</div>
