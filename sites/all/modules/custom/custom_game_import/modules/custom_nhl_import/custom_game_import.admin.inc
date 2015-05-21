<?php
// @file
/**
 * Custom function to set feed url for preseason and current
 * Eg http://data.nba.com/data/5s/v2014/xml/mobile_teams/nba/2014/teams/heat_schedule_01.xml
 */
function custom_nhl_import_settings(){
  $form = array();
  
  $form['game_feed_import_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('NBA Feed Import Settings Form')
  );
  $form['game_feed_import_form']['nhl_feed_base_url'] = array(
    '#type' => 'textfield',
    '#title' => t('NBA Pre Season Game Feed Url'),
    '#default_value' => variable_get('nhl_feed_base_url', ''),
    '#description' => 'E.g. http://www.nhl.com/feed/nhl/club/schedule.json?team=NYR',
    '#size' => 250,
    '#maxlength' => 300,
    '#required' => TRUE
  );
  
  $form['game_feed_import_form']['nhl_partner_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Partner Name'),
    '#default_value' => variable_get('nhl_partner_name', ''),
    '#description' => 'E.g. IOMedia-RangersScheduleApp',
    '#size' => 250,
    '#maxlength' => 300,
    '#required' => TRUE
  );
  
  $form['game_feed_import_form']['nhl_secret_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret Key related to Partner Name'),
    '#default_value' => variable_get('nhl_secret_key', ''),
    '#description' => '',
    '#size' => 250,
    '#maxlength' => 300,
    '#required' => TRUE
  );

  return system_settings_form($form);
}