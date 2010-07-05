<div id='main-header'><h1></h1></div>
{*<div class="section">Locker signups will become available on Monday, January 11th.</div>*}
<div class="section">
{if $message}
  <p>{$message}</p>
{elseif $current_locker}
  <p>You are currently signed up for locker #{$current_locker}.  If your locker is about to expire, you can renew it by clicking 'Sign up For Locker'.</p>
{else}
  <p>You are not currently signed up for a locker.</p>
{/if}

{if $validate.default.is_error}
{validate id="email_element" message="You must provide a valid email address<br/>"}
{/if}

<div class='form-container'>
<form action='{$smarty.server.REQUEST_URI}' method='post'>
<fieldset>
  <legend>Locker Information</legend>
  <label for='locker_id'>Locker Number:</label>
    <input name='locker_id' type='text' length='4' value='{$locker_id}' /> <a href="{$baseUrl}/lockers/map">Pick a Locker</a><br/>
  <label for='username'>Username:</label>
    <input name='username' type='text' length='8' disabled='true' value='{$username}' /><br/>
  <label for='email'>Email:</label>
    <input name='email' type='text' length='30' disabled='true' value='{$email}' /><br/>
  <label for='locker_current_phone'>Phone Number:</label>
    <input name='locker_current_phone' type='text' length='12' value='{$locker_current_phone}' /><br/>
  <label for='locker_combo'>Combo: (Optional)</label>
    <input name='locker_combo' type='text' length='10' value='{$locker_combo}' /><br/>
  <label for='expires'>Expires:</label>
    <input name='expires' type='text' length='15' disabled='true' value='{$expires}' /><br/>
</fieldset>
<div class='buttonrow'>
  <input name='submit' type='submit' value='Sign up For Locker' class='button'/> 
</div>

</form>
</div>
