<script>
  _gaq.push(['_trackEvent', 'Payment Popup', 'Validation Passed', <?php print $acct_id;?>, 1, true]);
</script>
<?php print $user_info; ?>
    <article class="col-sm-12 payment-thankyou-wrapper">
      <h2><?php print $stp_invoice_message;?></h2>
      <a class="btn btn-default return-home" href="/">Return to Home <i class="fa fa-chevron-right"></i></a>
    </article>
<div class="col-sm-12 thankyou-footer">&nbsp;</div>