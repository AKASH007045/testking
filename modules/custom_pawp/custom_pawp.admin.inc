<?php

/**
 * @FILE
 *
 */
module_load_include('inc', 'system', 'system.admin');

/* * *
 * General settings form
 */

function custom_pawp_form_setting() {
  $form = array();
  $form['pawp'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pay As We Play'),
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['pawp']['en_pawp'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Pay As We Play (For logged in users)?'),
    '#default_value' => variable_get('en_pawp', 1),
  );
  $form['pawp']['paswp_signup_confirmation_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Sign Up confirmation body (Top)'),
    '#default_value' => variable_get('paswp_signup_confirmation_body', ''),
    '#required' => TRUE,
  );
  $form['pawp']['paswp_signup_confirmation_body_bottom'] = array(
    '#type' => 'textarea',
    '#title' => t('Sign Up confirmation body (Bottom)'),
    '#default_value' => variable_get('paswp_signup_confirmation_body_bottom', ''),
    '#required' => TRUE,
  );
  $form['pawp']['paswp_cc_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Second Screen (Step 2):  Enter Credit Card Description'),
    '#default_value' => variable_get('paswp_cc_body', ''),
    '#required' => TRUE,
  );
  $form['pawp']['pawp_mail_subject'] = array(
    '#type' => 'textarea',
    '#title' => t('PAWP Mail Subject'),
    '#default_value' => variable_get('pawp_mail_subject', ''),
    '#required' => TRUE,
  );
  $form['pawp']['pawp_mail_body'] = array(
    '#type' => 'textarea',
    '#title' => t('PAWP Mail Body'),
    '#default_value' => variable_get('pawp_mail_body', ''),
    '#required' => TRUE,
  );
  return system_settings_form($form);
}