<?php

/**
 * @file
 * Admin settings for amgr sso.
 */

/**
 * FAPI callback for the admin settings form.
 */
function amgr_sso_configs() {
  $form = array();
  $form['amgr_sso_active'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable AMGR SSO on site?'),
    '#default_value' => variable_get('amgr_sso_active', 0),
  );
  $form['amgr_site_url'] = array(
    '#type' => 'textfield',
    '#title' => t('AMGR Site Url'),
    '#default_value' => variable_get('amgr_site_url', 'https://am.ticketmaster.com/'),
  );
  $form['classic_amgr_site_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Classic AMGR Url'),
    '#default_value' => variable_get('classic_amgr_site_url', 'https://oss.ticketmaster.com'),
  );
  $form['amgr_base_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Base URL for AMGR Rest APIs'),
    '#default_value' => variable_get('amgr_base_url', ''),
  );
  //oAuth API Setting
  $form['oauth'] = array(
    '#type' => 'details',
    '#title' => t('oAuth API setting'),
    '#open' => true,
  );
  $form['oauth']['amgr_oauth_token_url'] = array(
    '#title' => t('oAuth Token URL'),
    '#type' => 'textfield',
    '#default_value' => variable_get('amgr_oauth_token_url', ''),
  );
  $form['oauth']['amgr_member_id_url'] = array(
    '#title' => t('Member ID URL'),
    '#type' => 'textfield',
    '#default_value' => variable_get('amgr_member_id_url', ''),
    '#description' => t('Provide absolute API URL. Use @access_token as token for access token replacement.'),
  );
  $form['oauth']['amgr_member_details_url'] = array(
    '#title' => t('Member Details URL'),
    '#type' => 'textfield',
    '#default_value' => variable_get('amgr_member_details_url', ''),
  );
  $form['oauth']['amgr_token_refresh_url'] = array(
    '#title' => t('Access Token Refresh URL'),
    '#type' => 'textfield',
    '#default_value' => variable_get('amgr_token_refresh_url', ''),
  );
  $form['oauth']['amgr_accept'] = array(
    '#title' => t('Accept'),
    '#type' => 'textfield',
    '#default_value' => variable_get('amgr_accept', ''),
  );
  $form['oauth']['amgr_content_type'] = array(
    '#title' => t('Content Type'),
    '#type' => 'textfield',
    '#default_value' => variable_get('amgr_content_type', ''),
  );
  $form['oauth']['amgr_client_id'] = array(
    '#title' => t('Client Id'),
    '#type' => 'textfield',
    '#default_value' => variable_get('amgr_client_id', ''),
  );
  $form['oauth']['amgr_client_secret'] = array(
    '#title' => t('Client Secret'),
    '#type' => 'textfield',
    '#default_value' => variable_get('amgr_client_secret', ''),
  );
  // AMGR API Setting
  $form['ats'] = array(
    '#type' => 'details',
    '#title' => t('AMGR API setting'),
    '#open' => true,
  );
  $form['ats']['amgr_member_url'] = array(
    '#title' => t('Member API URL'),
    '#type' => 'textfield',
    '#default_value' => variable_get('amgr_member_url', ''),
  );
  $form['ats']['amgr_x_client'] = array(
    '#title' => t('X-Client'),
    '#type' => 'textfield',
    '#default_value' => variable_get('amgr_x_client', ''),
  );
  $form['ats']['amgr_x_api_key'] = array(
    '#title' => t('X-Api-Key'),
    '#type' => 'textfield',
    '#default_value' => variable_get('amgr_x_api_key', ''),
  );
  return system_settings_form($form);
}