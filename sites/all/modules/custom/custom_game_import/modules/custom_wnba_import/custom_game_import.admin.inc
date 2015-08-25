<?php
// @file
/**
 * Custom function to set feed url for preseason and current
 * Eg http://data.wnba.com/data/5s/v2014/xml/mobile_teams/wnba/2014/teams/heat_schedule_01.xml
 */
function custom_wnba_import_settings(){
  $form = array();
  
  $wnba_game_feed_url = variable_get('wnba_game_feed_url', '');
  $form['game_feed_import_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('WNBA Feed Import Settings')
  );
  
  $form['game_feed_import_form']['wnba_game_feed_url'] = array(
    '#type' => 'textfield',
    '#title' => t('WNBA Feed Url'),
    '#default_value' => $wnba_game_feed_url,
	'#description' => 'E.g. http://data.wnba.com/data/5s/v2014/xml/mobile_teams/wnba/2014/teams/DSN_schedule_01.xml',
    '#size' => 250,
    '#maxlength' => 300,
    '#required' => TRUE
  );
  
  $form['game_feed_import_form']['wnba_ticketmaster_run_cron'] = array(
    '#type' => 'checkbox',
    '#title' => t('Run game Feed Import cron?'),
    '#default_value' => variable_get('wnba_ticketmaster_run_cron', 1),
  );
  $form['game_feed_import_form']['wnba_ticketmaster_cron_interval'] = array(
    '#type' => 'textfield',
    '#title' => t('Game feed Cron Interval (in sec)'),
    '#default_value' => variable_get('wnba_ticketmaster_cron_interval', 24 * 60 * 60),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}