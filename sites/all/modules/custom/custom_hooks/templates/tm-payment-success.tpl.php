<script>
  _gaq.push(['_trackEvent', 'Payment Popup', 'Validation Passed', <?php print $acct_id;?>, 1, true]);
</script>
    <div class="popUpPanel clearfix payment-thankyou-wrapper">
            <div class="popUpPanelPanel clearfix">
                <div class="popUpDiv name">
                    <div class="user-name"><?php print $name;?></div>
                    <div class="accountDetail">Account # <span class="blueTxt"><?php print $acct_id;?></span></div>
                </div>
                <!-- Normal Messgae Start Here -->
                <div class="popUpDiv message"><!--error-->
                    <div class="msg">THANK YOU,</div>
                    <div class="detail">YOUR TRANSACTION IS COMPLETE</div>
                </div>
                <!-- Normal Messgae Ends Here -->
                
                <div class="popUpDiv text">
                    <!-- Normal Messgae Starts Here -->
                    <div>Your account manager will reflect payment within 24 hours.  We are excited for another season in AT&T Stadium and appreciate your support as a loyal Sacramento Kings season ticket holder.  You will receive your tickets through UPS before the first preseason game. Please contact your service coordinator if you have any questions.</div>
                    <!-- Normal Messgae Ends Here -->
                    
                </div>
                <div class="centered">
                    <a class="btn btn-lg btn-blue" href="/">Back to home<span></span></a>
                </div>
            </div>
            <div class="popUpPanelFooter"></div>
        </div>