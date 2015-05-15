<?php
/**
* $message body of block
* $type block no(1,2,3)
**/
?>
    <div class="popUpPanel clearfix payment-thankyou-wrapper payment-unsuccessful">
            <div class="popUpPanelPanel clearfix">
                <div class="popUpDiv name">
                    <div class="user-name"><?php print $name;?></div>
                    <div class="accountDetail">Account # <span class="blueTxt"><?php print $acct_id;?></span></div>
                </div>
                <!-- Normal Messgae Start Here -->
                <div class="popUpDiv message error"><!--error-->
                    <div class="msg">AN ERROR OCCURRED</div>
                    <div class="detail"><span class="payment-failure-txt">PLEASE GO BACK AND TRY AGAIN</span> <div style="display:inline-block;"><a href="/tmapi-payment/nojs" class="btn btn-lg btn-red ctools-use-modal">Go back<span></span></a></div></div>
                </div>
                <!-- Normal Messgae Ends Here -->
                
                <div class="popUpDiv text">
                    <!-- Normal Messgae Starts Here -->
                    <div>Your transaction was not completed, please go back and verify all your information.</div>
                    <!-- Normal Messgae Ends Here -->
                    
                </div>
                <div class="centered">
                    
                </div>
            </div>
            <div class="popUpPanelFooter"></div>
        </div>
