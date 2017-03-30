<?php
/**
 * Type-form admin settings.
 */
function custom_typeform_settings() {
  $form = array();
  $form['type_form_api'] = array(
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#default_value' => variable_get('type_form_api', ''),
    '#description' => t('Please enter API Key details'),
    '#required' => true,
  );
  $form['type_form_weburl'] = array(
    '#type' => 'textfield',
    '#title' => t('Web Url'),
    '#default_value' => variable_get('type_form_weburl', ''),
    '#description' => t('Please enter web url to retrieve typeform data'),
    '#required' => true,
  );
  $form['typeform_root_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Root Typeform Url'),
    '#default_value' => variable_get('typeform_root_url', ''),
    '#description' => t('Please enter root typeform url. This settings will restrict editor to add only the typeform with root url.'),
    '#required' => true,
  );
   // Query to get all avialable typeforms
  $query= db_query("SELECT * FROM {node} WHERE type = 'typeform' and status = 1");
  $result = $query->fetchAll();
  $node_title = array();
  foreach ( $result as $record ) {
    $node_title[$record->nid] = $record->title;
  }

  // Daily Typefrom reports
  $form['box1'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('stp-legend-heading')),
  );
  $form['box1']['daily'] = array(
    '#type' => 'checkbox',
    '#title' => t('Daily'),
    '#description' => t('Please enable checkbox to send daily reports.'),
    '#default_value' => variable_get('daily', 0),
  );
  $form['box1']['daily_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Time'),
	'#default_value' => variable_get('daily_time', ''),
    '#description' => t('Please enter daily time to send typeform reports'),
  );
  $form['box1']['daily_recipients'] = array(
   '#type' => 'textarea',
   '#title' => t('Recipients'),
   '#rows' => 5,
   '#description' => t("Please enter Recipient's email addresses. You can add multiple email addresses separated by ','."),
   '#default_value' => variable_get('daily_recipients'),
   //'#required' => TRUE,
 );
  $form['box1']['daily_typeform'] = array(
    '#type' => 'select',
    '#title' => t('Available Typefrom'),
    '#chosen' => TRUE,
    '#multiple' => TRUE,
    '#options' => $node_title,
	'#default_value' => variable_get($node_title),
    );
 
  // Weekly Typefrom reports
  $form['box2'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('stp-legend-heading')),
  );
  $form['box2']['weekly'] = array(
    '#type' => 'checkbox',
    '#title' => t('Weekly'),
    '#description' => t('Please enable checkbox to send weekly reports.'),
    '#default_value' => variable_get('weekly', 0),
  );
   $form['box2']['weekly_day'] = array(
    '#type' => 'textfield',
    '#title' => t('Day'),
    '#description' => t('Please enter weekly date to send weekly reports.'),
    '#default_value' => variable_get('weekly_day', ''),
  );
  $form['box2']['weekly_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Time'),
	'#default_value' => variable_get('weekly_time', ''),
    '#description' => t('Please enter weekly time to send typeform reports'),
  );
  $form['box2']['weekly_recipients'] = array(
   '#type' => 'textarea',
   '#title' => t('Recipients'),
   '#rows' => 5,
   '#description' => t("Please enter Recipient's email addresses. You can add multiple email addresses separated by ','."),
   '#default_value' => variable_get('weekly_recipients'),
   //'#required' => TRUE,
 );
 $form['box2']['weekly_typeform'] = array(
    '#type' => 'select',
    '#title' => t('Available Typefrom'),
    '#chosen' => TRUE,
    '#multiple' => TRUE,
    '#options' => $node_title,
	'#default_value' => variable_get($node_title),
    );
 
  // Monthly Typefrom reports
  $form['box3'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('stp-legend-heading')),
  );
  $form['box3']['monthly'] = array(
    '#type' => 'checkbox',
    '#title' => t('Monthly'),
    '#description' => t('Please enable checkbox to send monthly reports.'),
    '#default_value' => variable_get('monthly', 0),
  );
  $form['box3']['monthly_date'] = array(
    '#type' => 'textfield',
    '#title' => t('Date'),
    '#description' => t('Please enter date to send monthly reports.'),
    '#default_value' => variable_get('monthly_date', ''),
  );
  $form['box3']['monthly_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Time'),
	'#default_value' => variable_get('monthly_time', ''),
    '#description' => t('Please enter monthly time to send typeform reports'),
  );
  $form['box3']['monthly_recipients'] = array(
   '#type' => 'textarea',
   '#title' => t('Recipients'),
   '#rows' => 5,
   '#description' => t("Please enter Recipient's email addresses. You can add multiple email addresses separated by ','."),
   '#default_value' => variable_get('monthly_recipients'),
   //'#required' => TRUE,
 );
 $form['box3']['monthly_typeform'] = array(
    '#type' => 'select',
    '#title' => t('Available Typefrom'),
    '#chosen' => TRUE,
    '#multiple' => TRUE,
    '#options' => $node_title,
    '#default_value' => explode(',', $node_title),
    );
  
  
  return system_settings_form($form);
}

/**
 * function to retrieve the typeform data
 * @param $form_id typeform id
 * @return json response from typeform
 * 
 */
function _custom_tyform_data_retrival($form_id) {
  $typeform_api_secret = variable_get('type_form_api', '2abd9c29fe0aab52defacb75d08bb67817c371e7');
  $typeform_weburl = variable_get('type_form_weburl', '');
  $web_url = $typeform_weburl . '/'.  $form_id . '?key=' . $typeform_api_secret . '&completed=true';
  $ch1 = curl_init();
  $adb_option_defaults = array(
    CURLOPT_VERBOSE         => 1,
    CURLOPT_URL             => $web_url, 
    CURLOPT_CUSTOMREQUEST   => 'GET',
    CURLOPT_POST            => false,
    CURLOPT_POSTFIELDS      => NULL,
    CURLOPT_RETURNTRANSFER  => TRUE,
	//CURLOPT_CAINFO          => 'c:/wamp/bin/php/cacert.pem',
	//CURLOPT_SSL_VERIFYPEER  => true,
	//CURLOPT_SSL_VERIFYHOST  => 2,
  );
  curl_setopt_array($ch1, $adb_option_defaults);
  $response = curl_exec($ch1);
  $info = curl_getinfo($ch1);
  $error= curl_error($ch1);
  $httpcode = curl_getinfo($ch1, CURLINFO_HTTP_CODE);
  $header_size = curl_getinfo($ch1, CURLINFO_HEADER_SIZE);
  curl_close($ch1);
  return array('httpcode' => $httpcode, 'response' => $response, 'error' => $error, 'info' => $info);
}
/**
 * function to retrieve the typeform data and generate CSV file
 * @param $form_id, $email_addresses, typeform id
 * @return json response from typeform
 * 
 */
function _convert_csv_formate($form_id, $email_addresses) {
global $user;
  $data = _custom_tyform_data_retrival($form_id);
  $response_decode = json_decode($data['response'], true);
  $responses = $response_decode['responses'];
  $total_response = $response_decode['stats']['responses']['total'];
  
  $typeform_api_secret = variable_get('type_form_api', '2abd9c29fe0aab52defacb75d08bb67817c371e7');
  $typeform_weburl = variable_get('type_form_weburl', '');
  
  // Loop for CSV header
  foreach($response_decode['questions'] as $key => $header_data) {
    $header[$header_data['id']]=$header_data['question'];
  }
  $headers[] = $header;
  $csv_file= array();
  $comma = t('	');
  $rows[] = $header;
  
  $offset = 0;
  for ($i = 1;$i <= round($total_response/1000) ;$i++) {
	$web_url = $typeform_weburl . '/'.  $form_id . '?key=' . $typeform_api_secret . '&completed=true&offset='.$offset.'&limit=1000';
    // if (empty($records)) break;
    $offset = $offset +1000;
    $ch2 = curl_init();
		$adb_option_defaults = array(
		CURLOPT_VERBOSE         => 1,
		CURLOPT_URL             => $web_url, 
		CURLOPT_CUSTOMREQUEST   => 'GET',
		CURLOPT_POST            => false,
		CURLOPT_POSTFIELDS      => NULL,
		CURLOPT_RETURNTRANSFER  => TRUE,
		);
		curl_setopt_array($ch2, $adb_option_defaults);
		$response = curl_exec($ch2);
		$info = curl_getinfo($ch2);
		$error= curl_error($ch2);
		$httpcode = curl_getinfo($ch2, CURLINFO_HTTP_CODE);
		$header_size = curl_getinfo($ch2, CURLINFO_HEADER_SIZE);
		$response_decode = json_decode($response, true);
		$responses = $response_decode['responses'];
    curl_close($ch2);
    
	// Loop for CSV rows
	foreach($responses as $key=> $data) {
      $row = array();
      foreach ($data['answers'] as $key => $answers) {
	    $row[$key] = $answers;
	    /*if($key == 'yesno_10668101'){
		  $row[$key] = ($answers == 1) ? 'Yes' : 'No';
	    }*/	 
	  }
	  foreach ($data['hidden'] as $key => $hidden) {
	    $row['hidden_' . $key] = $hidden;
	  }
	  $rows[] = $row;
    }
  }
  module_load_include('php','custom_csv_automation', 'Classes/PHPExcel');
  $count = 0;
  $alpha = 65;
  if(count($rows)>0) {
    $objPHPExcel = new PHPExcel();
    $form_data = array_merge($headers,$rows);
    foreach($form_data as $rkey=>$rvalue){
      $alpha = 65;
      foreach($rvalue as $srkey=>$srvalue){
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue(chr($alpha).$count, $srvalue);
        $alpha++;
      }
      $count++;
    }

    $objPHPExcel->getActiveSheet()->setTitle('abc');
    $objPHPExcel->setActiveSheetIndex(0);
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel2007');
    $filename = 'reports_' . date("Y-m-d H:i:s").'.csv';
   // $filename = 'reports_' . rand().'.csv';
	$filepath = file_directory_temp() . '/' . $filename;
	//$filepath = 'sites/stp1.5.2/files/qwe.csv';

    $objWriter->save($filepath);
	
	if($filepath) {
	  $params['file'] = $filepath;
	  $params['filename'] = $filename;
	  $params['subject'] = 'CSV Report';
	  $params['body'] = 'Please find the CSV report of Typeform';
	}
  };
  $mail= $email_addresses;
  drupal_mail('custom_hooks', 'triggerkey', $mail, language_default(), $params);
  drupal_set_message('Report mailed successfully.');
  //drupal_mail('custom_typeform', 'typeform', $mail, language_default(), $params);
}
 
/**
* Convert a value or array of values to csv representation.
*/
function strtocsv(&$object) {
 if (is_array($object)) {
   foreach ($object as &$value) {
     $value = '"' . str_replace('"', '""', decode_entities(strip_tags($value))) . '"';
   }
   return;
 }
 $object = '"' . str_replace('"', '""', decode_entities(strip_tags($object))) . '"';
}


/*
* Function callback to store all typeform data in DB of particula account group
*
*/
function typeform_account_group_data($form_id, $nid) {
  global $user;
  $typeform_api_secret = variable_get('type_form_api', '2abd9c29fe0aab52defacb75d08bb67817c371e7');
  $typeform_weburl = variable_get('type_form_weburl', '');

  //Fetch max id from database table
  $max = db_query('SELECT MAX(response_id) FROM {tbl_typeform_results} WHERE typeform_id = :typeform_id', array(':typeform_id' => $form_id))->fetchField();
  if(isset($max) && $max != '0'){
   $next_max = $max+1;
    $web_url = $typeform_weburl . '/'.  $form_id . '?key=' . $typeform_api_secret . '&completed=true&offset='.$next_max.'&limit=1000';
  } else{
    $web_url = $typeform_weburl . '/'.  $form_id . '?key=' . $typeform_api_secret . '&completed=true&offset=0&limit=1000';
  }
  
  // Curl request started
  $ch2 = curl_init();
  $adb_option_defaults = array(
	CURLOPT_VERBOSE         => 1,
	CURLOPT_URL             => $web_url, 
	CURLOPT_CUSTOMREQUEST   => 'GET',
	CURLOPT_POST            => false,
	CURLOPT_POSTFIELDS      => NULL,
	CURLOPT_RETURNTRANSFER  => TRUE,
  );
  curl_setopt_array($ch2, $adb_option_defaults);
  $response = curl_exec($ch2);
  $info = curl_getinfo($ch2);
  $error= curl_error($ch2);
  $httpcode = curl_getinfo($ch2, CURLINFO_HTTP_CODE);
  $header_size = curl_getinfo($ch2, CURLINFO_HEADER_SIZE);
  $response_decode = json_decode($response, true);
  $responses = $response_decode['responses'];
  curl_close($ch2);
  // Inesert records in database table
  $row = array(); 
  foreach($responses as $key=> $data) {
	$answers = $data['answers'];
	$hidden = $data['hidden'];
	$complete_data = $answers + $hidden;
	$row['data'] = drupal_json_encode($complete_data);
	$row['acct_id'] = (isset($data['hidden']['acct_id']) && $data['hidden']['acct_id']) ? $data['hidden']['acct_id'] : 0;
	$row['response_id'] = isset($max) ? $max+1 : $key;
	$row['typeform_id'] = $form_id;
	db_insert('tbl_typeform_results')->fields($row)->execute();
	
  }
  if(isset($nid) && $nid != ''){
    $node = node_load($nid);
    $lang = isset($node->language) ? $node->language : LANGUAGE_DEFAULT;
	if(isset($node->field_typeform_success_url[$lang][0]['value'])) {
      drupal_goto($node->field_typeform_success_url[$lang][0]['value']);
	}
	else {
	  drupal_goto('home');
	}
  }
  else {
    return TRUE;
  }
}

function typeform_account_thankyou_page($node) {
  return theme('typeform_error_page');
}


/*
* Function callback to retrive typeform reports which is stored DB.
*
*/
function _custom_tyform_data_report($form_id, $email_addresses) {
  $typeform_api_secret = variable_get('type_form_api', '2abd9c29fe0aab52defacb75d08bb67817c371e7');
  $typeform_weburl = variable_get('type_form_weburl', '');;
  $web_url = $typeform_weburl . '/'.  $form_id . '?key=' . $typeform_api_secret . '&completed=true&offset=0&limit=1';
  // Curl Request
  $ch2 = curl_init();
  $adb_option_defaults = array(
	CURLOPT_VERBOSE         => 1,
	CURLOPT_URL             => $web_url, 
	CURLOPT_CUSTOMREQUEST   => 'GET',
	CURLOPT_POST            => false,
	CURLOPT_POSTFIELDS      => NULL,
	CURLOPT_RETURNTRANSFER  => TRUE,
  );
  curl_setopt_array($ch2, $adb_option_defaults);
  $response = curl_exec($ch2);
  $info = curl_getinfo($ch2);
  $error= curl_error($ch2);
  $httpcode = curl_getinfo($ch2, CURLINFO_HTTP_CODE);
  $header_size = curl_getinfo($ch2, CURLINFO_HEADER_SIZE);
  $response_decode = json_decode($response, true);
  $responses = $response_decode['responses'];
  curl_close($ch2);
	
  // Header response
  foreach($response_decode['questions'] as $key => $header_data) {
    $header[$header_data['id']]=$header_data['question'];
  }
  $headers[] = $header;
  $csv_file= array();
  $comma = t('	');
  $rows[] = $header;
  
  // query to fetch data from DB
  $query = db_query("SELECT acct_id, typeform_id, data FROM tbl_typeform_results WHERE typeform_id = :typeform_id", array(':typeform_id' => $form_id));
  $records = $query->fetchAll();
  $row_data = array();
  foreach($records as $result) {
	$row_data[] = json_decode($result->data, true);
  }
  foreach ($row_data as $key => $answers) {
    $rows[] = $answers;  
  }
	 
  // create csv report
  module_load_include('php','custom_csv_automation', 'Classes/PHPExcel');
  $count = 0;
  $alpha = 65;
  if(count($rows)>0) {
    $objPHPExcel = new PHPExcel();
    $form_data = array_merge($headers,$rows);
    foreach($form_data as $rkey=>$rvalue){
      $alpha = 65;
      foreach($rvalue as $srkey=>$srvalue){
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue(chr($alpha).$count, $srvalue);
        $alpha++;
      }
      $count++;
    }

    $objPHPExcel->getActiveSheet()->setTitle('abc');
    $objPHPExcel->setActiveSheetIndex(0);
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel2007');
    
    $filename = 'reports_' . date("Y-m-d H:i:s").'.csv';
	//$filename = 'reports_' . rand().'.csv';
	$filepath = file_directory_temp() . '/' . $filename;
    $objWriter->save($filepath);
	
	if($filepath) {
	  $params['file'] = $filepath;
	  $params['filename'] = $filename;
	  $params['subject'] = 'CSV Report';
	  $params['body'] = 'Please find the CSV report of Typeform';
	}
  }
  $mail= $email_addresses;
  drupal_mail('custom_hooks', 'triggerkey', $mail, language_default(), $params);
  drupal_set_message('Report mailed successfully.');
}