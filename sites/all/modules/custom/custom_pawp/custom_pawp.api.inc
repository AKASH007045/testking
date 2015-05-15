<?php
/**
 * @file
 */
/**
 * Pay as We Play multistep form
 */
 function tmapi_pawp_multistep_form($js = NULL, $step = NULL) {
 if ($js) {
    ctools_include('modal');
    ctools_include('ajax');
  }
  // Define array for ctools multistep wizard.
  $form_info = array(
    'id' => 'tmapi_pawp',
    'path' => "pawp/" . ($js ? 'ajax' : 'nojs') . "/form/%step",
    'show trail' => FALSE,
    'show back' => FALSE,
    'show cancel' => FALSE,
    'show return' => FALSE,
    'next callback' =>  'tmapi_pawp_wizard_next',
    'finish callback' => 'tmapi_pawp_wizard_finish',
    'cancel callback' => 'tmapi_pawp_wizard_cancel',

   // Define forms order.
    'order' => array(
      'start' => t('Pay As We Play'),
      'second' => t('Pay As We Play'),
    ),

   // Define forms
    'forms' => array(
      'start' => array(
        'form id' => 'custom_pawp_sign_up'
      ),
      'second' => array(
        'form id' => 'custom_pawp_tmapi_request_form'
      ),
    ),
  );

  // We're not using any real storage here, so we're going to set our
  // object_id to 1. When using wizard forms, id management turns
  // out to be one of the hardest parts. Editing an object with an id
  // is easy, but new objects don't usually have ids until somewhere
  // in creation.
  //
  // We skip all this here by just using an id of 1.
  $object_id = 1;

  if (empty($step)) {
    // We reset the form when $step is NULL because that means they have
    // for whatever reason started over.
    tmapi_pawp_cache_clear($object_id);
    $step = 'start';
  }

  // This automatically gets defaults if there wasn't anything saved.
  $object = tmapi_pawp_cache_get($object_id);

  // live $form_state changes.
  $form_state = array(
    'ajax' => $js,

    // Put our object and ID into the form state cache so we can easily find it.
    'object_id' => $object_id,
    'object' => &$object,
  );

  // Send this all off to our form. This is like drupal_get_form only wizardy.
  ctools_include('wizard');
  $form = ctools_wizard_multistep_form($form_info, $step, $form_state);
  $output = drupal_render($form);
  if ($js) {
    // If javascript is active, we have to use a render array.
    $commands = array();
    if ($output === FALSE || !empty($form_state['complete'])) {
      // Dismiss the modal.
      $commands[] = ajax_command_html('#ctools-sample', render(tnapi_pawp_get_result($object)));
      $commands[] = ctools_modal_command_dismiss();
    }
    elseif (!empty($form_state['cancel'])) {
      // If cancelling, return to the activity.
      $commands[] = ctools_modal_command_dismiss();
    }
    else {
      $commands = ctools_modal_form_render($form_state, $output);
    }
    print ajax_render($commands);
  }
  else {
    if ($output === FALSE || !empty($form_state['complete'])) {
      return render(tmapi_pawp_get_result($object)) . "\n\r" . l(t('Back'), 'example-link');
    }
    elseif (!empty($form_state['cancel'])) {
      drupal_goto('pawp');
    }
    else {
      return $output;
    }
  }
}

/**
 * Pay As We Play First Step -Sign up confirmation
 *
 */
function custom_pawp_sign_up($form, &$form_state) {
  global $user;
  if (isset($_SESSION['pawp_confirmation']) && $_SESSION['pawp_confirmation'] =='NO') {
    drupal_goto('<front>');
  }
  if ($user->uid) {
    $acct_id = $user->name;
  }
  $pawp_top_content = variable_get('paswp_signup_confirmation_body', '');
  $pawp_bottom_content = variable_get('paswp_signup_confirmation_body_bottom', '');
  $form['buttons']['next']['#value'] = 'Sign Up';
  $form['buttons']['#weight'] = '-9';
  $form['acct_id'] = array(
    '#type' => 'hidden',
    '#value' => $acct_id,
  );
  $form['pawp_signup_top'] = array(
    '#type' => 'markup',
    '#markup' => $pawp_top_content,
    '#weight' => '-10'
  );
  $form['nothanks'] = array(
    '#type' => 'submit',
    '#value' => t('No Thanks'),
    '#weight' => '-8'
  );
  $form['pawp_signup_bottom'] = array(
    '#type' => 'markup',
    '#markup' =>  $pawp_bottom_content,
    '#weight' => '0'
  );
  return $form;
}

/**
 * Sign up confirmation form submit handler.
 */
function custom_pawp_sign_up_submit($form, &$form_state) {
  if ($form_state['values']['op'] == 'No Thanks') {
    $_SESSION['pawp_confirmation'] = 'NO';
    module_load_include('inc', 'tm_api', 'tm_api.helper');
    global $user;
    $data = $form_state['values'];
    $response = array();
    $check_pawp_submission = custom_pawp_check_cc_status(-1);
    if (!$check_pawp_submission) {
      watchdog('tmapi_PAWP', 'TM API request (No Thanks) for ACCT_ID. %data', array('%data' => $user->name), WATCHDOG_NOTICE, NULL);
      $tm_response = _tmapi_customer_update_call($data['acct_id'], NULL, '', 'NO');
      if (!$tm_response) {
        watchdog('tmapi_PAWP', 'TM PAWP cc_update Failure (NO Thanks) for ACCT_ID. %data', array('%data' => $user->name), WATCHDOG_NOTICE, NULL);
      }
    }
    drupal_goto('<front>');
  }
  $form_state['object']->acct_id = $form_state['values']['acct_id'];
}

/**
 * Pay As We Play Step2 - Credit card Form
 */
function custom_pawp_tmapi_request_form($form, &$form_state) {
  if (isset($_SESSION['pawp_confirmation']) && $_SESSION['pawp_confirmation'] == 'NO') {
    drupal_goto('<front>');
  }
  $check_pawp_submission = custom_pawp_check_cc_status(1);
  if ($check_pawp_submission) {
    drupal_goto('pawp/error');
  }
  $form['buttons']['return']['#value'] = 'Sign Up';
  $form['buttons']['#weight'] = '9';
  $form['buttons']['return']['#suffix'] = '</div>';
  $form['#attributes']['autocomplete'] = 'off';
  $pawp_content = variable_get('paswp_cc_body', '');
  $form['#attributes']['autocomplete'] = 'off';
  $form['pawp_signup_top'] = array(
    '#type' => 'markup',
    '#prefix' => '<div class ="pawp-request-form">',
    '#markup' => '<div class="pawp-cc-information-text">Enter your preferred credit card information to opt into Kings Pay As We Play playoff plan.</div>',
    '#weight' => '-10'
  );
  $form['ctype'] = array(
    '#type' => 'select',
    '#title' => t('Card Type'),
    '#options' => array('VI' => 'Visa', 'MC' => 'MasterCard', 'AE' => 'American Express', 'DI' => 'Discover'),
    '#required' => TRUE,
    '#attributes' => array('tabindex' => '1')
  );
  $form['cc_num'] = array(
    '#type' => 'textfield',
    '#title' => t('Card Number '),
    '#required' => TRUE,
    '#maxlength' => 16,
    '#element_validate' => array('element_validate_integer_positive'), 
    '#attributes' => array('tabindex' => '2')
  );
  $form['exp_date'] = array(
    '#type' => 'textfield',
    '#title' => t('Expiration Date'),
    '#maxlength' => 4,
    '#element_validate' => array('element_validate_integer_positive'),
    '#required' => TRUE,
    '#attributes' => array('tabindex' => '3', 'placeholder' => 'mmyy')
  );
  $form['email_confirmation'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="conf-email">' . t('Enter your email to receive confirmation') . '</div>',
  );
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email:'),
    '#required' => TRUE,
    '#maxlength' => 100,
    '#attributes' => array('tabindex' => '12')
  );
  
  $form['acct_info'] = array(
    '#type' => 'markup',
    '#markup' => $pawp_content,
    '#weight' => 10,
  );
 
  return $form;
}

/**
 * Pay As We Play Second Step - Credit card Form Validations
 */
function custom_pawp_tmapi_request_form_validate(&$form, &$form_state) {
  $ctype = array('VI' => 4, 'MC' => 5, 'AE' => 3, 'DI' => 6);
  $clength = array('VI' => 16, 'MC' => 16, 'AE' => 15, 'DI' => 16);
  $data = $form_state['values'];
  $type = (isset($data['ctype']) && $data['ctype']) ? $data['ctype'] : '';
  $exp_valid_year = date('y');
  $exp_valid_month = date('m');
  $data_exp_year = substr($data['exp_date'], -2, 2);
  $data_expr_month = substr($data['exp_date'], 0, 2);
  $month_check = ($exp_valid_month <= $data_expr_month) ? 'true' : 'false';
  // Credit card number validation
  if ((isset($data['cc_num']) && $data['cc_num']) && $type) {
    if ((strlen($data['cc_num']) != $clength[$type]) || is_float($data['cc_num']) || (strlen((int)($data['cc_num'])) < $clength[$type])) {
      form_set_error('cc_num', 'Invalid Card Number');
    }
    elseif ($data['cc_num'] && (substr($data['cc_num'], 0, 1) != $ctype[$type])) {
      form_set_error('cc_num', 'Invalid Card Number');
    }
  }
  // Email validation
  if (!valid_email_address($data['email'])) {
    form_set_error('email', t('The e-mail address %mail is not valid.', array('%mail' => $data['email'])));
  }
  // Expiry Date validation
  if (isset($data['exp_date']) && $data['exp_date']) {
    if (strlen($data['exp_date']) < 4) {
      form_set_error('exp_date', 'Invalid Expiration Date.');
    }
    elseif (($data_exp_year < $exp_valid_year) || ($data_expr_month > 12) || ($data_expr_month < 1) || strstr($data['exp_date'], ' ')) {
      form_set_error('exp_date', 'Invalid Expiration Date.');
    }
    elseif (($data_exp_year == $exp_valid_year) && ($month_check == 'false')) {
      form_set_error('exp_date', 'Invalid Expiration Date.');
    }
    elseif (($data_exp_year < 15) || (($data_exp_year < 15) && ($data_expr_month < 1))) {
      form_set_error('exp_date', 'Cards expiring before 01/15 will not be accepted.');
    }
  }
}

/**
 * Pay As We Play Second Step - Credit card Form Submit Handler
 */
function custom_pawp_tmapi_request_form_submit(&$form, &$form_state) {
  module_load_include('inc', 'tm_api', 'tm_api.helper');
  global $user;
  $acct_id = $form_state['object']->acct_id ? $form_state['object']->acct_id : $user->name;
  $site_mail = variable_get('site_mail', ini_get('sendmail_from'));
  $data = $form_state['values'];
  $cc_response = 0;
  $response = array();
  watchdog('tmapi_PAWP', 'PAWP TM API request for ACCT_ID. %data', array('%data' => $data['tmuser_info']['acct_id']), WATCHDOG_NOTICE, NULL);
  $form_state['object']->result = 0;
  $cc_response = _tmapi_cc_update_call($acct_id, $data);
  if ($cc_response) {
    $tm_response = _tmapi_customer_update_call($acct_id, $data, $cc_response);
  }
  $cc_num = '************'. substr($data['cc_num'], -4, 4);
  $params['subject'] = variable_get('pawp_mail_subject', t('Sacramento Kings'));
  $data['tm_raw_response'] = '';
 if ($tm_response) {
      watchdog('tmapi_PAWP', 'TM PAWP Success for ACCT_ID. %data', array('%data' => $data['tmuser_info']['acct_id']), WATCHDOG_NOTICE, NULL);
      $msg = theme('tm_pawp_success', array('name' => $data['tmuser_info']['name'], 'acct_id' => $data['tmuser_info']['acct_id'], 'cc' => $cc_num));
      $params['body'] = t(variable_get('pawp_mail_body', 'Thank you for signing up for Pay As We Play.'), array('%cc_num' => $cc_num));
      drupal_mail('custom_hooks', 'tm_payment_notify', $data['email'], LANGUAGE_NONE, $params, $site_mail);
      $form_state['object']->result = 1;
  }
  else {
    $msg = theme('tm_pawp_error', array('name' => $data['tmuser_info']['name'], 'acct_id' => $data['tmuser_info']['acct_id'], 'cc' => $cc_num));
    $form_state['object']->result = 0;
    watchdog('tmapi_PAWP', 'TM PAWP Failure for ACCT_ID. %data', array('%data' => $data['tmuser_info']['acct_id']), WATCHDOG_NOTICE, NULL);
  }
}


/**
 * Clears the wizard cache.
 *
 * @param integer $id
 *   cache id.
 */
function tmapi_pawp_cache_clear($id) {
  ctools_include('object-cache');
  ctools_object_cache_clear('tmapi_pawp', $id);
}

/**
 * Stores our little cache so that we can retain data from form to form.
 *
 * @param integer $id
 *   cache id.
 * @param object $object
 *   object with form values.
 */
function tmapi_pawp_cache_set($id, $object) {
  ctools_include('object-cache');
  ctools_object_cache_set('tmapi_pawp', $id, $object);
}

/**
 * Gets the current object from the cache, or default.
 *
 * @param integer $id
 *   cache id.
 *
 * @return object
 *   cache with stored stuff.
 */
function tmapi_pawp_cache_get($id) {
  ctools_include('object-cache');
  $object = ctools_object_cache_get('tmapi_pawp', $id);
  if (!$object) {
    // Create a default object.
    $object = new stdClass;
  }
  return $object;
}

/**
 * Handles the 'next' click on the add/edit pane form wizard.
 *
 * All we need to do is store the updated pane in the cache.
 */
function tmapi_pawp_wizard_next(&$form_state) {
  tmapi_pawp_cache_set($form_state['object_id'], $form_state['object']);
}

/**
 * Handles the 'finish' click on teh add/edit pane form wizard.
 *
 * All we need to do is set a flag so the return can handle adding
 * the pane.
 */
function tmapi_pawp_wizard_finish(&$form_state) {
  $form_state['complete'] = TRUE;
}

/**
 * Handles the 'cancel' click on the add/edit pane form wizard.
 */
function tmapi_pawp_wizard_cancel(&$form_state) {
  $form_state['cancel'] = TRUE;
}

/**
 * Returns form results.
 *
 * @param object $object
 *   object with form values.
 *   
 * @return theme
 *   returns renderable theme for multistep form result output.
 */
function tmapi_pawp_get_result($object) {
  $acct_id = isset($object->acct_id) ? $object->acct_id : 0;
  if (!$acct_id) {
    global $user;
    $acct_id = $user->name;
  }
  if ($object->result == 1) {
    return theme('tm_pawp_success', array('acct_id' => $acct_id));
  }
  return theme('tm_pawp_error', array('acct_id' => $acct_id));
}


function tmapi_pawp_error_page() {
  global $user;
  return theme('tm_pawp_page_error', array('acct_id' => $user->name));
}

function tmapi_pawp_rsvp_success() {
  drupal_set_title(variable_get('rsvp_title', ''));
  return theme('rsvp_success');
}
function tmapi_pawp_rsvp_error() {
  drupal_set_title(variable_get('rsvp_title', ''));
  return theme('rsvp_error');
}