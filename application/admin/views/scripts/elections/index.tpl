<div class="section" style="width: 700px">
  <p><a href="{$baseUrl}/admin/elections/declare">Declare an election</a></p>
  <h2>Current Elections</h2>
  <form action="{$smarty.server.REQUEST_URI}" method="post">
  <table>
    <thead><tr>
      <th scope="col">Position</th>
      <th scope="col">Term</th>
      <th scope="col">Nominations</th>
      <th scope="col">Elections</th>
    </tr></thead>
    <tbody>
{foreach from=$elections key=id item=election name=loop}
      <tr>
        <th scope="row">{$election.position} ({$election.position_needed})</th>
        <td>{term id=$term_start} to {term id=$term_end}</td>
        <td></td>
        <td>{if $smarty.now > $elections.polls_close}<input type="submit" name="tally_{$id}" value="Results"/>{else}{$elections.polls_close}{/if}</td>
      </tr>
{/foreach}
  </table>
</div>
