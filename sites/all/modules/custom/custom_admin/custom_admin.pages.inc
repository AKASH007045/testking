<?php

/**
 * @FILE
 *
 */

function custom_admin_add_page_settings() {
  $form = array();
  $form['default_page_content'] = array(
    '#type' => 'textarea',
    '#title' => t('Dummy Page Content'),
    '#rows' => 10,
    '#default_value' => variable_get('default_page_content', ''),
    '#required' => TRUE,
  );
  $form['default_pdf_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Default PDF Url'),
    '#rows' => 10,
    '#default_value' => variable_get('default_pdf_url', ''),
    '#required' => TRUE,
  );
  $form['default_video_url'] = array(//homepage
    '#type' => 'textfield',
    '#title' => t('Default Video'),
    '#rows' => 10,
    '#default_value' => variable_get('default_video_url', ''),
    '#required' => TRUE,
  );
  return system_settings_form($form);
}
 
/* * *
 * Create new pdf/content/video page form
 */
function custom_admin_add_page($form, &$form_state, $variable) { 
  $nid = arg(5);
  $type = arg(4);
  $title1 = '';
  
  $title = (isset($variable) && $variable == 'stp_page') ? 'Content' : ucfirst($variable);
  $title = $type == 'pdf' ? 'PDF' : $title;
  drupal_set_title('New ' . $title . ' Page');
  $form = array();
  $stp_menus = custom_admin_get_menu_links('all');
  $stp_menus_anon = custom_admin_get_menu_links('anon');
  $stp_menus_logged = custom_admin_get_menu_links('sth');
  if (isset($form_state['values']['access'])) {
    $stp_menus = ($form_state['values']['access'] == 1) ? $stp_menus_logged : $stp_menus_anon;
  }
  $form['box1'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('stp-legend-heading')),
  );
  $form['box1']['title'] = array(
    '#type' => 'textfield',
    '#title' => (isset($nid) && $nid) ? 'Tab Title' : t('Title'),
    '#required' => TRUE,
  );
  if (!(isset($nid) && $nid)) {
    $form['box2'] = array(
      '#type' => 'fieldset',
      '#title' => 'Other ' . $title .' Page Options', 
    );
    $form['box2']['access'] = array(
      '#type' => 'radios',
      '#title' => t('Access'),
      '#options' => array('0' => 'Public', '1' => 'Logged IN'),
      '#required' => TRUE,
      '#ajax' => array(
        // #ajax has two required keys: callback and wrapper.
        // 'callback' is a function that will be called when this element changes.
        'callback' => 'custom_admin_add_page_ajax_callback',
        // 'wrapper' is the HTML id of the page element that will be replaced.
        'wrapper' => 'replace_parent_menu',
        'method' => 'replace',
        'effect' => 'fade',
      ),
    );
    $form['box2']['parent_menu'] = array(
      '#type' => 'select',
      '#title' => t('Site Map Location'),
      '#options' => $stp_menus,
      '#default_value' => '',
      '#prefix' => '<div id="replace_parent_menu">',
      '#suffix' => '</div>',
      '#required' => TRUE,
      '#description' => t('Select where new page will appear under top navigation.'),
      
    );
  }
  $form['box2']['type'] = array(
    '#type' => 'hidden',
    '#value' => $variable,
  );
  if ($variable == 'stp_page') {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => (isset($nid) && $nid) ? 'Add Tab' : t('Add '. $title .' Page'),
    );
  }
  else {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => (isset($nid) && $nid) ? 'Add Tab' : t('Add New '. $title .' Page'),
    );
  }
  return $form;
}

function custom_admin_add_page_validate($form, &$form_state) {
  $data = $form_state['values'];
  if (isset($data['box2']['parent_menu']) && $data['box2']['parent_menu']) {
     $submenu_count = db_query("SELECT mlid FROM {menu_links} WHERE module = 'menu' AND link_title != 'Bullpen Pages' and plid = :plid", array(':plid' => $data['box2']['parent_menu']))->rowCount();
    if ($submenu_count >=5) {
      form_set_error('title', 'Maximum of 5 menus can be added in SubNavigation bar.');
    }
  }
  $nid = arg(5);
  if (isset($nid) && $nid) {
    $count = db_query("SELECT entity_id FROM {field_data_field_tab_page} WHERE field_tab_page_nid = :nid", array(':nid' => $nid))->rowCount();
    if ($count >=5) {
      form_set_error('title', 'Maximum of 5 tabs can be added in Page.');
    }
  }
 
}
/**
 * Submit handler for Create pdf/video/content pages
 */
function custom_admin_add_page_submit($form, &$form_state) {
  global $user;
  $tab_node_nid = 0;
  $data = $form_state['values'];
  $lang = LANGUAGE_NONE;
  $new_node = new stdClass();
  $new_node->uid = $user->uid;
  $new_node->status = 1;
  $new_node->language = $lang;
  $new_node->type = $data['type'];
  $new_node->title = $data['title'];
  $new_node->uid = $user->uid;
  $access = (isset($form_state['values']['access']) && $form_state['values']['access']) ? $form_state['values']['access'] : 0;
  $new_node->weight_weight = 0;
  /*************************** If adding a new tab page**************************************/
  $nid = arg(5);
  if (isset($nid) && $nid) {
    $node = node_load($nid);
    // Content access Settings for newly created tab page node
    $settings['view'] = content_access_per_node_setting('view', $node);
    if ($node->type == 'tab_page') {
      $tab_node_nid = $nid;
      $new_node->field_tab_page[$lang][0]['nid'] = $nid;
      $new_node->weight_weight = $node->weight_weight + 1;
      $node->weight_weight = $node->weight_weight + 1;
      node_save($node);
    }
    else {
      $tab_node = custom_admin_create_tab_node($node, $data['title']);
      $new_node->field_tab_page[$lang][0]['nid'] = $tab_node->nid;
      $tab_node_nid = $tab_node->nid;
      $new_node->weight_weight = $tab_node->weight_weight + 1;
    }
  }
  //*******************************************************************
  else {
    if ($access) {
      $settings['view'] = array(2,3, 5, 6);
      $settings['view_own'] = array(2,3, 5, 6);
    }
    else {
      $settings['view'] = array(1, 5, 6);
      $settings['view_own'] = array(1, 5, 6);
    }
  }
  $filename = str_replace(' ', '_', $data['title']);
  if ($data['type'] == 'stp_page') {
    $new_node->body[$lang][0]['value'] = variable_get('default_page_content', '');
    $new_node->body[$lang][0]['format'] = 'full_html';
    $new_node->body[$lang][0]['safe_value'] = variable_get('default_page_content', '');
  }
  // Create a PDF
  if ($data['type'] == 'pdf') {
    $path = variable_get('default_pdf_url', 'public://sample.pdf');
    $filename = $data['title'].'pdf';
    $image = file_get_contents($path);
    //Saves a file to the specified destination and creates a database entry.
    $file = file_save_data($image, 'public://pdf/' . basename($path), FILE_EXISTS_RENAME);
    //Set the file status to permanent so it is not deleted in next cron run
    $file->display =1;
    $file->filename = $filename . '_pdf';
    $new_node->field_pdf[$lang][0] = (array)$file;
  }
  // Create a Video
  if ($data['type'] == 'video') {
    $path = variable_get('default_video_url', 'public://sample.mp4');
    $video = file_get_contents($path);
    //Saves a file to the specified destination and creates a database entry.
    $file =  file_save_data($video, 'public://video/' . basename($path), FILE_EXISTS_RENAME);
    //Set the file status to permanent so it is not deleted in next cron run
    $file->display =1;
    //$file->filename = $filename . '_video';
    $new_node->field_video[$lang][0] = (array)$file;
    $new_node->field_bottom_content[$lang][0]['value'] = variable_get('default_page_content', '');
    $new_node->field_bottom_content[$lang][0]['format'] = 'full_html';
    $new_node->field_bottom_content[$lang][0]['safe_value'] = variable_get('default_page_content', '');
  }
  // Add Menu Items
  if (isset($data['parent_menu']) && $data['parent_menu']) {
    $new_node->menu = array(
      'link_title' => $data['title'],
      'description' => '', // needs to pass this also or you will get php warnings
      'menu_name' => 'main-menu', // needs to be an existing mlid or it will generate a new mlid value
      'plid' => $data['parent_menu'], // needs to be an existing foreign mlid id
      'enabled' => 1,
    );
  }
  //$new_node->path['pathauto'] = 0;
  node_object_prepare($new_node);
  node_save($new_node);
 
  // Save per-node settings.
  content_access_save_per_node_settings($new_node, $settings);
  node_access_acquire_grants($new_node);
  module_invoke_all('per_node', $settings);
  if ((isset($tab_node_nid) && $tab_node_nid)) {
    drupal_goto('node/' . $tab_node_nid);
  }
  drupal_goto('node/' . $new_node->nid);
}
/**
 * Ajax callback for Parent Menu
 */
function custom_admin_add_page_ajax_callback($form, $form_state) {
  // The form has already been submitted and updated. We can return the replaced
  // item as it is.
  $access = $form_state['values']['access'];
  $stp_menus_anon = custom_admin_get_menu_links('anon');
  $stp_menus = custom_admin_get_menu_links('sth');
  $form['box2']['parent_menu']['#options'] = ($access == 1) ? $stp_menus : $stp_menus_anon;
  return $form['box2']['parent_menu'];
}

function custom_admin_get_menu_links($menu_type = 'all') {
  // Fetch main menu items
  $main_menu_tree = menu_tree_page_data('main-menu', NULL, TRUE);
  $main_tree = menu_tree_output($main_menu_tree);
  //$tree = menu_tree($pid);
  $result_sth = db_query("SELECT mlid FROM {menu_links} WHERE module = 'menu' AND menu_name = 'main-menu' AND link_title = 'Bullpen Pages'")->fetchField();
   $result_anon = db_query("SELECT mlid FROM {menu_links} WHERE module = 'menu' AND menu_name = 'menu-anonymous-menu' AND link_title = 'Bullpen Pages'")->fetchField();
   $stp_main_menus[''] = '--<None>--';
  foreach ($main_tree as $menu_pid => $menu) {
    if (is_numeric($menu_pid)) {
      $title = (isset($menu['#localized_options']['attributes']['title']) && $menu['#localized_options']['attributes']['title']) ? $menu['#localized_options']['attributes']['title'] : $menu['#title'];
      $stp_main_menus[$menu_pid] = $title;
    }
  }
  if (isset($result_sth) && $result_sth) {
    $stp_main_menus[$result_sth] = 'Bullpen Pages';
  }
  // Fetch Anonymous menu items
  $anon_menu_tree = menu_tree_page_data('menu-anonymous-menu', NULL, TRUE);
  $anon_tree = menu_tree_output($anon_menu_tree);
  //$tree = menu_tree($pid);
   $stp_anon_menus[''] = '--<None>--';
  foreach ($anon_tree as $menu_pid => $menu) {
    if (is_numeric($menu_pid)) {
      $stp_anon_menus[$menu_pid] = $menu['#title'];
    }
  }
  if (isset($result_anon) && $result_anon) {
    $stp_anon_menus[$result_anon] = 'Bullpen Pages';
  }
  
  if ($menu_type == 'all') {
    $menus = array_merge($stp_main_menus, $stp_anon_menus);
  }
  elseif ($menu_type == 'anon') {
    $menus = $stp_anon_menus;
  }
  elseif ($menu_type == 'sth') {
    $menus = $stp_main_menus;
  }
  return $menus;
}


/**
 * function to create a new tab page and associtate the existing page to new tab page
 * @param
 * $node node object
 * $title page title which needs to be created
 * @return
 * create a new node and redirect user to new page
 */
function custom_admin_create_tab_node($node, $title) {
 // print_r($node);
  $menu_mlid =  custom_admin_get_menu_id($node->nid);
  global $user;
  $lang = LANGUAGE_NONE;
  $new_node = new stdClass();
  $new_node->uid = $user->uid;
  $new_node->status = 1;
  $new_node->language = $lang;
  $new_node->type = 'tab_page';
  $new_node->title = isset($node->title) ? $node->title : $title;
  $new_node->uid = $user->uid;
  node_object_prepare($new_node);
  $new_node->weight_weight = $node->weight_weight + 1;
  node_save($new_node);
  // Manage Navigation links
  if ($menu_mlid) {
    $menu = menu_link_load($menu_mlid);
    $menu['link_path'] = 'node/' . $new_node->nid;
    menu_link_save($menu);
    watchdog('Menu', 'Navigation changed for page %page', array('%page' => $menu['link_title']), WATCHDOG_NOTICE);
  }
  // Content access Settings for newly created tab page node
  $settings['view'] = content_access_per_node_setting('view', $node);
  content_access_save_per_node_settings($new_node, $settings);
  node_access_acquire_grants($new_node);
  module_invoke_all('per_node', $settings);
  // If new tab page node created we will add existing page under newly created tab
  $node->field_tab_page[$lang][0]['nid'] = $new_node->nid;
  node_save($node);
  return $new_node;
  //drupal_goto('node/' . $new_node->nid);
}

/**
 * function to fetch menu id of the node
 * @param
 * $nid node id of the node
 * @return integer mlid of menu item if exist
 * 
 */
function custom_admin_get_menu_id($nid) {
  $mlid = db_select('menu_links' , 'ml')
  ->condition('ml.link_path', 'node/'. $nid)
    ->condition('ml.menu_name', 'main-menu')
    ->fields('ml' , array('mlid'))
    ->range(0, 1)
    ->execute()
    ->fetchField();
  return $mlid;
}