<script>
  _gaq.push(['_trackEvent', 'Pay As We Play', 'Validation Failed', <?php print $acct_id;?>, 1, true]);
</script>
<div id="user-login" class="pawp-error">
  <h1>AN ERROR OCCURRED</h1>
  <p>PLEASE GO BACK AND TRY AGAIN</p>
  <div id="edit-actions"><a href="/pawp/nojs/form" class="btn btn-default form-submit">Go Back</a></div>
  <div id="edit-form-description" class="form-type-item form-item form-group">
     Your transaction was not completed, please go back<br>and verify all your information.
    </div>
</div>
<?php print variable_get('paswp_cc_body', ''); ?>
