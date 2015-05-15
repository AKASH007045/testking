<?php

/**
 * @file
 * template.php
 */
/**
 * Add body classes if certain regions have content.
 */
function sports_preprocess_html(&$variables) {
  // Add conditional stylesheets for IE
  drupal_add_css(path_to_theme() . '/css/ie8.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 8', '!IE' => FALSE), 'preprocess' => FALSE));
  drupal_add_css(path_to_theme() . '/css/ie.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 9', '!IE' => FALSE), 'preprocess' => FALSE));
  if (isset($variables['node']) && $variables['node']->type == 'home_page') {
    $variables['classes_array'][] = 'front';
  }
  if (arg(0) == 'schedule') {
    $variables['classes_array'][] = 'page-season-schedule';
  }
}
