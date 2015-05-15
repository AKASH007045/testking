<?php
function custom_renew_setting($form, &$form_state){

  $date_val = variable_get('start_date');
  //print_r($date_val); exit;
  $form['renew'] = array(
    '#type' => 'fieldset',
    '#title' => t('renew Settings'),
    '#weight' => 3,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  /*$form['renew']['start_date_set'] = array(
    '#type' => 'fieldset',
    '#title' => t('renew Start Date'),
    '#weight' => 4,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['renew']['end_date_set'] = array(
    '#type' => 'fieldset',
    '#title' => t('renew End Date'),
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );*/
  $form['renew']['show_renew_popup'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show renew Pop Up'),
    '#default_value' => variable_get('show_renew_popup', 1)  
  );
  /*$form['renew']['start_date_set']['start_date'] = array(
    '#type' => 'date_popup', 
    '#title' => "",
    '#default_value' => variable_get('start_date', date('Y-m-d'))
  );
  $form['renew']['end_date_set']['end_date'] = array(
    '#type' => 'date_popup', 
    '#title' => "",
    '#default_value' => variable_get('end_date', date('Y-m-d'))
  );*/
  
  return system_settings_form($form);
}