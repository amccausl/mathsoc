<div id="main-header"><h1></h1></div>
<div class="section" style="width: 700px">
  <form action="{$smarty.server.REQUEST_URI}" method="post">
  <table width="700">
    <caption>Unapproved Exams</caption>
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
        <td>{if $exam.has_questions}<a href="{$baseUrl}/admin/exambank/review/exam/{$exam.id}">View</a>{/if}</td>
        <td>{if $exam.has_solutions}<a href="{$baseUrl}/admin/exambank/review/solutions/{$exam.id}">View</a>{/if}</td>
        <td>
          <input type="submit" name="approve_{$exam.id}" value="Approve" />
          <input type="submit" name="reject_{$exam.id}" value="Reject" />
          <input type="submit" name="update_{$exam.id}" value="Update" />
          <input type="submit" name="delete_{$exam.id}" value="Delete" />
        </td>
      </tr>
{/foreach}
    </tbody>
  </table>
  </form>
</div>
