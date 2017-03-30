

<?php // if(isset($node) && $node != ''){
//  print_r($node); 
//print "Mail Title: ". $node->title . "<br/>"; 
//print "Mail Body: " . $node->body['und'][0]['value']. "<br/>";
//print "Mail Subject: " . $node->field_subject['und'][0]['value']. "<br/>";
//print "Template Type: ". $node->field_template_type['und'][0]['value']. "<br/>";
//
//} else {
//  print "No Preview Found !";
//}
?>
<!--<span class = "popup-btn-close">close</span>-->



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
global $base_url;
?>
<html>
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  </head>
  <body id="mimemail-body" <?php if ($key): print 'class="'. $key .'"'; endif; ?>>
    <?php print $node->body['und'][0]['value']; ?>
    <span class = "popup-btn-close">close</span>
  </body>
</html>
