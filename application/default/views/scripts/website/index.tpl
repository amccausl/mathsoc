<div id='main-header'><h1></h1></div>
<div class="section">
{if $message}
  <p>{$message}</p>
{/if}

  <p>Welcome to MathSoc's new website.  Have comments?  Concerns?  Found a bug?  Leave us a note and we'll try to get back to you ASAP.</p>

  <div class='form-container'>
  <form action='{$smarty.server.REQUEST_URI}' method='post'>
  <fieldset>
    <legend>Website Poll</legend>
	<label for='prefer_new'>Do you prefer the new website to the old?</label><br/>
      <input type="radio" name="prefer_new" value="Yes" checked/>Yes<br>
      <input type="radio" name="prefer_new" value="No"/>No<br>

    <label for='comments'>What comments do you have?</label>
      <textarea name="comments" cols="40" rows="5">Enter your comments or concerns here...</textarea><br/>

    <label for='bugs'>Would you like to report a bug or request a feature?</label>
      <textarea name="bugs" cols="40" rows="5">Enter your bug description here...</textarea><br/>

    <label for='email'>If you would like to be contacted for further information, include an email:</label>
      <input name='email' type='text' length='30' value='{$email}' /><br/>
    <input type="submit" name='submit' value="Submit" />	  
  </fieldset>
  </form>
  </div>
</div>
