<div id='main-header'><h1></h1></div>
<div class="section">
  <h2>Locker Usage Information</h2>
  <p>Currently {$used_lockers} of {$total_lockers} are in use.
  {$expiring_lockers} lockers are expiring on {$expiring}.  {$expired_lockers} are already expired.
  </p>
{if !$isActive}
  <p>Today is the expiry date, which means it's lock cutting time.  <a href="{$baseUrl}/admin/lockers/empty">Go Here</a> to expire the lockers and get a list of "empty" lockers.
{/if}
</div>
