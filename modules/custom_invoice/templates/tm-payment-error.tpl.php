<script>
  _gaq.push(['_trackEvent', 'Payment', 'Validation Failed', <?php print $acct_id;?>, 1, true]);
</script>
<?php
/**
* $message body of block
* $type block no(1,2,3)
**/
print $user_info;
$message = $_SESSION['failure_message'];
unset($_SESSION['failure_message']);
?>
<article class="col-sm-12 payment-unsuccessful">
	<h1>AN ERROR OCCURRED</h1>
	<p><?php print isset($message) ? $message : 'The transaction was declined. Please use a different card or contact your bank.';?></p>
	<p><a class="btn btn-default return-home" href="/invoice/<?php print arg(1)?>/payment/<?php print arg(3)?>"><i class="fa fa-chevron-left"></i> TRY AGAIN</a></p>
</article>
<div class="col-sm-12 thankyou-footer">&nbsp;</div>
