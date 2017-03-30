<?php 

/**
 * @file
 * Default theme implementation to format an HTML mail.
 *
 * Copy this file in your default theme folder to create a custom themed mail.
 * Rename it to mimemail-message--[key].tpl.php to override it for a
 * specific mail.
 *
 * Available variables:
 * - $recipient: The recipient of the message
 * - $subject: The message subject
 * - $body: The message body
 * - $css: Internal style sheets
 * - $key: The message identifier
 *
 * @see template_preprocess_mimemail_message()
 */
//global $base_url;
//?>
<div class ="admin-preview-node">
  
  <img id="loader1" src="/sites/default/files/loader_big.gif" width="36" height="36" alt="loading gif" text-align="center" margin-left= "250px" />
  
  <iframe id ="iframe1" src="/node/<?php print $node->nid;?>" style="     min-height: 335px;
    width: 100%;
    padding: 0;
    margin: 0;
    max-height: 600px;"></iframe>
    <span class = "popup-btn-close">X</span>
</div>

<script>
    jQuery(document).ready(function () {
        jQuery('#iframe1').on('load', function () {
            jQuery('#loader1').hide();
        });
    });
</script>