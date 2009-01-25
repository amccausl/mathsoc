<div id="main-header"><h1></h1></div>
<div class="section" style="width: 700px">
  <form action="{$smarty.server.REQUEST_URI}" method="post">
  <table>
    <tr><td>Course Prefix:</td><td>{html_options name=course_prefix options=$prefix_options selected=$course_prefix}</td></tr>
    <tr><td>Course Number:</td><td><input type='text' name='course_code' size='4' value='{$course_code}' /></td></tr>
    <tr><td>Term:</td><td>{html_options name=term options=$term_options selected=$term}</td></tr>
    <tr><td>Status:</td><td>{html_options name=status options=$status_options selected=$status}</td></tr>
    <tr><td colspan='2' align='center'><input type='submit' name='filter' value='Filter' /></td></tr>
  </table>
  <table width="700">
    <thead><tr>
      <th scope="col">Course</th>
      <th scope="col">Term</th>
      <th scope="col">Type</th>
      <th scope="col">Uploader</th>
      <th scope="col">Exam</th>
      <th scope="col">Solutions</th>
      <th scope="col">Actions</th>
    </tr></thead>
    <tbody>
{foreach from=$exams item=exam}
      <tr>
        <th scope="row">{$exam.course}</th>
        <td>{term id=$exam.term}</td>
        <td>{$exam.type} {$exam.number}{if $exam.practise} (Practise){/if}</td>
        <td>{$exam.uploader}</td>
        <td>{if $exam.has_questions}<a href="{$baseUrl}/admin/exambank/index/exam/{$exam.id}">View</a>{/if}</td>
        <td>{if $exam.has_solutions}<a href="{$baseUrl}/admin/exambank/index/solutions/{$exam.id}">View</a>{/if}</td>
        <td>
          <input type="submit" name="approve_{$exam.id}" value="Approve" {if $exam.status == 'approved'}disabled="disabled"{/if}/>
          <input type="submit" name="reject_{$exam.id}" value="Reject" {if $exam.status == 'rejected'}disabled="disabled"{/if}/>
          <input type="submit" name="update_{$exam.id}" value="Update" disabled="disabled"/>
        </td>
      </tr>
{/foreach}
    </tbody>
  </table>
  </form>
</div>
