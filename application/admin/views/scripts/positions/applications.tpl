<div id="main-header"><h1></h1></div>
<div class="section">
  <form action="{$smarty.server.REQUEST_URI}" method="post">
{if $application}
  <h2></h2>
{/if}
  <h2>Current Position Applications</h2>
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
        <td>{$application.term}</td>
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
