<?php
// @file
/**
 * Custom function to set feed url for preseason and current
 * Eg http://data.nba.com/data/5s/v2014/xml/mobile_teams/nba/2014/teams/heat_schedule_01.xml
 */
function custom_nba_import_settings(){
  $form = array();
  
  $nba_preseason_game_feed_url = variable_get('nba_preseason_game_feed_url', '');
  $nba_game_feed_url = variable_get('nba_game_feed_url', '');
  $form['game_feed_import_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('NBA PreSeason Feed Import Settings')
  );
  $form['game_feed_import_form']['nba_preseason_game_feed_url'] = array(
    '#type' => 'textfield',
    '#title' => t('NBA Pre Season Game Feed Url'),
    '#default_value' => $nba_preseason_game_feed_url,
	'#description' => 'E.g. http://data.nba.com/data/5s/v2014/xml/mobile_teams/nba/2014/teams/DSN_schedule_01.xml',
    '#size' => 250,
    '#maxlength' => 300,
    '#required' => TRUE
  );
  $form['game_feed_import_form']['ticketmaster_run_cron_preseason'] = array(
    '#type' => 'checkbox',
    '#title' => t('Run Preseason game Feed Import cron?'),
    '#default_value' => variable_get('ticketmaster_run_cron_preseason', 1),
  );
  $form['game_feed_import_form']['ticketmaster_cron_interval_preseason'] = array(
    '#type' => 'textfield',
    '#title' => t('Ticketmaster Preseason Cron Interval (in sec)'),
    '#default_value' => variable_get('ticketmaster_cron_interval_preseason', 24 * 60 * 60),
    '#required' => TRUE,
  );
  
  $form['game_feed_import_form_post'] = array(
    '#type' => 'fieldset',
    '#title' => t('NBA Season Feed Import Settings')
  );
  $form['game_feed_import_form_post']['nba_game_feed_url'] = array(
    '#type' => 'textfield',
    '#title' => t('NBA Feed Url'),
    '#default_value' => $nba_game_feed_url,
	'#description' => 'E.g. http://data.nba.com/data/5s/v2014/xml/mobile_teams/nba/2014/teams/DSN_schedule_02.xml',
    '#size' => 250,
    '#maxlength' => 300,
    '#required' => TRUE
  );
  $form['game_feed_import_form_post']['nba_ticketmaster_run_cron'] = array(
    '#type' => 'checkbox',
    '#title' => t('Run game Feed Import cron?'),
    '#default_value' => variable_get('nba_ticketmaster_run_cron', 1),
  );
  $form['game_feed_import_form_post']['nba_ticketmaster_cron_interval'] = array(
    '#type' => 'textfield',
    '#title' => t('Ticketmaster Cron Interval (in sec)'),
    '#default_value' => variable_get('nba_ticketmaster_cron_interval', 24 * 60 * 60),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}