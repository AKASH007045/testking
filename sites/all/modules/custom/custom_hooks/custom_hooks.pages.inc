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
    'subject' => variable_get('invoice_mail_subject', t('2014 Sacramento Kings Invoice')),
    'body'=> variable_get('invoice_mail_body', t('Attached to this email is a copy of your invoice for the 2014 Sacramento Kings Season Subscription.')),
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
/**
 * Forgot Password page
 *
 */
function custom_hooks_forgot_password_page($ajax) {
  if ($ajax) {
    ctools_include('ajax');
    ctools_include('modal');
    $form_state = array(
      'ajax' => TRUE,
      'title' => '  ',
    );
    // Use ctools to generate ajax instructions for the browser to create
    // a form in a modal popup.
    // If the form has been submitted, there may be additional instructions
    // such as dismissing the modal popup.
    $output = ctools_modal_form_wrapper('custom_hooks_forgot_password', $form_state);
    if (!empty($form_state['ajax_commands'])) {
      $output = $form_state['ajax_commands'];
    }

    // Return the ajax instructions to the browser via ajax_render().
    print ajax_render($output);
    drupal_exit();
  }
  else {
    return drupal_get_form('custom_hooks_forgot_password');
  }
}

function custom_hooks_forgot_password() {
  global $user;
  $password = variable_get('forgot_link', 'https://oss.ticketmaster.com/html/home.htmI?team=sackings&l=EN&STAGE=1');
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

/**
 * Parking Promo Page
 *
 */
function custom_hooks_parking_promo_page($ajax) {
  if ($ajax) {
    ctools_include('ajax');
    ctools_include('modal');
    $form_state = array(
      'ajax' => TRUE,
      'title' => '<img src="/sites/all/themes/sports/images/starSmall.png">',
    );
    // Use ctools to generate ajax instructions for the browser to create
    // a form in a modal popup.
    // If the form has been submitted, there may be additional instructions
    // such as dismissing the modal popup.
    $output = ctools_modal_form_wrapper('custom_hooks_parking_promo', $form_state);
    if (!empty($form_state['ajax_commands'])) {
      $output = $form_state['ajax_commands'];
    }
    // Return the ajax instructions to the browser via ajax_render().
    $_SESSION['Drupal_visitor_parking_promo'] = 1;
    print ajax_render($output);
    drupal_exit();
  }
  else {
    return '';
  }
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
