<div class="section">
  <p>Please ensure that you have permission share your exam before you post it.  If you have any problems with the submission form, you can email <a href='mailto:exambank@mathsoc.uwaterloo.ca'>exambank@mathsoc</a> for help.</p>
  <p style='color:{$colour}; font-weight:bold'>
  {validate id="fname" message="Full Name cannot be empty"}
  </p>
  <form action='{$smarty.server.REQUEST_URI}' enctype='multipart/form-data' method='post'>
  <table>
    <tr><td>Course Prefix:</td><td>{$prefix_field}</td></tr>
    <tr><td>Course Number:</td><td><input type='text' name='course_number' size='4' value='' /></td></tr>
    <tr><td>Term:</td><td>{$term_field}</td></tr>
    <tr><td>Exam Type:</td><td>{$examtype_field} #<input type='text' name='index' size='2' value=''/></td></tr>
    <tr><td>Pratice:</td><td><input type='checkbox' name='practice' {$is_practice}/></td></tr>
    <tr><td>Exam:</td><td><input type='file' name='exam' size='20' value='' /></td></tr>
    <tr><td>Solutions:</td><td><input type='file' name='solutions' size='20' value=''/></td></tr>
    <tr><td colspan='2' align='center'><input type='submit' name='submit' value='Submit' /></td></tr>
  </table>
  </form>
</div>
