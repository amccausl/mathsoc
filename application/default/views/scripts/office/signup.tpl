<div class="section">
  <p>Please ensure that you have permission share your exam before you post it.  If you have any problems with the submission form, you can email <a href='mailto:exambank@mathsoc.uwaterloo.ca'>exambank@mathsoc</a> for help.</p>
  <p style='color:{$colour}; font-weight:bold'>
  {if $validate.default.is_error}
  {validate id="prefix_element" message="You must select a course prefix<br/>"}
  {validate id="code_element" message="You must select a valid course code<br/>"}
  {validate id="term_element" message="You must select the term for the exam<br/>"}
  {else}
  Your exam has been successfully submitted.
  {/if}
  </p>
  <table>
    <tr><td>Course Prefix:</td><td>{html_options name=course_prefix options=$prefix_options selected=$course_prefix}</td></tr>
    <tr><td>Course Number:</td><td><input type='text' name='course_code' size='4' value='{$course_code}' /></td></tr>
    <tr><td>Term:</td><td>{html_options name=course_term options=$term_options selected=$course_term}</td></tr>
    <tr><td>Exam Type:</td><td>{html_options name=exam_type options=$type_options select=$exam_type} #<input type='text' name='index' size='2' value='{$exam_index}'/></td></tr>
    <tr><td>Pratice:</td><td><input type='checkbox' name='practice' {$is_practice}/></td></tr>
    <tr><td>Exam:</td><td><input type='file' name='exam_file' size='20' value='{$exam_file}' /></td></tr>
    <tr><td>Solutions:</td><td><input type='file' name='exam_solutions' size='20' value='{$exam_solutions}'/></td></tr>
    <tr><td colspan='2' align='center'><input type='submit' name='submit' value='Submit' /></td></tr>
  </table>

  <div class='form-container'>
    <form action='{$smarty.server.REQUEST_URI}' method='post'>
    <fieldset>
      <legend>Locker Information</legend>
      <label for='locker_id'>Locker Number:</label>
        <input name='locker_id' type='text' length='4' disabled='true' value='{$locker_id}' /><br/>
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
