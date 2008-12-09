<div class="section">
  electionid = {$election.electionId}<br/>
  position = {$election.position}<br/>
  vote count = {$election.vote_count}<br/>
  winners =<br/>
  {foreach from=$winners item=candidate}
    {$candidate.name}<br/>
  {/foreach}
  details =<br/>
  <pre>
    {$election.details}
  </pre>
</div>
