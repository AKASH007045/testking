<?php
function custom_game_cron_mail_settings(){
  $form = array();
  
  /*$params['feed'] = 'My html Mail2';
      $lang = LANGUAGE_NONE;
      $to = variable_get('cron_recipients', ''); 
      drupal_mail('custom_cron_alert', 'start', $to, $lang, $params); exit;*/
  
  $cron_recipients = variable_get('cron_recipients', '');
  $cron_success_subject = variable_get('cron_success_subject', '');
  $cron_success_body = variable_get('cron_success_body', '');
  $cron_start_subject = variable_get('cron_start_subject', '');
  $cron_start_body = variable_get('cron_start_body', '');
    
  $form['cron_mail_settings_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cron Job Mail Settings Form')
  );
  
  $form['cron_mail_settings_fieldset']['cron_recipients'] = array(
    '#type' => 'textfield',
    '#title' => t('Cron Job Alert Recipient'),
    '#default_value' => $cron_recipients,
    '#size' => 250,
    '#maxlength' => 300,
    '#required' => TRUE
  );

  $form['cron_mail_settings_fieldset']['cron_start_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject - Cron Job Start'),
    '#default_value' => $cron_start_subject,
    '#size' => 250,
    '#maxlength' => 300,
    '#required' => TRUE
  );

  $form['cron_mail_settings_fieldset']['cron_start_body'] = array(
	'#title' => t('Body - Cron Job Start'),
	'#type' => 'text_format',
	'#description' => t ('Alert email notification message body'),
	'#default_value' => isset($cron_start_body['value'])?$cron_start_body['value']:"",
	'#format' => 'full_html',
	'#required' => TRUE,
	
  );
  
    $form['cron_mail_settings_fieldset']['cron_success_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject - Cron Job Success'),
    '#default_value' => $cron_success_subject,
    '#size' => 250,
    '#maxlength' => 300,
    '#required' => TRUE
  );

  $form['cron_mail_settings_fieldset']['cron_success_body'] = array(
	'#title' => t('Body - Cron Job Success'),
	'#type' => 'text_format',
	'#description' => t ('Alert email notification message body'),
	'#default_value' => isset($cron_success_body['value']) ? $cron_success_body['value'] : '',
	'#format' => 'full_html',
	'#required' => TRUE
  );
   
  
  return system_settings_form($form);
}

/*function custom_cron_mail_settings_validate($form, &$form_state) {
  $emails = explode(',', $form_state['values']['cron_recipients']);
  $invalid_emails = array();
  foreach($emails as $email){
	if(!valid_email_address($email)){
	  $invalid_emails[] = $email;
	}
  }
  
  $cnt = count($invalid_emails);
  $wrong_emails = implode(', ', $invalid_emails);
  
  if($cnt==1){
	$message = " Email is invalid";
	form_set_error('cron_recipients', $message . ' ' . $wrong_emails);
  }
  else if($cnt>0){
    $message = " Emails are invalid";
    form_set_error('cron_recipients', $message . ' ' . $wrong_emails);
  }
}*/