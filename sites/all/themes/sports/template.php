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
  drupal_add_css(path_to_theme() . '/css/ie.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 9', '!IE' => FALSE), 'preprocess' => FALSE));
  if (isset($variables['node']) && $variables['node']->type == 'home_page') {
    $variables['classes_array'][] = 'front';
  }
  if (arg(0) == 'schedule') {
    $variables['classes_array'][] = 'page-season-schedule';
  }
  $headers = drupal_get_http_header();
  if (isset($headers['status']) && $headers['status'] == '404 Not Found') {
    $variables['classes_array'][] = 'page-404';
  }
  elseif (isset($headers['status']) && $headers['status'] == '403 Forbidden') {
    $variables['classes_array'][] = 'page-403';
  }
}

//function sports_preprocess_page(&$variables) {
//  global $user;
//  //flog_it($variables);
//  //flog_it(menu_navigation_links('main-menu'));
//  if (!$user->uid) {
//    $variables['main_menu'] = theme('links__system_main_menu', array('links' => menu_navigation_links('menu-anonymous-menu'), 'attributes' => array('class'=> array('menu nav navbar-nav', 'menu')) ));
//  } else {
//    $variables['main_menu'] = theme('links__system_main_menu', array('links' => menu_navigation_links('main-menu'), 'attributes' => array('class'=> array('menu nav navbar-nav', 'menu')) ));
//  }
//}

function sports_menu_tree(&$variables) {
  return '<ul class="menu nav navbar-nav">' . $variables['tree'] . '</ul>';
}
/**
 * Override theme function for PDF formatter
 */
function sports_pdf_formatter_default($variables) {
  $library = libraries_load('pdf.js');
  if ($library['loaded'] == FALSE) {
    drupal_set_message($library['error message'], 'error');
    return 'Please download and install ' . l( $library['name'], $library['download url'] ) . '!';
  }
  $file_url = file_create_url($variables['file']->uri);
  $module_path = drupal_get_path('module', 'pdf');
  $library_path = libraries_get_path('pdf.js');
  $iframe_src = file_create_url($library_path . '/web/viewer.html') . '?file=' . rawurlencode($file_url);
  $force_pdfjs = $variables['keep_pdfjs'];
  $content = array(
    '#type' => 'html_tag',
    '#tag' => 'iframe',
    '#value' => $file_url,
    '#attributes' => array(
      'class' => array('pdf'),
      'webkitallowfullscreen' => '',
      'mozallowfullscreen' => '',
      'allowfullscreen' => '',
      'frameborder' => 'no',
      'width' => $variables['width'],
      'height' => $variables['height'],
      'src' => $iframe_src,
      'data-src' => $file_url,
    ),
  );
  if ($force_pdfjs != TRUE) {
    drupal_add_js($module_path . '/js/acrobat_detection.js');
    drupal_add_js($module_path . '/js/default.js');
  }

  return '<div class="bg-page-link-edit stp-update-pdf"><i class="fa fa-edit"></i> Edit</div>' . render($content);
}

/**
 * Returns HTML for help text based on file upload validators.
 *
 * @param $variables
 *   An associative array containing:
 *   - description: The normal description for this field, specified by the
 *     user.
 *   - upload_validators: An array of upload validators as used in
 *     $element['#upload_validators'].
 *
 * @ingroup themeable
 */
function sports_file_upload_help($variables) {
  $description = $variables['description'];
  $upload_validators = $variables['upload_validators'];

  $descriptions = array();

  if (strlen($description)) {
    $descriptions[] = $description;
  }
  if (isset($upload_validators['file_validate_size'])) {
    $descriptions[] = t('Files must be less than !size', array('!size' => '<strong>' . format_size($upload_validators['file_validate_size'][0])  . '</strong>'));
  }
  if (isset($upload_validators['file_validate_extensions'])) {
    $descriptions[] = t('File types allowed: !extensions', array('!extensions' => '<strong>' . str_replace(' ', ', ', check_plain($upload_validators['file_validate_extensions'][0])) . '</strong>'));
  }
  if (isset($upload_validators['file_validate_image_resolution'])) {
    $max = $upload_validators['file_validate_image_resolution'][0];
    $min = $upload_validators['file_validate_image_resolution'][1];
    if ($min && $max && $min == $max) {
      $descriptions[] = t('Images must be exactly !size pixels.', array('!size' => '<strong>' . $max . '</strong>'));
    }
    elseif ($min && $max) {
      $descriptions[] = t('Images must be between !min and !max pixels.', array('!min' => '<strong>' . $min . '</strong>', '!max' => '<strong>' . $max . '</strong>'));
    }
    elseif ($min) {
      $descriptions[] = t('Images must be larger than !min pixels', array('!min' => '<strong>' . $min . '</strong>'));
    }
    elseif ($max) {
      $descriptions[] = t('Images must be smaller than !max pixels.', array('!max' => '<strong>' . $max . '</strong>'));
    }
  }
  return implode('<br />', $descriptions);
}

function sports_preprocess_page(&$vars) {
  if (isset($vars['node']->type) && $vars['node']->nid == '1175') {
    // If the content type's machine name is "my_machine_name" the file
    // name will be "page--my-machine-name.tpl.php".
    $vars['theme_hook_suggestions'][] = 'page__user__login';
  }
}