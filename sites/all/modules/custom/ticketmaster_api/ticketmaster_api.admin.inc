<?php

// @file

function ticketmaster_api_settings_form() {
  $form = array();
  $apis = ticketmaster_get_apis();
  $options = array(
    '' => t('-- Select a api'),
  );
  if (count($apis)) {
    foreach ($apis as $service => $info) {
      $options[$service] = $info['class'];
    }
  }
  $form['tm_api'] = array(
    '#title' => t('Ticketmaster Data Api'),
    '#type' => 'select',
    '#description' => t('Please select ticketmaster api to show data'),
    '#options' => $options,
    '#required' => TRUE,
    '#default_value' => variable_get('tm_api', 'tm_data'),
  );
  return system_settings_form($form);
}