<?php
// @file
/**
 * Custom function to set feed url for preseason and current
 * Eg http://data.nba.com/data/5s/v2014/xml/mobile_teams/nba/2014/teams/heat_schedule_01.xml
 */
function custom_nfl_import_settings(){
  $form = array();
  $form['game_feed_import_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('NFL Feed Import Settings')
  );
  $form['game_feed_import_form']['nfl_feed_base_url'] = array(
    '#type' => 'textfield',
    '#title' => t('NFA Pre Season Game Feed Url'),
    '#default_value' => variable_get('nfl_feed_base_url', 'http://feeds.nfl.com/feeds/schedules'),
    '#description' => 'E.g. http://feeds.nfl.com/feeds/schedules',
    '#size' => 250,
    '#maxlength' => 300,
    '#required' => TRUE
  );
  
  $form['game_feed_import_form']['nfl_feed_user'] = array(
    '#type' => 'textfield',
    '#title' => t('User Name'),
    '#required' => TRUE,
    '#default_value' => variable_get('nfl_feed_user', 'iomedia'),
  );
  $form['game_feed_import_form']['nfl_feed_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#default_value' => variable_get('nfl_feed_password', 'swc22cza654B'),
    '#required' => TRUE,
  );
  
  $form['game_feed_import_form']['nfl_ticketmaster_run_cron'] = array(
    '#type' => 'checkbox',
    '#title' => t('Run game Feed Import cron?'),
    '#default_value' => variable_get('nfl_ticketmaster_run_cron', 1),
  );
  $form['game_feed_import_form']['nfl_ticketmaster_cron_interval'] = array(
    '#type' => 'textfield',
    '#title' => t('Game feed Cron Interval (in sec)'),
    '#default_value' => variable_get('nfl_ticketmaster_cron_interval', 24 * 60 * 60),
    '#required' => TRUE,
  );
  
  return system_settings_form($form);
}