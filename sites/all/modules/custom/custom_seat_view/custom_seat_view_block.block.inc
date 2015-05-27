
<?php
function custom_seat_view_get_html() {
  global $user;
  $acct_id = $user->name;
  if ($acct_id) {
    $seat_holds = get_seats_hold($acct_id);
    $user_info = get_user_info($acct_id);
  }
  $imageFolder = 'images_1024';
  drupal_add_css(drupal_get_path('module', 'custom_seat_view') . '/css/custom_seat_view.css');
  if ($seat_count = count($seat_holds)) {
    //UPDATE tbl_section_data  JOIN tbl_tilt  ON (INSTR(tbl_section_data.vrview_folder, tbl_tilt.`vrview_folder`)>0) SET tbl_section_data.`cameraAngleTiltY` = tbl_tilt.cameraAngleTiltY
    // Adding js files
    drupal_add_js(drupal_get_path('module', 'custom_seat_view') . '/js/jquery.cors.js');
    drupal_add_js(drupal_get_path('module', 'custom_seat_view') . '/js/swfkrpano.js');
    drupal_add_js(drupal_get_path('module', 'custom_seat_view') . '/js/Pano.js');
    drupal_add_js(drupal_get_path('module', 'custom_seat_view') . '/js/custom_vrviewer.js');
    foreach ($seat_holds as $seats) {
      if (isset($seats->seat_num) && $seats->seat_num) {
	$vv_data = get_virtual_venue_data($seats->section_name, $seats->row_name, $seats->seat_num);
	$seat_url = variable_get('vv_seat_url', 'http://vv-orl.s3.amazonaws.com/media/vrview') . '/' . $imageFolder;
	$price_code_desc = (isset($seats->price_code) && $seats->price_code) ? strtolower(trim($seats->price_code)) : '';
	if (count($vv_data)) {
	  $data_tilt = (isset($vv_data[0]->cameraAngleTiltY) && $vv_data[0]->cameraAngleTiltY) ? $vv_data[0]->cameraAngleTiltY : 0;
	  $items[$seats->section_name . '-' . $seats->row_name . '-' . $seats->seat_num . '-' . $seats->last_seat] = '<a data-tilt= "' . $data_tilt . '" class="seat-info-level-'. str_replace(' ', '_', strtolower(trim($price_code_desc))) .'" href="'. $seat_url . '/' . $vv_data[0]->vrview_folder . '"><span><ul><li>'. t('Section') . ' ' . $seats->section_name . '&nbsp;</li><li>' . t('Row') . ' ' . $seats->row_name  . '</li><li>' .  t('Seats') . ' ' . $seats->seat_num . '-' . $seats->last_seat . '</li><li> <i class="fa fa-angle-down"> </i></li></ul></span></a>';
	}
	else {
	  $items[$seats->section_name . '-' . $seats->row_name . '-' . $seats->seat_num . '-' . $seats->last_seat] = '<a class="seat-info-level-'. str_replace(' ', '_', strtolower(trim($price_code_desc))) .'" href="#"><span><ul><li>'. t('Section') . ' ' . $seats->section_name . '&nbsp;</li><li>' . t('Row') . ' ' . $seats->row_name  . '</li><li>' .  t('Seats') . ' ' . $seats->seat_num . '-' . $seats->last_seat . '</li><li> <i class="fa fa-angle-down"> </i></li></ul></span></a>';
	}
      }
    }
    $data = theme('item_list', array('items' => $items, 'type' => 'ul', 'attributes' => array('class' => 'seat-vv-info')));
  }
  return theme('vv_seat_view', array('seat_holds' => $data, 'seat_count' => $seat_count));
}