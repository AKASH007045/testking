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
    <?php if ($css): ?>
    <style type="text/css">
      <!--
      <?php print $css ?>
      -->
    </style>
    <?php endif; ?>
  </head>
  <body id="mimemail-body" <?php if ($key): print 'class="'. $key .'"'; endif; ?>>
    <table width="618" border="0" cellspacing="0" cellpadding="0"  style="border:1px solid #333300;font-family:Arial, Helvetica, sans-serif;font-size:12px;color:#666666;">
    <tr style="background-color:#ffffff;padding:7px 0 7px 45px;">
       <td style="border-bottom:2px solid #00295b;">
          <div style="padding:7px 0 7px 20px;">
            <a href="<?php print url('<front>');?>">
              <img src="/<?php print drupal_get_path('theme', 'sports')?>/images/subscribe-email-logo.png" height="59" alt="logo" style="border:none;" />
            </a>
          </div>
       </td>
     </tr>
     <tr>
       <td style="background-color:#00295b;line-height:3px;">
         &nbsp;
       </td>
     </tr>
      <tr>
         <td>
           <div style="padding:20px 10px 20px 20px;"">
             <?php print $body; ?>
           </div>
         </td>
       </tr>
   </table>
  </body>
</html>
