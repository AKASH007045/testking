<?php
/**
 * @file
 */

/**
* function to send invoice email to users
*/
function custom_hooks_email_invoice() {
  global $user;
  $acct_id = $user->name;
  $name = get_user_full_name($user);
  if (isset($acct_id)) {
    $seat_holds = get_seats_hold($acct_id);
    $playoff_data = get_playoff_data($acct_id);
    if (!count($seat_holds) && !count($playoff_data)) {
      drupal_set_message(t('Invoice data not avaiable'), 'warning');
      drupal_goto('invoice');
    }
  }
  module_load_include('inc', 'custom_block', 'custom_block.block'); 
  $output = custom_block_invoice_page('pdf');

  $params = array(
    'name' => $user->name,
    'subject' => variable_get('invoice_mail_subject', t('2015 Sacramento Kings Invoice')),
    'body'=> variable_get('invoice_mail_body', t('Attached to this email is a copy of your invoice for the 2015 Sacramento Kings Season Subscription.')),
  );

  // Checking mPDF library existence.
  if (pdf_using_mpdf_library_exist() == TRUE) {
    $filename = "Kings-Invoice_" . str_replace(' ', '-', $name);
    _pdf_using_mpdf_generator($output, $filename, 'F', $name);
    $file=file_directory_temp() . '/' . $filename.".pdf";
    $params['file'] = $file;
    $params['filename'] = $filename . '.pdf';
  }

  $mail= $user->mail;
  drupal_mail('custom_hooks', 'triggerkey', $mail, language_default(), $params);
  drupal_set_message('Invoice mailed successfully.');
  drupal_goto('invoice');
  return '';
}

function custom_hooks_contact_us_access(){
  // module_load_include('inc', 'tm_data', 'tm_data.inc');
  global $user;
   //print_r($user);
    if(in_array('R_19', $user->roles)){      
        drupal_goto('contact-us'); 
    }
    else if(in_array('NOT_R_19', $user->roles)){      
        drupal_goto('contact-us'); 
    }
   else{
       return false; 
   }          
}
/**
* function to print invoice
*/
function custom_hooks_print_invoice() {
  global $user, $conf;
  $acct_id = $user->name;
  $name = get_user_full_name($user);
  if (isset($acct_id)) {
    $seat_holds = get_seats_hold($acct_id);
    $playoff_data = get_playoff_data($acct_id);
    if (!count($seat_holds) && !count($playoff_data)) {
      drupal_set_message(t('Invoice data not avaiable'), 'warning');
      drupal_goto('invoice');
    }
  }
  module_load_include('inc', 'custom_block', 'custom_block.block'); 
  $output = custom_block_invoice_page('pdf');
  // Checking mPDF library existence.
 
  if (pdf_using_mpdf_library_exist() == TRUE) {
    $filename = "kings-Invoice_" . str_replace(' ', '-', $name);
    $conf['pdf_using_mpdf_pdf_save_option'] = 0;
    _pdf_using_mpdf_generator($output, $filename, 'I', $name);
    exit;
  }
  return '';
}

function custom_hooks_invoice_page() {
  return '';
}

function custom_hooks_clear_custom_cache() {
  cache_clear_all('*', 'cache_custom', TRUE);
  watchdog('custom_hooks', 'All cache cleared after import.', array(), WATCHDOG_NOTICE, NULL);
  exit();
}

function custom_hooks_forgot_password() {
  global $user;
  $password = variable_get('forgot_link', 'https://oss.ticketmaster.com/html/home.htmI?team=cowboys&l=EN&STAGE=1&CNTX=120962&out=1');
  $form = array();
  if ($user->uid) {
    $msg = '<div class="user-name">ACCESS DENIED</div><p style="height:125px;">SORRY, YOU ARE NOT AUTHORIZED TO ACCESS THE PAGE YOU ARE LOOKING FOR.</p>';
  }
  else {
    $msg = theme('forgot_password', array('password' => $password));
  }
  $form['acct_info'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="popUpDiv name step1">
      <div class="accountDetail"><p>'. $msg .'</p></div>
      </div>',
    '#weight' => -10,
  );
  return $form;
}

function custom_hooks_parking_promo() {
  global $user;
  $content = variable_get('tm_park_purchase_body', '');
  $form = array();
  $form['acct_info'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="popUpDiv name step1 parking-promo-wrapper"><div class="popUpPanelPanel clearfix">
      ' . $content . '
      </div></div>',
  );
  unset($form['close_link']);
  return $form;
}

function custom_hooks_birthday_celebration() {
  global $user;
  $birthday_takeover_access = tm_user_birthday_access(1);
  $birthday = variable_get('en_birthday', 0);
  if ($birthday_takeover_access && $birthday) {
    $name = get_user_full_name($user);
    return theme('birthday_celebration', array('name' => $name));
  }
  drupal_goto('<front>');
  drupal_exit();
}

/**
 * Ajaxified Node callback.
 */
function ajaxified_node($node) {
  if ($node) {
    if($node->type == 'event') {
       $node_view = node_view($node, 'event_node');
       $rendered_node = drupal_render($node_view);
      $json['data'] = $rendered_node;
      $json['title'] = $node->title;
      $json['body'] = $node->body[$node->language][0]['value'];
      exit(drupal_json_encode($json));
    }
    else {
      $json['title'] = $node->title;
      $json['body'] = $node->body[$node->language][0]['value'];
      exit(drupal_json_encode($json));
    }
  }
  else {
    return MENU_NOT_FOUND;
  }
}
