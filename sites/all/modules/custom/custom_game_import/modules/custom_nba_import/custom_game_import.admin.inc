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
    '#title' => t('NBA Feed Import Settings Form')
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

  $form['game_feed_import_form']['nba_game_feed_url'] = array(
    '#type' => 'textfield',
    '#title' => t('NBA Feed Url'),
    '#default_value' => $nba_game_feed_url,
	'#description' => 'E.g. http://data.nba.com/data/5s/v2014/xml/mobile_teams/nba/2014/teams/DSN_schedule_01.xml',
    '#size' => 250,
    '#maxlength' => 300,
    '#required' => TRUE
  );

  return system_settings_form($form);
}