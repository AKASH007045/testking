<?php
/**
 * @file
 */

module_load_include('inc', 'system', 'system.admin');

function custom_invoice_settings() {
  $validate =  array(      
    'file_validate_extensions' => array('png jpg jpeg gif'),
    // Pass the maximum file size in bytes
    'file_validate_size' => array(1024*1024*2),
  );
  $form['box1'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('stp-legend-heading')),
  );
  $form['box1']['invoice_logo'] = array(
    '#type' => 'managed_file',
    '#title' => t('Invoice Logo'),
    '#description' => t('Please enter subject for invoice mail.'),
    '#default_value' => variable_get('invoice_logo_fid', ''),
    //'#required' => TRUE,
    '#upload_location' => 'public://invoice/'
  );
  $form['box2'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('stp-legend-heading')),
  );
  $form['box2']['invoice_team_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Invoice Teamname'),
    '#description' => t('Please enter Team name for invoice.'),
    '#default_value' => variable_get('invoice_team_name', t('STP')),
    '#required' => TRUE,
  );
  $form['box3'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('stp-legend-heading')),
  );
  $form['box3']['invoice_watermark'] = array(
    '#type' => 'managed_file',
    '#title' => t('Invoice Watermark'),
    '#description' => t('Please uplaod watermark for invoice.'),
    '#default_value' => variable_get('invoice_watermark_fid', ''),
   // '#required' => TRUE,
     '#upload_validators' => $validate,
    '#upload_location' => 'public://invoice/',
    '#file_upload_description' => theme('file_upload_help', array('upload_validators' => $validate)),
   // '#theme' => array('image_widget'),
  );
  $form['box4'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('stp-legend-heading')),
  );
  $form['box4']['invoice_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Invoice Colors - Lines and Headers'),
    '#description' => t('Please enter HEX color code for lines and headers.'),
    '#default_value' => variable_get('invoice_color', t('#000000')),
    '#required' => TRUE,
    
  );
  $form['box5'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('stp-legend-heading')),
  );
  $footer =  variable_get('invoice_footer', array());
  $form['box5']['invoice_footer'] = array(
      '#type' => 'text_format',
      '#title' => t('Invoice Footer - Team Info'),
      '#rows' => 40,
      '#description' => t('Please enter Invoice footer - team info.'),
      '#default_value' => isset($footer['value'])?$footer['value'] : '',
      '#required' => TRUE,
      '#format' => 'full_html',
  );
  
  // Perform our custom submit before system submit
  $form['#submit'][] = 'custom_settings_form_submit';
  return system_settings_form($form);
}

function custom_settings_form_submit($form, &$form_state) {
 // global $user;
  // Load the file via file.fid.
  $file = file_load($form_state['values']['invoice_logo']);
  if ($file) {
    // Change status to permanent.
    $file->status = FILE_STATUS_PERMANENT;
    // Save.
    file_save($file);
    // Save file to vatiable
    variable_set('invoice_logo_fid', $file->fid);
    // Record that the module (in this example, user module) is using the file. 
   // file_usage_add($file, 'user', 'user', $user->uid);
    // Unset formstate value
    unset($form_state['values']['invoice_logo']); // make sure it is unset for system submit
    file_usage_add($file, 'file', 'file', $file->fid);
  }
   $file = file_load($form_state['values']['invoice_watermark']);
  if ($file) {
    // Change status to permanent.
    $file->status = FILE_STATUS_PERMANENT;
    // Save.
    file_save($file);
    // Save file to vatiable
    variable_set('invoice_watermark_fid', $file->fid);
    // Record that the module (in this example, user module) is using the file. 
   // file_usage_add($file, 'user', 'user', $user->uid);
    // Unset formstate value
    file_usage_add($file, 'file', 'file', $file->fid);
    unset($form_state['values']['invoice_watermark']); // make sure it is unset for system submit
  }
}