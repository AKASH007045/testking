<?php
// @file
/**
 * Custom function to set feed url for preseason and current
 * Eg http://data.nba.com/data/5s/v2014/xml/mobile_teams/nba/2014/teams/heat_schedule_01.xml
 */
function custom_mlb_import_settings(){
  $form = array();
  $mlb_game_feed_url = variable_get('mlb_game_feed_url', '');
  $form['game_feed_import_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('MLB Feed Import Settings Form')
  );
  $form['game_feed_import_form']['mlb_game_feed_url'] = array(
    '#type' => 'textfield',
    '#title' => t('MLB Feed Url'),
    '#default_value' => $mlb_game_feed_url,
    '#description' => 'E.g. http://www.milb.com/gen/stats/xml/clubs/147/schedule.xml',
    '#size' => 250,
    '#maxlength' => 300,
    '#required' => TRUE
  );
  return system_settings_form($form);
}