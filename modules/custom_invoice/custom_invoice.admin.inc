<?php
/**
 * @file
 */
module_load_include('inc', 'system', 'system.admin');

/*
This functionality is shifted to auto script css branding.
function custom_invoice_settings() {
  $validate =  array(      
    'file_validate_extensions' => array('png jpg jpeg gif'),
    // Pass the maximum file size in bytes
    'file_validate_size' => array(1024*1024*2),
  );
  $form['box1'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('stp-legend-heading')),
  );
  $form['box1']['invoice_logo'] = array(
    '#type' => 'managed_file',
    '#title' => t('Invoice Logo'),
    '#description' => t('Please enter subject for invoice mail.'),
    '#default_value' => variable_get('invoice_logo_fid', ''),
    //'#required' => TRUE,
    '#upload_location' => 'public://invoice/'
  );
  $form['box2'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('stp-legend-heading')),
  );
  $form['box2']['invoice_team_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Invoice Teamname'),
    '#description' => t('Please enter Team name for invoice.'),
    '#default_value' => variable_get('invoice_team_name', t('STP')),
    '#required' => TRUE,
  );
  $form['box3'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('stp-legend-heading')),
  );
  $form['box3']['invoice_watermark'] = array(
    '#type' => 'managed_file',
    '#title' => t('Invoice Watermark'),
    '#description' => t('Please uplaod watermark for invoice.'),
    '#default_value' => variable_get('invoice_watermark_fid', ''),
   // '#required' => TRUE,
     '#upload_validators' => $validate,
    '#upload_location' => 'public://invoice/',
    '#file_upload_description' => theme('file_upload_help', array('upload_validators' => $validate)),
   // '#theme' => array('image_widget'),
  );
  $form['box4'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('stp-legend-heading')),
  );
  $form['box4']['invoice_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Invoice Colors - Lines and Headers'),
    '#description' => t('Please enter HEX color code for lines and headers.'),
    '#default_value' => variable_get('invoice_color', t('#000000')),
    '#required' => TRUE,
    
  );
  $form['box5'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('stp-legend-heading')),
  );
  $footer =  variable_get('invoice_footer', array());
  $form['box5']['invoice_footer'] = array(
    '#type' => 'text_format',
    '#title' => t('Invoice Footer - Team Info'),
    '#rows' => 40,
    '#description' => t('Please enter Invoice footer - team info.'),
    '#default_value' => isset($footer['value'])?$footer['value'] : '',
    '#required' => TRUE,
    '#format' => 'full_html',
  );
  
  
  
  
  
  
  // end of typeform implementation
  
  
  // Perform our custom submit before system submit
  $form['#submit'][] = 'custom_settings_form_submit';
  return system_settings_form($form);
}




function custom_settings_form_submit($form, &$form_state) {
 // global $user;
  // Load the file via file.fid.
  $file = file_load($form_state['values']['invoice_logo']);
  if ($file) {
    // Change status to permanent.
    $file->status = FILE_STATUS_PERMANENT;
    // Save.
    file_save($file);
    // Save file to vatiable
    variable_set('invoice_logo_fid', $file->fid);
    // Record that the module (in this example, user module) is using the file. 
   // file_usage_add($file, 'user', 'user', $user->uid);
    // Unset formstate value
    unset($form_state['values']['invoice_logo']); // make sure it is unset for system submit
    file_usage_add($file, 'file', 'file', $file->fid);
  }
   $file = file_load($form_state['values']['invoice_watermark']);
  if ($file) {
    // Change status to permanent.
    $file->status = FILE_STATUS_PERMANENT;
    // Save.
    file_save($file);
    // Save file to vatiable
    variable_set('invoice_watermark_fid', $file->fid);
    // Record that the module (in this example, user module) is using the file. 
   // file_usage_add($file, 'user', 'user', $user->uid);
    // Unset formstate value
    file_usage_add($file, 'file', 'file', $file->fid);
    unset($form_state['values']['invoice_watermark']); // make sure it is unset for system submit
  }
}*/

/**
 * System settings form for Invoice Listing page
 * 
 */
function custom_admin_invoice_listpage_setting() {
  $form['box1'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('stp-legend-heading')),
  );
  $form['box1']['multi_invoice_filter_flag'] = array(
    '#type' => 'checkbox',
    '#title' => t('Filter Invoices by Flag'),
    '#description' => t('Please choose if you want to filter invoices using underscore or other value.'),
    '#default_value' => variable_get('multi_invoice_filter_flag', 0),
  );
  $form['box1']['multi_invoice_filter_current'] = array(
    '#type' => 'select',
    '#options' => array('' => 'Show All', '_' => 'Underscore _', '#' => 'Hash #'),
    '#title' => t('Choose Invoice Filter for Current Invoices'),
    '#description' => t('Please choose the invoice filter'),
    '#default_value' => variable_get('multi_invoice_filter_current', ''),
  );
  $form['box1']['multi_invoice_filter_past'] = array(
    '#type' => 'select',
    '#options' => array('' => 'Show All', '_' => 'Underscore _', '#' => 'Hash #'),
    '#title' => t('Choose Invoice Filter for Past Invoices'),
    '#description' => t('Please choose the invoice filter'),
    '#default_value' => variable_get('multi_invoice_filter_past', ''),
  );
  return system_settings_form($form);
}

function custom_invoice_admin_invoice_list() {
  $header = array(
    array('data' => 'ID'),
    array('data' => 'Title'),
    array('data' => 'Season'),
    array('data' => 'Type'),
    array('data' => 'API'),
    array('data' => 'Value'),
    array('data' => 'Status'),
    array('data' => 'Edit'),
    array('data' => 'Clone')
  );
  $query = db_select('stp_invoice', 'n');
  $query->fields('n')
  ->orderBy('n.invoice_status', 'ASC');
  $result =$query->execute();
  
  $row = array();
  foreach ($result as $key => $data) {
    $row = array();
    $row[] = $data->id;
    $row[] = $data->invoice_title;
    $row[] = $data->invoice_season;
    $row[] = $data->invoice_type;
    $row[] = $data->invoice_field;
    $row[] = $data->invoice_value;
    $row[] = ($data->invoice_status ==1) ? 'Active' : 'In-Active';
    $row[] = l(t('Edit'), 'admin/manage/settings/invoice/' . $data->id);
    $row[] = l(t('Clone'), 'admin/manage/settings/invoice-clone/' . $data->id);
    $rows[] = $row;
  }
  $link = l(t('Add Invoice'), 'admin/manage/settings/invoice/0', array('query' => array('destination' => 'admin/manage/settings/invoice/list'), 'attributes' => array('class' => array('add-invoice backtopage form-submit'))));
  return $link . theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'yankees-plans')));
}

/**
 * Renewal Invoice settings
 */
function custom_invoice_admin_invoice_form($form, &$form_state, $id) {
  drupal_add_js(drupal_get_path('module', 'custom_invoice') . '/js/admin_preview_popup.js');
  drupal_add_css(drupal_get_path('module', 'custom_invoice') . '/css/invoice_admin.css');
  if ($id) {
    $query = db_select('stp_invoice', 'n');
    $query->fields('n');
    $query->condition('n.id', $id);
    $data = $query->execute()->fetchObject();
    $invoice_list_query = db_select('stp_invoice_details', 'n');
    $invoice_list_query->fields('n');
    $invoice_list_query->condition('n.inv_conf_id', $id);
    $invoice_data = $invoice_list_query->execute()->fetchAll();
    
    $thankyou_query = db_select('stp_invoice_thankyou', 'n');
    $thankyou_query->fields('n');
    $thankyou_query->condition('n.inv_conf_id', $id);
    $thankyou_data = $thankyou_query->execute()->fetchAll();
    
    $typeform_query = db_select('stp_invoice_typeform', 'n');
    $typeform_query->fields('n');
    $typeform_query->condition('n.inv_conf_id', $id);
    $typeform_data = $typeform_query->execute()->fetchAll();
  }
  $form['box0'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('stp-legend-heading')),
  );
  $form['box0']['invoice_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Invoice Title'),
    '#description' => t('Please enter the title of invoice.'),
    '#default_value' => isset($data->invoice_title) ? $data->invoice_title : '',
  );
  $form['box0']['stp_invoice_filter'] = array(
    '#type' => 'radios',
    '#title' => t(''),
    '#options' => array('0' => 'Enable All Invoices', '1' => 'Enable Recent INET Enabled Invoice', '2' => 'Enable Invoice on the basis of below filters'),
    '#default_value' => isset($data->stp_invoice_filter) ? $data->stp_invoice_filter : 2,
  );
  $form['box1'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('stp-legend-heading')),
  );
  $form['box1']['invoice_type'] = array(
    '#type' => 'select',
    '#title' => t('Invoice Type'),
    '#options' => array('renewal' => 'Renewal', 'playoff' => 'Playoff'),
    '#description' => t('Please select invoice type.'),
    '#default_value' => isset($data->invoice_type) ? $data->invoice_type : '',
  );
  $form['box1']['invoice_field'] = array(
    '#type' => 'select',
    '#options' => array('invoice_descriptions' => 'Invoice Descriptions', 'invoice_long_descriptions' => 'Invoice Long Descriptions'),
    '#title' => t('API Field'),
    '#description' => t('Please select invoice filter'),
    '#default_value' => isset($data->invoice_field) ? $data->invoice_field : '',
  );
  $form['box1']['invoice_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Value'),
    '#description' => t('Please enter comma separated invoice filter value'),
    '#default_value' => isset($data->invoice_value) ? $data->invoice_value : '',
  );
  $form['box1']['invoice_season'] = array(
    '#type' => 'select',
    '#title' => t('Season'),
    '#options' => array('current' => 'Current', 'past' => 'Past'),
    '#description' => t('Please select season of this invoice'),
    '#default_value' => isset($data->invoice_season) ? $data->invoice_season : '',
  );
  $form['box1']['invoice_status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#options' => array('1' => 'Active', '0' => 'In-Active'),
    '#description' => t('Please select the status of invoice'),
    '#default_value' => isset($data->invoice_status) ? $data->invoice_status : '',
  );
  // Invoice table starts here
  $inv_options = array('num_seat' => 'num_seat', 'seat_num' => 'seat_num', 'last_seat' => 'last_seat', 'unit_price' => 'unit_price', 'invoiced_amount' => 'invoiced_amount', 'current_due_amount' => 'current_due_amount', 'paid_amount' => 'paid_amount', 'event_name' => 'event_name', 'ga_indicator' => 'ga_indicator',  'seat_label' => 'seat_label', 'row_label' => 'row_label', 'section_label' => 'section_label', 'section_name' => 'section_name', 'row_name' => 'row_name', 'seat_increment' => 'seat_increment', 'ticket_type' => 'ticket_type', 'item_name' => 'invoice_list_item_name', 'item_name_long' => 'item_name_long', 'item_type' => 'item_type', 'event_name_inet' => 'Event Details: inet_name', 'event_description' => 'Event Details: description');
  $style = array('7.5' => 'qty {width: 7.5%;}', '10' => '{width: 10%;}', '12' => 'Section/Row/Seat {width: 12%;}', '12.5' => 'totalCost {width: 12.5%;}', '20' => '{width:20%;}', '30' => '{width:30%;}', '44' => '{width:44%;}');
  $wt = array(0, 1, 2, 3, 4, 5, 6, 7, 8);
  $form['box'] = array(
    '#type' => 'fieldset',
    '#title' => 'INVOICE DETAILS',
    '#attributes' => array('class' => array('stp-legend-heading')),
  );
  $form['box']['tableheader'] = array(
    '#type' => 'markup',
    '#attributes' => array('class' => array('stp-legend-heading')),
    '#markup' => '<table class="cmttblcls"><tr><th>Heading</th><th>Heading Title</th><th class="th3api">TM API Field</th><th>Width(in %)</th><th>Sort</th><th>Desktop Enabled?</th><th>Mob Enabled?</th></tr>',
  );
  // Fields
  $headings = array('TQTY', 'DESCRIPTION', 'SECTION', 'ROW', 'SEATS', 'PRICE PER ITEM', 'OTHER FIELD', 'CALCULATED FIELD', 'TOTAL COST');
  $temp_fields = array('num_seat', 'item_name_long', 'section_name', 'row_name', 'seat_num-last_seat', 'unit_price', 'unit_price','invoiced_amount * 0.25', 'invoiced_amount');
  $temp_css = array(7.5, 44, 12, 12, 12, 7.5, 7.5, 7.5, 12.5);
  for ($i=0; $i<= 8;$i++) {
    // ID field
    $form['id_'. $i] = array(
      '#type' => 'hidden',
      '#value' => isset($invoice_data[$i]->id) ? $invoice_data[$i]->id : '',
    );

    $form['title_'. $i] = array(
      '#type' => 'textfield',
      '#prefix' => '<tr><td>' . $headings[$i] . '</td><td>',
      '#suffix' => '</td>',
      '#default_value' => isset($invoice_data[$i]->title) ? $invoice_data[$i]->title : $headings[$i],
      '#attributes' => array('style' => 'width:200px;'),
    );

    if ($i == 0 || $i == 2 || $i == 3 || $i == 4) {
      $form['field_' . $i] = array(
       '#type' => 'textfield',
       '#attributes' => array('disabled' => 'disabled'),
       '#prefix' => '<td class="td3api">',
       '#suffix' => '</td>',
      '#default_value' => isset($invoice_data[$i]->field) ? $invoice_data[$i]->field : $temp_fields[$i],
      );
    }
    elseif ($i == 7) {
       $form['field_' . $i] = array(
       '#type' => 'textfield',
       '#prefix' => '<td class="td3api">',
       '#attributes' => array('disabled' => 'disabled'),
       '#suffix' => '</td>',
      '#default_value' => isset($invoice_data[$i]->field) ? $invoice_data[$i]->field : $temp_fields[$i],
      );
    }
    else {
      $form['field_' . $i] = array(
        '#type' => 'select',
        '#options' => $inv_options,
        '#prefix' => '<td>',
        '#suffix' => '</td>',
        '#default_value' => isset($invoice_data[$i]->field) ? $invoice_data[$i]->field : $temp_fields[$i],
      );
    }

    $form['class_' . $i] = array(
      '#type' => 'select',
      '#options' => $style,
      '#prefix' => '<td>',
      '#default_value' => isset($invoice_data[$i]->class) ? $invoice_data[$i]->class : $temp_css[$i],
      '#suffix' => '</td>',
    );

    $form['weight_' . $i] = array(
      '#type' => 'select',
      '#options' => $wt,
      '#prefix' => '<td>',
      '#default_value' => isset($invoice_data[$i]->weight) ? $invoice_data[$i]->weight : $wt[$i],
      '#suffix' => '</td>',
    );

    $form['enable_' . $i] = array(
      '#type' => 'checkbox',
      '#prefix' => '<td>',
      '#attributes' => ($i== 7) ? array('disabled' => 'disabled') : array(),
      '#default_value' => isset($invoice_data[$i]->enable) ? $invoice_data[$i]->enable : ($i <= 4 || $i == 8) ? 1 : 0,
      '#suffix' => '</td>',
    );
    $form['mob_enable_' . $i] = array(
      '#type' => 'checkbox',
      '#prefix' => '<td>',
      '#attributes' => ($i== 7) ? array('disabled' => 'disabled') : array(),
      '#default_value' => isset($invoice_data[$i]->mob_enable) ? $invoice_data[$i]->mob_enable : ($i <= 4 || $i == 8) ? 1 : 0,
      '#suffix' => '</td></tr>',
    );
  }
  $form['tablefooter'] = array(
    '#type' => 'markup',
    '#markup' => '</tr></table>',
  );
  // Calculated Field
  /*$form[$i]['id_8'] = array(
      '#type' => 'hidden',
      '#value' => isset($invoice_data[$i]->id) ? $invoice_data[$i]->id : 0,
      '#prefix' => '<tr>',
    );
  $form[$i]['title_8'] = array(
      '#type' => 'textfield',
      '#placeholder' => $headings[8],
      '#prefix' => '<tr><td>'. $headings[8] .'</td><td>',
      '#suffix' => '</td>',
      '#default_value' => isset($invoice_data[8]->title) ? $invoice_data[8]->title : $headings[8],
      '#attributes' => array('style' => 'width:200px;'),
    );
    $form['field_8'] = array(
      '#type' => 'textfield',
      '#prefix' => '<td>',
      '#suffix' => '</td>',
      '#default_value' => isset($invoice_data[8]->field) ? $invoice_data[8]->field  : 'invoiced_amount * 0.25',
      '#attributes' => array('style' => 'width:152px;'),
    );
    $form['class_8'] = array(
      '#type' => 'select',
      '#options' => $style,
      '#prefix' => '<td>',
      '#default_value' => isset($invoice_data[8]->class) ? $invoice_data[8]->class : '7.5',
      '#suffix' => '</td>',
    );
    $form['weight_8'] = array(
      '#type' => 'select',
      '#options' => $wt,
      '#prefix' => '<td>',
      '#default_value' => isset($invoice_data[8]->weight) ? $invoice_data[8]->weight : 8,
      '#suffix' => '</td>',
    );
    $form['enable_8'] = array(
      '#type' => 'checkbox',
      '#prefix' => '<td>',
      '#default_value' => isset($invoice_data[8]->enable) ? $invoice_data[8]->enable : 0,
      '#suffix' => '</td></tr></table>',
    );*/
  $form['box11'] = array(
    '#type' => 'fieldset',
    '#title' => 'INVOICE SUMMARY',
    '#attributes' => array('class' => array('stp-legend-heading')),
  );
  $form['box11']['invoice_total_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Ticket Total Label'),
    '#required' => TRUE,
    '#description' => t('Please enter the ticket total label appearing on invoice details page'),
    '#default_value' => isset($data->invoice_total_title) ? $data->invoice_total_title : 'Total Ticket Amount',
  );
  $form['box11']['invoice_credits_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Less Payments/Credits Label'),
    '#description' => t('Please enter the Less Payments/Credits label appearing on invoice details page'),
    '#default_value' => isset($data->invoice_credits_title) ? $data->invoice_credits_title : 'Less Payments/Credits (-)',
    '#required' => TRUE,
  );
  $form['box11']['invoice_amount_desc'] = array(
    '#type' => 'textfield',
    '#title' => t('Invoice Details Description '),
    '#description' => t('Please enter the invoice description appearing on invoice details page.'),
    '#default_value' => isset($data->invoice_amount_desc) ? $data->invoice_amount_desc : 'Deadline for Payment: mm/dd/yy',
  );
  $form['box11']['invoice_details_text'] = array(
    '#type' => 'textfield',
    '#maxlength' => 62,
    '#title' => t('Text Displayed below Invoice Details'),
    '#description' => t('Please enter text appearing below invoice details section.'),
    '#default_value' => isset($data->invoice_details_text) ? $data->invoice_details_text : '',
  );
  $form['box11']['invoice_other_info_text'] = array(
    '#type' => 'text_format',
    '#format' => 'full_html',
    '#maxlength' => 1200,
    '#title' => t('Invoice Other Info Text'),
    '#description' => t('Please enter the invoice other information text appearing on invoice details page.'),
    '#default_value' => isset($data->invoice_other_info_text) ? $data->invoice_other_info_text : '',
  );
  $form['box11']['parking'] = array(
    '#type' => 'fieldset',
    '#title' => 'Parking',
    '#attributes' => array('class' => array('stp-legend-heading')),
  );
  $form['box11']['handling'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('stp-legend-heading')),
  );
  $form['box11']['parking']['invoice_en_park'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Parking?'),
    '#description' => t('Please check if you want to enable parking for this invoice.'),
    '#default_value' => isset($data->invoice_en_park) ? $data->invoice_en_park : 0,
  );
  $form['box11']['parking']['invoice_park_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Parking Label'),
    '#description' => t('Please enter the parking label appearing on invoice details page'),
    '#default_value' => isset($data->invoice_park_title) ? $data->invoice_park_title : 'Parking Amount',
  );
  $form['box11']['parking']['invoice_park_event'] = array(
    '#type' => 'textfield',
    '#title' => t('Parking Events'),
    '#description' => t('Please enter the comma separated values for parking events'),
    '#default_value' => isset($data->invoice_park_event) ? $data->invoice_park_event : '',
  );
  $form['box11']['parking']['invoice_park_api_field'] = array(
    '#type' => 'select',
    '#title' => t('Parking API Field'),
    '#options' => array('item_name' => 'item_name', 'item_name_long' => 'item_name_long', 'event_name' => 'event_name'),
    '#description' => t('Please choose the API field to match the parking events'),
    '#default_value' => isset($data->invoice_park_api_field) ? $data->invoice_park_api_field : 'item_name_long',
  );
  $form['box11']['handling']['invoice_en_hand'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Handling?'),
    '#description' => t('Please check if you want to enable handling for this invoice.'),
    '#default_value' => isset($data->invoice_en_hand) ? $data->invoice_en_hand : 0,
  );
  $form['box11']['handling']['invoice_hand_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Handling Label'),
    '#description' => t('Please enter the handling label appearing on invoice details page'),
    '#default_value' => isset($data->invoice_hand_title) ? $data->invoice_hand_title : 'Handling Fee',
  );
  $form['box11']['handling']['invoice_hand_event'] = array(
    '#type' => 'textfield',
    '#title' => t('Handling Events'),
    '#description' => t('Please enter the comma separated values for handling fee events'),
    '#default_value' => isset($data->invoice_hand_event) ? $data->invoice_hand_event : '',
  );
  $form['box11']['handling']['invoice_hand_api_field'] = array(
    '#type' => 'select',
    '#title' => t('API Field'),
    '#options' => array('item_name' => 'item_name', 'item_name_long' => 'item_name_long', 'event_name' => 'event_name'),
    '#description' => t('Please choose the API field to match the handling fee events'),
    '#default_value' => isset($data->invoice_hand_api_field) ? $data->invoice_hand_api_field : 'item_name_long',
  );
  // Invoice table ends here
   $form['box21'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('stp-legend-heading')),
    //'#weight' => -10,
  );

  $form['box21']['invoice_en_typeform'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Typeform?'),
    '#description' => t('Please check if you want to enable typeform for this invoice.'),
    '#default_value' => isset($data->invoice_en_typeform) ? $data->invoice_en_typeform : 0,
  );

  $results = array();
  // Implementeation for Typeform automation
  if (db_table_exists('field_data_field_typeform_type')) {
    $query  = db_query("
      SELECT n.title AS title, n.nid AS nid FROM {node} n 
      INNER JOIN {field_data_field_typeform_type} t ON n.nid = t.entity_id 
      AND (t.entity_type = 'node')
      WHERE (n.type = 'typeform') AND (t.field_typeform_type_value = 'invoice')");
    $records = $query->fetchAll();
    foreach($records as $result) {
      $results[$result->nid] = $result->title;
    }
  }
  $user_roles = user_roles();
  $roles = user_roles(TRUE);
  $grps[4] = 'STH User';
  foreach ($roles as $key => $role) {
    if(strpos($role, '_') !== FALSE) {
      $grps[$key] = $role;
    }
   }
  /****************************************************************Added option for multiple typeform pages**************************************************************************************/
  
  // Fieldset container where we can add unlimited typeform for particular roles
  $form['box2'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="wrapper_closed">',
    '#suffix' => '</div>',
    '#attributes' => array('class' => array('stp-legend-heading')),
    '#collapsible' => FALSE,
    '#description' => t('Add Typrform for some account groups.'),
  );
  // Add a flag to add on additional typeform if the user clicks "Add One More"
  $form_state['storage']['add_typeform'] = FALSE;
  if (isset($form_state['triggering_element']) && $form_state['triggering_element']['#value'] == 'Add More') {
    $form_state['storage']['add_typeform'] = TRUE;
  }
  $form_state['storage']['typeform_init'] = isset($form_state['storage']['typeform_init']) ? $form_state['storage']['typeform_init'] : TRUE;
  // First time through, grab the values from
  if ($form_state['storage']['typeform_init']) {
    $typeform_arr = $typeform_data;
    $form_state['storage']['typeform_init'] = FALSE;
  // On subsequent renderings, grab the values from the values array
  } else {
    // get current values from form_state
    $typeform_arr = $form_state['values']['box2'];
    // remove the Add One More button from this array
    array_pop($typeform_arr);
    $typeform_cnt = count($typeform_arr);
  }
  // Get all the stored closed typeform exceptions and remove the user-selected index
  if (isset($form_state['triggering_element']) && $form_state['triggering_element']['#value'] == 'Remove Typeform') {
    // retrieve the index of the one we need to remove
    // preg_match required to get around Drupal bug related to multiple
    // submit buttons with the same name
    preg_match('/\d+/', $form_state['triggering_element']['#name'], $remove_typeform);
    $remove_typeform = $remove_typeform[0];
    unset($typeform_arr[$remove_typeform]);
    // reorder the array, so we have expected 0 through (count-1) indices
    $typeform_arr = array_values($typeform_arr);

    // Update the form values that will be submitted
    unset($form_state['input']['box2'][$remove_typeform]);
    $form_state['input']['box2'] = array_values($form_state['input']['box2']);
    // or re-rendered to the form
    unset($form_state['complete form']['box2'][$remove_typeform]);
    unset($form_state['values']['box2'][$remove_typeform]);
  }

  $typeform_cnt = count($typeform_arr);
  if($typeform_cnt == 0){
    $typeform_cnt = 1; 
  }
  // Add all the current closed Typeform exceptions
  for ($i = 0; $i < $typeform_cnt; $i++) {
     $form['box2'][$i]['account_type'] = array(
      '#type' => 'select',
        '#title' => t('Account Groups'),
        '#chosen' => TRUE,
        '#multiple' => TRUE,
        '#options' => $grps,
        //'#attributes' => array('class' => array('stp-inv-acctgrp')),
        '#default_value' => explode(',', $typeform_data[$i]->acct_grp),
      );
    $form['box2'][$i]['typeform_nid'] = array(
      '#type' => 'select',
      '#title' => t('Typeform URL'),
      '#attributes' => array('class' => array('stp-inv-typeform-url')),
      '#options' => $results,
      '#default_value' => $typeform_data[$i]->typeform_nid,
    );
    // Add a remove button for each one
    $form['box2'][$i]['remove_typeform'] = array(
      '#type' => 'button',
      '#value' => t('Remove Typeform'),
      '#name' => "removemon-$i", // need to create unique names, regardless of tree structure
      '#ajax' => array(
        'callback' => 'typeform_ajax_callback',
        'wrapper' => 'wrapper_closed'
      ),
    );
  }

  // Check if user clicked "Add One More"
  if ($form_state['storage']['add_typeform']) {
    // index for this one is at the end of the stored values
    $index = count($typeform_arr);
      $form['box2'][$index]['account_type'] = array(
      '#type' => 'select',
      '#multiple' => true,
      '#title' => t('Account Groups'),
      '#attributes' => array('class' => array('stp-typeform')),
      '#chosen' => TRUE,
      '#options' => $grps,
    );
    $form['box2'][$index]['typeform_nid'] = array(
      '#type' => 'select',
      '#title' => t('Typeform URL'),
      '#options' => $results,
    );
    $form['box2'][$index]['remove_typeform'] = array(
      '#type' => 'button',
      '#value' => t('Remove Typeform'),
      '#name' => "removemon-$index", // need to create unique names, regardless of tree structure
      '#ajax' => array(
        'callback' => 'typeform_ajax_callback',
        'wrapper' => 'wrapper_closed',
      ),
    );
  }
  $form['box2']['add_open_typeform'] = array(
    '#type' => 'button',
    '#value' => t('Add More'),
    '#name' => t('add-monday'),
    '#ajax' => array(
      'callback' => 'typeform_ajax_callback',
      'wrapper' => 'wrapper_closed'
    ),
  );
 
 /****************************************************************Added option for multiple payment success pages**************************************************************************************/
   // Fieldset container where we can add Thankyou message
  $form['box5'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#prefix' => '<div id="box5">',
    '#suffix' => '</div>',
	 '#attributes' => array('class' => array('stp-legend-heading')),
	'#collapsible' => FALSE,
	'#tree' => TRUE,
    '#description' => t('Add Thank you message for some account groups.'),
  );
  // Add a flag to add on additional box5 if the user clicks "Add One More"
  $form_state['storage']['add_message'] = FALSE;
  if (isset($form_state['triggering_element']) && $form_state['triggering_element']['#value'] == 'Add One More') {
    $form_state['storage']['add_message'] = TRUE;
  }
  $form_state['storage']['message_init'] = isset($form_state['storage']['message_init']) ? $form_state['storage']['message_init'] : TRUE;
  // First time through, grab the values from the DB
  if ($form_state['storage']['message_init']) {
    $box5_array = $thankyou_data;
    $form_state['storage']['message_init'] = FALSE;
  // On subsequent renderings, grab the values from the values array
  } else {
    // get current values from form_state
    $box5_array = $form_state['values']['box5'];
    // remove the Add One More button from this array
    array_pop($box5_array);
    $num_box5 = count($box5_array);
  }
  // Get all the stored closed box5 and remove the user-selected index
  if (isset($form_state['triggering_element']) && $form_state['triggering_element']['#value'] == 'Remove Message') {
    // retrieve the index of the one we need to remove
    // preg_match required to get around Drupal bug related to multiple
    // submit buttons with the same name
    preg_match('/\d+/', $form_state['triggering_element']['#name'], $remove_message);
    $remove_message = $remove_message[0];
    unset($box5_array[$remove_message]);
    // reorder the array, so we have expected 0 through (count-1) indices
    $box5_array = array_values($box5_array);

    // Update the form values that will be submitted
    unset($form_state['input']['box5'][$remove_message]);
    $form_state['input']['box5'] = array_values($form_state['input']['box5']);
    // or re-rendered to the form
    unset($form_state['complete form']['box5'][$remove_message]);
    unset($form_state['values']['box5'][$remove_message]);
  }
  $num_box5 = count($box5_array);
  if($num_box5 == 0){
    $num_box5 = 1; 
  }
  // Add all the current box5
  for ($i = 0; $i < $num_box5; $i++) {
    $form['box5'][$i]['account_grp'] = array(
      '#type' => 'select',
      '#multiple' => true,
      '#title' => t('Account Groups'),
      '#chosen' => TRUE,
      '#options' => $grps,
      '#default_value' => explode(',', $thankyou_data[$i]->acct_grp),
    );
    $message_value = t(variable_get('message', 'THANK YOU<br/>YOUR TRANSACTION IS COMPLETE.'));
    $form['box5'][$i]['message'] = array(
      '#type' => 'text_format',
      '#title' => t('Message'),
      '#format' => 'full_html',
      '#default_value' => isset($thankyou_data) && $thankyou_data ? $thankyou_data[$i]->message : $message_value,
    );
    // Add a remove button for each one
    $form['box5'][$i]['remove_message'] = array(
      '#type' => 'button',
      '#value' => t('Remove Message'),
      '#name' => "removehol-$i", // need to create unique names, regardless of tree structure
      '#ajax' => array(
        'callback' => 'message_ajax_callback',
        'wrapper' => 'box5'
      ),
    );
  }
  // Check if user clicked "Add One More"
  if ($form_state['storage']['add_message']) {
    // index for this one is at the end of the stored values
    $index = count($box5_array);
      $form['box5'][$index]['account_grp'] = array(
      '#type' => 'select',
      '#multiple' => true,
      '#title' => t('Account Groups'),
      '#chosen' => TRUE,
      '#options' => $grps,
    );
    $form['box5'][$index]['message'] = array(
      '#type' => 'text_format',
      '#title' => t('Message'),
      '#format' => 'full_html',
      '#default_value' => isset($thankyou_data) && $thankyou_data ? $thankyou_data[$i]->message : $message_value,
    );
    $form['box5'][$index]['remove_message'] = array(
      '#type' => 'button',
      '#value' => t('Remove Message'),
      '#name' => "removehol-$index", // need to create unique names, regardless of tree structure
      '#ajax' => array(
        'callback' => 'message_ajax_callback',
        'wrapper' => 'box5',
      ),
    );
  }

  $form['box5']['add_message'] = array(
    '#type' => 'button',
    '#value' => t('Add One More'),
    '#name' => t('add-holiday'),
    '#ajax' => array(
      'callback' => 'message_ajax_callback',
      'wrapper' => 'box5'
    ),
  );

  // Email Invoice page Settings
//  $form['box3'] = array(
//    '#type' => 'fieldset',
//    '#attributes' => array('class' => array('stp-legend-heading')),
//  );
//  $form['box3']['invoice_mail_subject'] = array(
//    '#type' => 'textfield',
//    '#title' => t('"Email Invoice" Subject'),
//    '#description' => t('Please enter subject for email invoice.'),
//    '#default_value' => isset($data->invoice_mail_subject) ? $data->invoice_mail_subject : '2016-17 STP Renewal Invoice',
//    '#required' => TRUE,
//  );
//  $form['box3']['invoice_mail_body'] = array(
//    '#type' => 'textarea',
//    '#title' => t('"Email Invoice" Body'),
//    '#rows' => 5,
//    '#description' => t('Please enter email invoice message.'),
//    '#default_value' => isset($data->invoice_mail_body) ? $data->invoice_mail_body : variable_get('invoice_mail_body', t('Attached to this email is a copy of your invoice for the 2016-17 Season Ticket Portal Subscription.')),
//    '#required' => TRUE,
//  );
  // Payment Email Settings
  $form['box4'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('stp-legend-heading')),
  );
  $invoice_mail = custom_invoice_fetch_template_list('email-invoice');
  $form['box4']['invoice_template'] = array(
    '#type' => 'select',
    '#default_value' => isset($data->invoice_template) ? $data->invoice_template : '',
    '#options' =>  $invoice_mail, 
    '#title' => t('Invoice Email Template'),
    '#description' => t('Please select the email template for invoice.'),
     
  );
  $payment_mail = custom_invoice_fetch_template_list('invoice');
  $form['box4']['payment_template'] = array(
    '#type' => 'select',
    '#default_value' => isset($data->payment_template) ? $data->payment_template : '',
    '#options' =>  $payment_mail, 
    '#title' => t('Payment Email Template'),
    '#description' => t('Please select the email template for invoice.'),
  );
//  $form['box4']['invoice_payment_subject'] = array(
//    '#type' => 'textfield',
//    '#title' => t('"Payment Success Email" Subject'),
//    '#description' => t('Please enter subject for invoice payment mail.'),
//    '#default_value' => isset($data->invoice_payment_subject) ? $data->invoice_payment_subject : variable_get('invoice_payment_subject', t('Thank you For Your Payment')),
//    '#required' => TRUE,
//  );
//  $form['box4']['invoice_payment_body'] = array(
//    '#type' => 'textarea',
//    '#title' => t('"Payment Success Email" Body'),
//    '#rows' => 5,
//    '#description' => t('Please enter the message text for successful payment mail. Available tokens are: %amount, %cc_num'),
//    '#default_value' => isset($data->invoice_payment_body) ? $data->invoice_payment_body : variable_get('invoice_payment_body', 'Thanks for your payment.<br/> $ %amount was charged to your card ending in %cc_num<br/>Get ready for a great 2014 season! Go Cowboys <br/>Your account manager will reflect payment within 24 hours.  We are excited for another season in AT&T Stadium and appreciate your support as a loyal Dallas Cowboys season ticket holder.  You will receive your tickets through UPS before the first preseason game. Please contact your service coordinator if you have any questions.'),
//    '#required' => TRUE,
//  );
   // Payment Successs page Settings
  /*$form['box5']['invoice_payment_success_body'] = array(
    '#type' => 'textarea',
    '#title' => t('"Payment Success" Page content'),
    '#rows' => 5,
    '#description' => t('Please enter message text for successful payment page.'),
    '#default_value' => isset($data->invoice_payment_success_body) ? $data->invoice_payment_success_body : variable_get('invoice_payment_success_body', '<h1>THANK YOU</h1><h2>YOUR TRANSACTION IS COMPLETE</h2>'),
    '#required' => TRUE,
  );*/
  // Payment Failure page Settings

  // Payment page Settings
  $form['box7'] = array(
    '#type' => 'fieldset',
    '#title' => 'PAYMENT FORM',
    '#attributes' => array('class' => array('stp-legend-heading')),
  );
  $form['box7']['invoice_surname'] = array(
    '#type' => 'textfield',
    '#rows' => '3',
    '#title' => t('Label for Name/Surname'),
    '#description' => t('Please enter the Label for Name/Surname.'),
    '#default_value' => isset($data->invoice_surname) ? $data->invoice_surname : '',
  );
  $form['box7']['invoice_state'] = array(
    '#type' => 'textfield',
    '#rows' => '3',
    '#title' => t('Label for State'),
    '#description' => t('Please enter the Label for State.'),
    '#default_value' => isset($data->invoice_state) ? $data->invoice_state : '',
  );
  $form['box7']['invoice_city'] = array(
    '#type' => 'textfield',
    '#rows' => '3',
    '#title' => t('Label for City'),
    '#description' => t('Please enter the Label for City.'),
    '#default_value' => isset($data->invoice_city) ? $data->invoice_city : '',
  );
  $form['box7']['invoice_zipcode'] = array(
    '#type' => 'textfield',
    '#rows' => '3',
    '#title' => t('Label for Zipcode'),
    '#description' => t('Please enter the Label for Zipcode.'),
    '#default_value' => isset($data->invoice_zipcode) ? $data->invoice_zipcode : '',
  );
  $form['box7']['invoice_en_fullpayment'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Full Payment?'),
    '#description' => t('Please check to enable full payment for this invoice.'),
    '#default_value' => isset($data->invoice_en_fullpayment) ? $data->invoice_en_fullpayment : 1,
  );
   $form['box7']['invoice_en_partial_payment'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Partial Payment?'),
    '#description' => t('Allow partial payment other than full amount'),
    '#default_value' => isset($data->invoice_en_partial_payment) ? $data->invoice_en_partial_payment : 1,
  );
  $form['box7']['invoice_en_suppress_dates'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable payment plan dates'),
    '#description' => t('Please check to show payment plan dates if output parameter "suppress_payment_plan_details" is = "Y".'),
    '#default_value' => isset($data->invoice_en_suppress_dates) ? $data->invoice_en_suppress_dates : 0,
  );
    $form['box7']['invoice_en_suppress_amount'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable payment plan amount'),
    '#description' => t('Please check to show payment plan amount if output parameter "suppress_payment_plan_details" is = "Y".'),
    '#default_value' => isset($data->invoice_en_suppress_amount) ? $data->invoice_en_suppress_amount : 0,
  );
  $form['box7']['invoice_hide_enter_amount'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide Enter Amount?'),
    '#description' => t('Please check to hide payment box for this invoice.'),
    '#default_value' => isset($data->invoice_hide_enter_amount) ? $data->invoice_hide_enter_amount : 0,
  );
  $form['box7']['invoice_grey_enter_amount'] = array(
    '#type' => 'checkbox',
    '#title' => t('Grayed Enter Amount?'),
    '#description' => t('Please check to Grayed Enter Amount box for this invoice.'),
    '#default_value' => isset($data->invoice_grey_enter_amount) ? $data->invoice_grey_enter_amount : 0,
  );
  $form['box7']['box8'] = array(
    '#type' => 'fieldset',
    '#title' => 'Terms And Conditions',
    '#attributes' => array('class' => array('stp-legend-heading')),
  );
  $form['box7']['box8']['invoice_en_terms'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Terms & Conditions?'),
    '#description' => t('Please check to enable Terms & Conditions for this invoice.'),
    '#default_value' => isset($data->invoice_en_terms) ? $data->invoice_en_terms : 0,
  );
  $form['box7']['box8']['invoice_en_terms_pdf'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Terms & Conditions For PDF?'),
    '#description' => t('Please check to enable Terms & Conditions for this invoice on PDF.'),
    '#default_value' => isset($data->invoice_en_terms_pdf) ? $data->invoice_en_terms_pdf : 0,
  );
  $form['box7']['box8']['invoice_terms_prefix_text'] = array(
    '#type' => 'textfield',
    '#rows' => 1,
    '#default_value' => isset($data->invoice_terms_prefix_text) ? $data->invoice_terms_prefix_text : t('I Accept'),
    '#title' => t('Terms & Conditions Prefix Text'),
    '#description' => t('Please enter prefix text for terms and conditions'),
  );
  $form['box7']['box8']['invoice_terms_header'] = array(
    '#type' => 'textfield',
    '#rows' => 1,
    '#default_value' => isset($data->invoice_terms_header) ? $data->invoice_terms_header : t('Terms & Conditions'),
    '#title' => t('Terms & Conditions Title'),
    '#description' => t('Please enter title for terms and conditions'),
  );
  $form['box7']['box8']['invoice_terms_page_link'] = array(
    '#type' => 'textfield',
    '#rows' => 1,
    '#default_value' => isset($data->invoice_terms_page_link) ? $data->invoice_terms_page_link : t(''),
    '#title' => t('Terms & Conditions Link'),
    '#description' => t('Please enter path alias of terms and conditions page'),
  );
  $form['box7']['box8']['invoice_terms_suffix_text'] = array(
    '#type' => 'textfield',
    '#rows' => 1,
    '#default_value' => isset($data->invoice_terms_suffix_text) ? $data->invoice_terms_suffix_text : t(''),
    '#title' => t('Terms & Conditions Suffix Text'),
    '#description' => t('Please enter suffix text for terms and conditions'),
  );
  $form['box7']['box8']['stp_terms_filter'] = array(
    '#type' => 'radios',
    '#title' => t(''),
    '#options' => array('0' => 'Open T&C in New Tab', '1' => 'Open T&C in Overlay'),
    '#default_value' => isset($data->stp_terms_filter) ? $data->stp_terms_filter : 0,
  );
  $terms =  t(variable_get('invoice_terms', ''));
  $form['box7']['box8']['invoice_terms'] = array(
    '#type' => 'text_format',
    '#rows' => 20,
    '#default_value' => isset($data->invoice_terms) && $data->invoice_terms ? $data->invoice_terms : $terms['value'],
    '#format' => 'full_html',
    '#title' => t('Terms & Conditions'),
    '#description' => t('Please enter terms & conditions for invoice.'),
  );
  
  
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#title' => t('Submit'),
    '#value' => t('Submit'),
  );
  return $form;
}

//Function callbacks for typeform automation
function typeform_ajax_callback($form, &$form_state) {
  return $form['box2'];
}

function message_ajax_callback($form, &$form_state) {
  return $form['box5'];
}



function custom_invoice_admin_invoice_form_submit($form, &$form_state, $id = NULL) {
  $data = $form_state['values'];
  $headings = array('TQTY', 'DESCRIPTION', 'SECTION', 'ROW', 'SEATS', 'PRICE PER ITEM', 'OTHER FIELD', 'CALCULATED FIELD', 'TOTAL COST');
  for ($i =0; $i<=8;$i++) {
    $invoice_details[] = array('heading' =>  $headings[$i], 'title' => $data['title_' . $i], 'field' => $data['field_' . $i], 'class' => $data['class_' . $i], 'weight' => $data['weight_' . $i], 'enable' => $data['enable_' . $i], 'id' => $data['id_' . $i], 'mob_enable' => $data['mob_enable_' . $i]);
    unset($data['title_' . $i]);
    unset($data['field_' . $i]);
    unset($data['class_' . $i]);
    unset($data['weight_' . $i]);
    unset($data['enable_' . $i]);
    unset($data['id_' . $i]);
    unset($data['mob_enable_' . $i]);
  }
  $typeform_data = $data['box2'];
  $success_msg = $data['box5'];
  unset($data['box2']);
  unset($data['box5']);
  unset($data['submit']);
  unset($data['form_build_id']);
  unset($data['form_token']);
  unset($data['form_id']);
  unset($data['op']);
  $data['invoice_terms'] = t($data['invoice_terms']['value']);
  $data['invoice_other_info_text'] = t($data['invoice_other_info_text']['value']);
  if (arg(4) && arg(4) > 0) {
    db_merge('stp_invoice')->key(array('id' => arg(4)))->fields($data)->execute();
    $inv_conf_id = arg(4);
  }
  else {
    $inv_conf_id = db_insert('stp_invoice')->fields($data)->execute();
  }
  foreach ($invoice_details as $invoice) {
    if (isset($invoice['id']) && $invoice['id']) {
      $inv_id = $invoice['id'];
      unset($invoice['id']);
      db_merge('stp_invoice_details')->key(array('inv_conf_id' => $inv_conf_id, 'id' => $inv_id))->fields($invoice)->execute();
    }
    else {
      $invoice['inv_conf_id'] = $inv_conf_id;
      unset($invoice['id']);
      db_insert('stp_invoice_details')->fields($invoice)->execute();
    }
  }
  // Typeform Data
  //$typeform_data = $data['box2'];
  db_delete('stp_invoice_typeform')
   ->condition('inv_conf_id', arg(4))
   ->execute();
   foreach ($typeform_data as $type_form) {
    $accnt_grps = implode(',', $type_form['account_type']);
    if (isset($type_form['typeform_nid']) && (int)$type_form['typeform_nid']) {
     db_merge('stp_invoice_typeform')
      ->key(array('inv_conf_id' => $key))
      ->fields(array(
        'inv_conf_id' => arg(4),
        'acct_grp' => $accnt_grps,
        'typeform_nid' => $type_form['typeform_nid'],
      ))->execute();
    }
  }
  unset($success_msg['add_message']);
  // Data for success message
   db_delete('stp_invoice_thankyou')
    ->condition('inv_conf_id', arg(4))
    ->execute();
   foreach ($success_msg as $success) {
   $accnt_grp = implode(',', $success['account_grp']);
     db_merge('stp_invoice_thankyou')
      ->key(array('inv_conf_id' => $key))
      ->fields(array(
        'inv_conf_id' => arg(4),
        'acct_grp' => $accnt_grp,
        'message' => $success['message']['value'],
      ))->execute();
  }
  //$typeform_data = $data[];
  cache_clear_all('invoice-' . $inv_conf_id, 'cache_custom', $wildcard = FALSE);
   cache_clear_all('invoice-mob-' . $inv_conf_id, 'cache_custom', $wildcard = FALSE);
  cache_clear_all('stp-typeform-' . $inv_conf_id, 'cache_custom', $wildcard = FALSE);
  cache_clear_all('stp-success-' . $inv_conf_id, 'cache_custom', $wildcard = FALSE);
  drupal_goto('admin/manage/settings/invoice/list');
}

/**
 * Function to clone a invoice
 */
function custom_invoice_admin_clone_invoice($invoice_id) {
  $query = db_select('stp_invoice', 'n');
  $query->fields('n');
  $query->condition('n.id', $invoice_id);
  $data = $query->execute()->fetchAssoc();
  if (count($data)) {
    unset($data['id']);
    $data['invoice_title'] = 'Clone of ' . $data['invoice_title'];
    db_insert('stp_invoice')->fields($data)->execute();
    //print_r($data);die;
    $query1 = db_select('stp_invoice_details', 'n');
    $query1->fields('n');
    $query1->condition('n.inv_conf_id', $invoice_id);
    $invoice_details = $query1->execute()->fetchAll();
    foreach ($invoice_details as $key => $invoice_data) {
      $invoice_data = (array)$invoice_data;
      unset($invoice_data['id']);
      db_insert('stp_invoice_details')->fields($invoice_data)->execute();
    }
    drupal_set_message('Invoice has been cloned successfully.');
  }
drupal_goto('admin/manage/settings/invoice/list');
}

/*
 * Function to Clone the node
 */
function custom_invoice_admin_clone_message($nid) {
  $destination = drupal_get_destination();
  $node = node_load($nid);
  unset($node->nid);
  unset($node->vid);
  unset($node->path);
  node_save($node);
  drupal_goto($destination);
}

/**
 * Function to fetch all nodes for email_template type 
 * @return associative array of node id and type.
 */
function custom_invoice_fetch_template_list($key_type = NULL) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', 'email_template')
      ->fieldCondition('field_template_type', 'value', $key_type, '=')
      ->propertyCondition('status', 1);
  $result = $query->execute();
  $template = array('select');
  if (!empty($result['node'])) {
    $nids = array_keys($result['node']);
    foreach ($nids as $nid) {
      $node = node_load($nid, NULL, TRUE);
//      $template[$node->nid] = $node->field_template_type['und'][0]['value'];
      $template[$node->nid] = $node->title;
    }
  }
  return $template;
}
/*
 * Admin Preview 
 */
function custom_invoice_admin_preview($nid, $hook, $vars) {
  $output = '';
  if (isset($nid) && $nid != 0) {
    $output = node_load($nid);
  }
  print theme('custom_preview_node', array('node' => $output));
//      print theme('mimemail_message__stpmail_template.tpl', array('node' => $output));
}

/*
 * Setting form for Payment Methods
 */
function custom_invoice_payment_methods() {
  $form = array();
    $form['amex_card'] = array(
    '#type' => 'checkbox',
    '#title' => t('American Express'),
    '#description' => t('Enable American Express card'),
    '#default_value' => variable_get('amex_card'),
  );
    $form['discover_card'] = array(
    '#type' => 'checkbox',
    '#title' => t('Discover'),
    '#description' => t('Enable Discover card'),
    '#default_value' => variable_get('discover_card'),
  );
    $form['master_card'] = array(
    '#type' => 'checkbox',
    '#title' => t('MasterCard'),
    '#description' => t('Enable Master card'),
    '#default_value' => variable_get('master_card'),
  );
    $form['visa_card'] = array(
    '#type' => 'checkbox',
    '#title' => t('Visa'),
    '#description' => t('Enable Visa card'),
    '#default_value' => variable_get('visa_card'),
  );
  return system_settings_form($form);
}
function custom_invoice_admin_list_page() {
  $form = array();
  $form['inv_list_page_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Page Title'),
    '#description' => t('Please enter the page title for invoice list page'),
    '#default_value' => variable_get('inv_list_page_title', 'My Invoices'),
  );
  $form['inv_list_page_pay'] = array(
    '#type' => 'textfield',
    '#title' => t('Pay button Label'),
    '#description' => t('Please enter the page title for invoice list page'),
    '#default_value' => variable_get('inv_list_page_pay', 'Pay'),
  );
  $form['inv_list_page_print'] = array(
    '#type' => 'textfield',
    '#title' => t('Print Button Label'),
    '#description' => t('Please enter the page title for invoice list page'),
    '#default_value' => variable_get('inv_list_page_print', 'Print'),
  );
  $form['inv_list_date_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Label for DUE Date Heading'),
    '#description' => t('Please enter the heading for the date heading e.g. Due Date (MM/DD/YYYY)'),
    '#default_value' => variable_get('inv_list_date_title', 'DUE DATE (MM/DD/YYYY)'),
  );
  $form['inv_list_balance_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Label for Balance Due Heading'),
    '#description' => t('Please enter the page title for invoice list page'),
    '#default_value' => variable_get('inv_list_balance_title', 'BALANCE DUE'),
  );
  $form['inv_list_date_format'] = array(
    '#type' => 'select',
    '#options' => array('m/d/Y' => 'MM/DD/YYYY', 'd/m/Y' => 'DD/MM/YYYY'),
    '#title' => t('Date Format'),
    '#description' => t('Please enter the date format'),
    '#default_value' => variable_get('inv_list_date_format', 'm/d/Y'),
  );
  $form['inv_footer_copyright'] = array(
    '#type' => 'textarea',
    '#title' => t('Print Invoice footer copyright text'),
    '#description' => t('Please enter the copyright text for print invoice'),
    '#default_value' => variable_get('inv_footer_copyright', ' Team Name &nbsp;I&nbsp; Address &nbsp;I&nbsp; City, State, Zip &nbsp;I&nbsp; Phone Number &nbsp;I&nbsp; Website'),
  );
  return system_settings_form($form);
}

function custom_invoice_admin_manage_invoice_css(){
  iom_color_picker_add_reqired_js();
  $validate =  array(      
    'file_validate_extensions' => array('png jpg jpeg gif'),
    // Pass the maximum file size in bytes
    'file_validate_size' => array(1024*1024*2),
  );
  $gradient_picker = 'To update/create the gradient color <a href="http://www.colorzilla.com/gradient-editor"  target="_blank"><span>Click Here</span></a>';

  $fields_mob = custom_invoice_get_brands_fields_invoice();
  foreach ($fields_mob as $key => $val){    
    //Dynamically create blank variable;
    $$val = '';
  }
  //Get data from database;
  $select = db_select('tbl_manage_branding_invoice', 'tbs');  
  $select->fields('tbs');//We need all field always.
  $select->orderBy('id', 'DESC');
  $select->range(0,1);
  $results = $select->execute();  
   foreach ($results as $robj){
    $row = (array)$robj;
    foreach ($fields_mob as $key1 => $val1){     
      //Dynamically create real variable;
      if (in_array($val1, array_keys($row))){
        $$val1 = $row[$val1];
      }
    }    
     
   }
   $form['id'] = array(
    '#type' => 'hidden',
    '#default_value' => $id,
  );
  $form['box1'] = array(
    '#type' => 'fieldset',
    '#title' => '',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['box1']['site_title_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Top Header Page Title Text Color',
    '#default_value' => $site_title_color ? $site_title_color : '#ffffff',
    '#attributes' => array('id' => "site_title_color"),
  );

  $form['invoice_list'] = array(
    '#type' => 'fieldset',
    '#title' => 'Invoice List',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['invoice_list']['invoice_list_header_bg_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Table Header BG Color',
    '#default_value' => $invoice_list_header_bg_color ? $invoice_list_header_bg_color : '#522b1c',
    '#attributes' => array('id' => "invoice_list_header_bg_color"),
  );
  $form['invoice_list']['invoice_list_text_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Table Header Text Color',
    '#default_value' => $invoice_list_text_color ? $invoice_list_text_color : '#ffffff',
    '#attributes' => array('id' => "invoice_list_text_color"),
  );
  $form['invoice_list']['email_buttons_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Pay/Print/Email Button Text Color',
    '#default_value' => $email_buttons_color ? $email_buttons_color : '#ffffff',
    '#attributes'=> array('id'=>"email_buttons_color"),
  );
  $form['invoice_list']['email_button_bg_hover'] = array(
    '#type' => 'textfield',
    '#title' => t('Pay/Print/Email button BG hover'),    
    '#default_value' => $email_button_bg_hover ? $email_button_bg_hover : '#030303',
    '#required' => TRUE,
    '#attributes'=>array('id'=>'email_button_bg_hover'),
  );
  $form['invoice_list']['email_button_border'] = array(
    '#type' => 'textfield',
    '#title' => t('Pay/Print/Email button border color'),    
    '#default_value' => $email_button_border ? $email_button_border : '#030303',
    '#required' => TRUE,
    '#attributes'=>array('id'=>'email_button_border'),
  );
  $form['invoice_list']['email_button_box_shadow'] = array(
    '#type' => 'textfield',
    '#title' => t('Pay/Print/Email button shadow color'),    
    '#default_value' => $email_button_box_shadow ? $email_button_box_shadow : '#5b5b5b',
    '#required' => TRUE,
    '#attributes'=>array('id'=>'email_button_box_shadow'),
  );
  $gradiant_email_bg = "url('data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0idXRmLTgiPz4gPHN2ZyB2ZXJzaW9uPSIxLjEiIHhtbG5zPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyI+PGRlZnM+PGxpbmVhckdyYWRpZW50IGlkPSJncmFkIiBncmFkaWVudFVuaXRzPSJvYmplY3RCb3VuZGluZ0JveCIgeDE9IjAuNSIgeTE9IjAuMCIgeDI9IjAuNSIgeTI9IjEuMCI+PHN0b3Agb2Zmc2V0PSIwJSIgc3RvcC1jb2xvcj0iIzYyNjI2MiIvPjxzdG9wIG9mZnNldD0iMjMlIiBzdG9wLWNvbG9yPSIjNGU0ZTRlIi8+PHN0b3Agb2Zmc2V0PSI2MiUiIHN0b3AtY29sb3I9IiMyMDIwMjAiLz48c3RvcCBvZmZzZXQ9IjY5JSIgc3RvcC1jb2xvcj0iIzFhMWExYSIvPjxzdG9wIG9mZnNldD0iODclIiBzdG9wLWNvbG9yPSIjMDcwNzA3Ii8+PHN0b3Agb2Zmc2V0PSI5MiUiIHN0b3AtY29sb3I9IiMwMzAzMDMiLz48c3RvcCBvZmZzZXQ9IjEwMCUiIHN0b3AtY29sb3I9IiMwNDA0MDQiLz48L2xpbmVhckdyYWRpZW50PjwvZGVmcz48cmVjdCB4PSIwIiB5PSIwIiB3aWR0aD0iMTAwJSIgaGVpZ2h0PSIxMDAlIiBmaWxsPSJ1cmwoI2dyYWQpIiAvPjwvc3ZnPiA=');
  background-size: 100%;
  background-image: -webkit-gradient(linear, 50% 0%, 50% 100%, color-stop(0%, #626262), color-stop(23%, #4e4e4e), color-stop(62%, #202020), color-stop(69%, #1a1a1a), color-stop(87%, #070707), color-stop(92%, #030303), color-stop(100%, #040404));
  background-image: -moz-linear-gradient(top, #626262 0%, #4e4e4e 23%, #202020 62%, #1a1a1a 69%, #070707 87%, #030303 92%, #040404 100%);
  background-image: -webkit-linear-gradient(top, #626262 0%, #4e4e4e 23%, #202020 62%, #1a1a1a 69%, #070707 87%, #030303 92%, #040404 100%);
  background-image: linear-gradient(to bottom, #626262 0%, #4e4e4e 23%, #202020 62%, #1a1a1a 69%, #070707 87%, #030303 92%, #040404 100%);";
  $form['invoice_list']['email_button_bg_gradiant'] = array(
    '#type' => 'textarea',
    '#title' => t('Pay/Print/Email button Gradient'). ' ('.$gradient_picker.')',
    '#default_value' => $email_button_bg_gradiant ? $email_button_bg_gradiant : $gradiant_email_bg,
    '#required' => TRUE,
  );
 $form['box2'] = array(
    '#type' => 'fieldset',
    '#title' => 'Invoice Details',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
 $form['box2']['order_invoice_watermark'] = array(
    '#type' => 'managed_file',
    '#title' => t('Invoice Watermark Logo'),
    '#description' => t('Please upload watermark.'),
    '#default_value' => variable_get('order_invoice_watermark', ''),
     '#upload_validators' => $validate,
    '#upload_location' => 'public://invoice/',
    '#file_upload_description' => theme('file_upload_help', array('upload_validators' => $validate)),
  );
 $form['box2']['invoice_reps_bg'] = array(
    '#type' => 'textfield',
    '#title' => t('Reps Box BG Color'),
    '#description' => t('Please enter Invoice page color whichrelates to .infoPanelwrap .dedicateserrep class'),
    '#default_value' => $invoice_reps_bg ? $invoice_reps_bg : '#000000',
    '#required' => TRUE,
    '#attributes' => array('id' => 'invoice_reps_bg'),
  );
  $form['box2']['invoice_page_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Heading text and table header color'),
    '#description' => t('Please enter text color for Account ID, Rep Name, th,h2, h3 tags on invoice detail page'),
    '#default_value' => $invoice_page_color ? $invoice_page_color : '#522b1c',
    '#required' => TRUE,
    '#attributes' => array('id' => 'invoice_page_color'),
  );
  $form['box2']['buttons_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Pay/Continue/Submit Button Text Color',
    '#default_value' => $buttons_color ? $buttons_color : '#ffffff',
    '#attributes'=> array('id'=>"buttons_color"),
  );
  
  $form['box2']['buttons_border_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Pay/Continue/Submit Button Border color',
    '#default_value' => $buttons_border_color ? $buttons_border_color : 'rgba(25, 28, 31, 0.8)',
    '#attributes'=> array('id' => "buttons_border_color"),
  );  
  $form['box2']['dropdown_arrow_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Card Type Dropmenu Arrow Color',
    '#default_value' => $dropdown_arrow_color ? $dropdown_arrow_color :'#522b1c',
    '#attributes'=> array('id' => "dropdown_arrow_color"),
  );
  $form['box2']['buttons_back_hover_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Pay/Continue/Submit Hover BG Color',
    '#default_value' => $buttons_back_hover_color ? $buttons_back_hover_color : '#5b4810',
    '#attributes'=> array('id' => "buttons_back_hover_color"),
  );
  $form['box2']['buttons_back_hover_border_color'] = array(
    '#type' => 'textfield', 
    '#title' => 'Pay/Continue/Submit Border hover Color',
    '#default_value' => $buttons_back_hover_border_color ? $buttons_back_hover_border_color : 'rgba(0, 0, 0, 0.75)',
    '#attributes'=> array('id' => "buttons_back_hover_border_color"),
  );

$default_buttons_gradiant_def = "background-image: url('data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0idXRmLTgiPz4gPHN2ZyB2ZXJzaW9uPSIxLjEiIHhtbG5zPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyI+PGRlZnM+PGxpbmVhckdyYWRpZW50IGlkPSJncmFkIiBncmFkaWVudFVuaXRzPSJvYmplY3RCb3VuZGluZ0JveCIgeDE9IjAuNSIgeTE9IjAuMCIgeDI9IjAuNSIgeTI9IjEuMCI+PHN0b3Agb2Zmc2V0PSIwJSIgc3RvcC1jb2xvcj0iI2U0YjUyYiIvPjxzdG9wIG9mZnNldD0iMTklIiBzdG9wLWNvbG9yPSIjZTRiNTJiIi8+PHN0b3Agb2Zmc2V0PSIyNCUiIHN0b3AtY29sb3I9IiNlMWIxMjkiLz48c3RvcCBvZmZzZXQ9IjI3JSIgc3RvcC1jb2xvcj0iI2RjYWQyOSIvPjxzdG9wIG9mZnNldD0iMzIlIiBzdG9wLWNvbG9yPSIjZDBhNDI3Ii8+PHN0b3Agb2Zmc2V0PSI0MSUiIHN0b3AtY29sb3I9IiNiYTkyMjEiLz48c3RvcCBvZmZzZXQ9IjQ2JSIgc3RvcC1jb2xvcj0iI2E4ODUyMSIvPjxzdG9wIG9mZnNldD0iNjIlIiBzdG9wLWNvbG9yPSIjNzY1ZTE2Ii8+PHN0b3Agb2Zmc2V0PSI3MyUiIHN0b3AtY29sb3I9IiM1ZTRiMTEiLz48c3RvcCBvZmZzZXQ9Ijc2JSIgc3RvcC1jb2xvcj0iIzViNDgxMCIvPjxzdG9wIG9mZnNldD0iMTAwJSIgc3RvcC1jb2xvcj0iIzViNDgxMCIvPjwvbGluZWFyR3JhZGllbnQ+PC9kZWZzPjxyZWN0IHg9IjAiIHk9IjAiIHdpZHRoPSIxMDAlIiBoZWlnaHQ9IjEwMCUiIGZpbGw9InVybCgjZ3JhZCkiIC8+PC9zdmc+IA==');
  background-size: 100%;
    background-image: -webkit-gradient(linear, 50% 0%, 50% 100%, color-stop(0%, #e4b52b), color-stop(19%, #e4b52b), color-stop(24%, #e1b129), color-stop(27%, #dcad29), color-stop(32%, #d0a427), color-stop(41%, #ba9221), color-stop(46%, #a88521), color-stop(62%, #765e16), color-stop(73%, #5e4b11), color-stop(76%, #5b4810), color-stop(100%, #5b4810));
    background-image: -moz-linear-gradient(top, #e4b52b 0%, #e4b52b 19%, #e1b129 24%, #dcad29 27%, #d0a427 32%, #ba9221 41%, #a88521 46%, #765e16 62%, #5e4b11 73%, #5b4810 76%, #5b4810 100%);
    background-image: -webkit-linear-gradient(top, #e4b52b 0%, #e4b52b 19%, #e1b129 24%, #dcad29 27%, #d0a427 32%, #ba9221 41%, #a88521 46%, #765e16 62%, #5e4b11 73%, #5b4810 76%, #5b4810 100%);
    background-image: linear-gradient(to bottom, #e4b52b 0%, #e4b52b 19%, #e1b129 24%, #dcad29 27%, #d0a427 32%, #ba9221 41%, #a88521 46%, #765e16 62%, #5e4b11 73%, #5b4810 76%, #5b4810 100%);";
  $form['box2']['buttons_default_color_gradiant'] = array(
    '#type' => 'textarea',
    '#title' => 'Pay/Continue/Submit Button BG gradient Color ('.$gradient_picker.')',
    '#default_value' => $buttons_default_color_gradiant ? $buttons_default_color_gradiant : $default_buttons_gradiant_def,
  );
  $form['box3'] = array(
    '#type' => 'fieldset',
    '#title' => t('Print Invoice'),
    '#weight' => 3,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,    
  );
  
  
  $form['box3']['invoice_logo_fid'] = array(
    '#type' => 'managed_file',
    '#title' => t('Invoice Logo'),
    '#description' => t('Please enter subject for invoice mail.'),
    '#default_value' => variable_get('invoice_logo_fid', ''),
    //'#required' => TRUE,
    '#upload_location' => 'public://invoice/'
  );
  $form['box3']['invoice_watermark_fid'] = array(
    '#type' => 'managed_file',
    '#title' => t('Invoice Watermark Logo'),
    '#description' => t('Please upload watermark for invoice.'),
    '#default_value' => variable_get('invoice_watermark_fid', ''),
     '#upload_validators' => $validate,
    '#upload_location' => 'public://invoice/',
    '#file_upload_description' => theme('file_upload_help', array('upload_validators' => $validate)),
  ); 
  $form['box3']['invoice_team_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Invoice Teamname'),
    '#description' => t('Please enter Team name for invoice.'),
    '#default_value' => variable_get('invoice_team_name', t('STP')),
    '#required' => TRUE,
  );
  $form['box3']['invoice_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Invoice Colors - Lines and Headers'),
    '#description' => t('Please enter HEX color code for lines and headers.'),
    '#default_value' => variable_get('invoice_color', t('#000000')),
    '#attributes' => array('id' => "invoice_color"),
    '#required' => TRUE,    
  );
  $form['box4'] = array(
    '#type' => 'fieldset',
    '#title' => t('Additional'),
    '#weight' => 4,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,        
  );
  $form['box4']['addtional_css_overwrite'] = array(
    '#type' => 'textarea',
    '#title' => t('Additional CSS for overwrite on specific changes'),
    '#default_value' => $addtional_css_overwrite ? $addtional_css_overwrite : '',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#weight' => 4,
    '#value' => 'Submit',
    );

  return $form;

  // Perform our 'custom submit' before system submit
  //$form['#submit'][] = 'custom_invoice_admin_manage_invoice_css_form_submit';
  //return system_settings_form($form);
}

/**
 * Please don't change the function name, since function is utilized in more area.
 */
function custom_invoice_admin_manage_invoice_css_submit($form, &$form_state) {  
  global $user;  
  //Make file object and save permanent
  $files = array();
  $files['invoice_logo_fid'] = file_load($form_state['values']['invoice_logo_fid']);
  $files['invoice_watermark_fid'] = file_load($form_state['values']['invoice_watermark_fid']);
  $files['order_invoice_watermark'] = file_load($form_state['values']['order_invoice_watermark']);

  variable_set('invoice_team_name', $form_state['values']['invoice_team_name']);
  variable_set('invoice_color', $form_state['values']['invoice_color']);
  $f_record = array();
  foreach ($files as $key => $file) {
    variable_set($key, '');
     if ($file) {
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      $f_record[$key] = $file->fid;
      variable_set($key, $file->fid);
      file_usage_add($file, 'iom_color_invoice', $key, $file->fid);
     }
  }
  $brand_fields = custom_invoice_get_brands_fields_invoice();
  $record = array();
  foreach ($brand_fields as $key => $val){
    //Dynamically create blank variable;
    if (in_array($val, array_keys($form_state['values']))) {
      $record[$val] = $form_state['values'][$val] ? $form_state['values'][$val] : '';
    }
  }
  $record['uid'] = $user->uid;
  $record['timestamp'] = $record['timestamp'] ? $record['timestamp'] : time();
  
   $key_id = array();
   if (isset($record['id']) && $record['id']) {
     $key_id = 'id';
     //if you want to keep the last updated and new record will be created always uncomment below two lines.
     //$key_id = array();
     //unset($record['cid']);     
   }
   
  $file_created = '';
  $result = drupal_write_record('tbl_manage_branding_invoice', $record, $key_id);  
  if ($result) {
    $file_created = custom_invoice_auto_create_css($form_state['values']);   
  }
  
  switch($result){
    case 1:
      drupal_set_message(t('A new record has been created successfully'));
      if ($file_created){
       drupal_set_message(t('A new Invoice css has been added successfully'));
      }
    break;
    case 2:
      drupal_set_message(t('Record has been updated successfully'));
      if ($file_created){
       drupal_set_message(t('Invoice css has been updated successfully'));
      }
    break;
    default:
      drupal_set_message(t('Submission failed!'), 'error');
   }
}

/**
 * //These fields are related to db field.
 * Purpose: Needs these field to display current value in the text area/text fields,
   also required for initialize of variable before display to.
 */
function custom_invoice_get_brands_fields_invoice(){ 
 $brand_fields = array(
                'id',
                'site_title_color',
                'invoice_list_header_bg_color',
                'invoice_page_color',
                'invoice_reps_bg',
                'invoice_list_text_color',
                'buttons_color',
                'buttons_border_color',
                'dropdown_arrow_color',
                'buttons_back_hover_color',
                'buttons_back_hover_border_color',
                'buttons_default_color_gradiant',
                'email_buttons_color',
                'email_button_bg_gradiant',
                'email_button_bg_hover',
                'email_button_border',
                'email_button_box_shadow',        
                'timestamp',
                'addtional_css_overwrite',
                'status',
                'uid',
                );
  return $brand_fields;
}

/**
 * Hook_form_submit().
 * Purpose: get the submitted value and create a css named:"mobile_css.css"
 * @return: $file_created, TRUE OR FALSE.
 */
function custom_invoice_auto_create_css($form_state_value){
     global $user;
     if (!$form_state_value){
      return FALSE;
     }

    //key is the tables col and value are css variables will be write in css.
    $path = drupal_get_path('module', 'custom_invoice');    
    $filename = $path ."/auto_css.php";
    
    $handle = fopen($filename, "r");
    if (FALSE === $handle) {
      exit("Failed to open stream to URL");
    }

   //mapping form field (key) and mobile_css.php field (value)
   //which will be replace by...
   //name and key may not same.
    $css_variables = custom_invoice_get_brands_fields_invoice();
    $ct = 1;
    $number_of_var = count($css_variables);

    //$images_in_css = array('order_invoice_watermark');
    
     if (variable_get('order_invoice_watermark')){        
        $file = file_load(variable_get('order_invoice_watermark'));
        $file_full_path = file_create_url($file->uri);
        $order_invoice_watermark = $file_full_path;
      }
    foreach ($css_variables as $key => $var) {
      $color = $form_state_value[$var];
      $val1 = $var;
      $$val1 = $color; //Create real variable to replace php variable in css var.
      if ($var == 'timestamp'){
        $timestamp = date('Y-m-d h:i:s A');
      }
      if ($var == 'uid'){
        $uid = $user->name;
      }
      
      /*if (in_array($var, $images_in_css) && $color){        
        $file = file_load($color);
        $file_full_path = file_create_url($file->uri);
        $$val1 = $file_full_path;
      }*/
            
      if ($number_of_var == $ct){
        $filename = "auto_css.php";
        include_once $filename;
        $updated_css = $invoice_css; //$css variable defined in auto_css.php
      }
      $ct++;
    }
        
    $path_to_public_file = iom_color_picker_get_files_path();    
    $fp = fopen($path_to_public_file."/invoice_automation.css", 'w+');
    $file_opened = TRUE;
    if (FALSE === $fp) {
      $file_opened = FALSE;
      drupal_set_message(t('Failed to open stream to URL and css unable to update!'));
    }
    else if ($file_opened){
       fwrite($fp, $updated_css);
       cache_clear_all();
       fclose($fp);
     }
    fclose($fp);  
    return $file_opened;
}
