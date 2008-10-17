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
  <form action='{$smarty.server.REQUEST_URI}' enctype='multipart/form-data' method='post'>
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
  </form>
</div>
