<?php global $user;?>
<script>
  _gaq.push(['_trackEvent', 'RSVP', 'Error', <?php print $user->name;?>, 1, true]);
</script>
<div id="user-login" class="pawp-page-error">
  <h1>AN ERROR OCCURRED</h1>
  <div id="edit-actions"><a href="/" class="btn btn-default form-submit">Return to Home</a></div>
  <div id="edit-form-description" class="form-type-item form-item form-group">
    Either you have already submitted the "RSVP" form or you are not authorized to access RSVP. For more information, please contact your Dedicated Service Coordinator.
    </div>
</div>
