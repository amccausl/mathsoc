<div class="section">
{if $validate.default.is_error}
{validate id="email_element" message="You must provide a valid email address<br/>"}
{/if}

<div class='form-container'>
<form action='{$smarty.server.REQUEST_URI}' method='post'>
<fieldset>
  <legend>Locker Information</legend>
  <label for='locker_id'>Locker Number:</label>
    <input name='locker_id' type='text' length='4' value='{$locker_id}' /><br/>
  <label for='username'>Username:</label>
    <input name='username' type='text' length='8' disabled='true' value='{$username}' /><br/>
  <label for='email'>Email:</label>
    <input name='email' type='text' length='30' disabled='true' value='{$email}' /><br/>
  <label for='locker_current_phone'>Phone Number:</label>
    <input name='locker_current_phone' type='text' length='12' value='{$locker_current_phone}' /><br/>
  <label for='locker_combo'>Combo: (Optional)</label>
    <input name='locker_combo' type='text' length='10' value='{$locker_combo}' /><br/>
  <label for='locker_expires'>Expires:</label>
    <input name='locker_expires' type='text' length='15' disabled='true' value='{$locker_expires}' /><br/>
</fieldset>
<div class='buttonrow'>
  <input name='renew' type='submit' value='Renew Locker' class='button'/> 
  <input name='update' type='submit' value='Update Information' class='button'/> 
  <input name='change' type='submit' value='Change Locker' class='button'/> 
</div>
</form>
</div>
</div>
