<?php

/**
 * @file
 * Drush integration for the devel module.
 */

/**
 * Implements hook_drush_command().
 */
function twitter_pull_drush_command() {
  $items['twitter-pull-download'] = array(
    'callback' => 'twitter_pull_drush_download',
    'description' => dt('Downloads the Twitter API PHP wrapper from https://github.com/J7mbo/twitter-api-php.'),
    'arguments' => array(
      'path' => dt('Optional. A path to the download folder. If omitted Drush will use the default location (sites/all/libraries/twitter-api-php).'),
    ),
  );

  return $items;
}

/**
 * A command callback.
 */
function twitter_pull_drush_download() {
  $args = func_get_args();

  //If an argument was supplied, use it as the download path
  if (isset($args[0])) {
    $path = $args[0];
  }
  //Otherwise get a location using libraries or a provided default
  else {
    $path = twitter_pull_lib_path(drush_get_context('DRUSH_DRUPAL_ROOT'));
  }

  //Clone from github and then remove the extraneous git files
  if (drush_shell_exec('git clone git://github.com/J7mbo/twitter-api-php.git ' . $path . ' && rm -rf ' . $path . '/.git')) {
    drush_log(dt('twitter-api-php has downloaded to @path.', array('@path' => $path)), 'success');
  }
  //Otherwise, there was a problem
  else {
    drush_log(dt('Drush was unable to export twitter-api-php to @path.', array('@path' => $path)), 'warning');
  }
}
