<div class="section">
<!-- TODO: display outcome of election -->
  electionid = {$election.electionId}<br/>
  position = {$election.position}<br/>
  vote count = {$election.vote_count}<br/>
  winners =
  {foreach from=$winners item=candidate}
    {$candidate.name}<br/>
  {/foreach}
  details =<br/>
  <pre>
    {$election.details}
  </pre>
</div>
