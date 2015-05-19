
<?php
function custom_seat_view_seat_view_settings() {
  $form['box1'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('stp-legend-heading')),
  );
  $form['box1']['vv_seat_url'] = array(//homepage
    '#type' => 'textfield',
    '#title' => t('3D Bucket Url'),
    '#default_value' => variable_get('vv_seat_url', 'http://vv-orl.s3.amazonaws.com/media/vrview'),
    '#required' => TRUE,
    '#size' => 120,
  );
  $form['box5'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('stp-legend-heading')),
  );
  $footer =  variable_get('seat_view_footer', array());
  $form['box5']['seat_view_footer'] = array(
    '#type' => 'text_format',
    '#title' => t('Seat Information'),
    '#rows' => 40,
    '#default_value' => isset($footer['value'])?$footer['value'] : '',
    '#required' => TRUE,
    '#format' => 'full_html',
  );
  return system_settings_form($form);
}