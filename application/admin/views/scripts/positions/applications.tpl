<div id="main-header"><h1></h1></div>
<div class="section" style="width: 700px">
  <form action="{$smarty.server.REQUEST_URI}" method="post">
{if $application}
  <h2>{$application.name}</h2>
  <p>Application for {$application.position} for the {term id=$application.term} term.  Application was received at {$application.applied}.
  <dl>
{foreach from=$application.questions key=question item=answer}
    <dt>{$question}</dt>
	<dd>{$answer}</dd><br/>
{/foreach}
  </dl>
{/if}
  <h2>Current Applications</h2>
  <table width="700">
    <thead><tr>
      <th scope="col">Position</th>
      <th scope="col">Term</th> 
      <th scope="col">User</th>
      <th scope="col">Actions</th>
    </tr></thead>
    <tbody>
{foreach from=$applications item=application}
      <tr>
        <th scope="row"{if $application.id == $id} class="active"{/if}>{$application.position}</th>
        <td>{term id=$application.term}</td>
        <td>{$application.user}</td>
        <td>
          <input type="submit" name="view_{$application.id}" value="View Application" />
          <input type="submit" name="select_{$application.id}" value="Select For Position" />
          <input type="submit" name="reject_{$application.id}" value="Reject Application" />
        </td>
      </tr>
{/foreach}
    </tbody>
  </table>
  </form>
</div>
