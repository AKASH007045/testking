<script>
  _gaq.push(['_trackEvent', 'Payment', 'Validation Failed', <?php print $acct_id;?>, 1, true]);
</script>
<?php
/**
* $message body of block
* $type block no(1,2,3)
**/
print $user_info;
?>
    <article class="col-sm-12 payment-unsuccessful">

          <h1>AN ERROR OCCURRED</h1>
         <h2>PLEASE GO BACK AND TRY AGAIN</h2>
         
        <a class="btn btn-default return-home" href="/invoice/payment">Go Back <i class="fa fa-chevron-right"></i></a>
          
       </article>
        <div class="col-sm-12 thankyou-footer">&nbsp;</div>
        
