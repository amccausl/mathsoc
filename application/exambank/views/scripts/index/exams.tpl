{* This script renders search results from an exambank query. *}
{assign var='layout' value='basic'}
{assign var='stylesheets' value='/css/exam-table.css'}
{foreach from=$exams item=exam name=loop}
  {if $smarty.foreach.loop.first}
<table>
  <!-- <caption>MathSoc ExamBank Query</caption> -->
  <thead><tr>
    <th scope='col'>Course</th>
    <th scope='col'>Term</th>
    <th scope='col'>Type</th>
    <th scope='col'>Exam</th>
    <th scope='col'>Solutions</th>
  </tr></thead>
  <tbody>
  {/if}
  {if $smarty.foreach.loop.index % 2 == 0}
    <tr>
  {else}
    <tr class="odd">
  {/if}
      <th scope='row'>{$exam.course}</th>
      <td>{$exam.term}</td>
      <td>{$exam.type}</td>
  {if $exam.exam_path}
      <td><a href='{$baseUrl}/exambank/exams/{$exam.prefix}/{$exam.code}/{$exam.term}/{$exam.id}/exam' target='_blank' title='Download the {$exam.term} exam'>Download</a></td>
  {else}
      <td></td>
  {/if}
  {if $exam.solutions_path}
      <td><a href='{$baseUrl}/exambank/exams/{$exam.prefix}/{$exam.code}/{$exam.term}/{$exam.id}/solutions' target='_blank' title='Download the {$exam.term} exam solution'>Download</a></td>
  {else}
      <td></td>
  {/if}
    </tr>
  {if $smarty.foreach.loop.last}
  </tbody>
</table>
  {/if}
{foreachelse}
There are currently no exams for this course.
{/foreach}
