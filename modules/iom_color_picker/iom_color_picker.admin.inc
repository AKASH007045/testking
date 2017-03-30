<?php
/**
 * @file
 * Hooks for color module.
 */



/** 
 * color setting form for desktop.
 */
function iom_color_settings_form() {
  $form = array();
  iom_color_picker_add_reqired_js();  
  
  //These fields are related to db field.
 $brand_fields = iom_color_picker_get_brands_fields();
  
  foreach ($brand_fields as $key => $val){    
    //Dynamically create blank variable;
    $$val = '';
  }
  //Get data from database;
  $select = db_select('tbl_manage_branding', 'tbs');  
  $select->fields('tbs');//We need all field always.
  $select->orderBy('cid', 'DESC');
  $select->range(0,1);
  $results = $select->execute()->fetchObject();
  $cid = $results->cid;
  $timestamp = $results->timestamp;
  $row = unserialize($results->css);
  foreach ($brand_fields as $key1 => $val1) { 
    //Dynamically create real variable;
    if (in_array($val1, array_keys($row))){
      $$val1 = $row[$val1];
    }
  }
   
  $gradient_picker = 'To update/create the gradient color <a href="http://www.colorzilla.com/gradient-editor"  target="_blank"><span>Click Here</span></a>';
   
  $form['cid'] = array(
    '#type' => 'hidden',
    '#default_value' => $cid,
  );
  
 $form['contact_information'] = array(
  '#markup' => '<strong>Last Updated: '. format_date($timestamp).'</strong> ',
  );

$site_container = "background-image: url(data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0idXRmLTgiP…B3aWR0aD0iMTAwJSIgaGVpZ2h0PSIxMDAlIiBmaWxsPSJ1cmwoI2dyYWQpIiAvPjwvc3ZnPiA=);
    background-size: 100%;
    background-image: -webkit-gradient(linear, 50% 0%, 50% 100%, color-stop(0%, #000000), color-stop(75%, rgba(0, 0, 0, 0.81)), color-stop(85%, rgba(0, 0, 0, 0.61)), color-stop(90%, rgba(0, 0, 0, 0.41)), color-stop(100%, rgba(0, 0, 0, 0)));
    background-image: -moz-linear-gradient(top, #000000 0%, rgba(0, 0, 0, 0.81) 75%, rgba(0, 0, 0, 0.61) 85%, rgba(0, 0, 0, 0.41) 90%, rgba(0, 0, 0, 0) 100%);
    background-image: -webkit-linear-gradient(top, #000000 0%, rgba(0, 0, 0, 0.81) 75%, rgba(0, 0, 0, 0.61) 85%, rgba(0, 0, 0, 0.41) 90%, rgba(0, 0, 0, 0) 100%);
    background-image: linear-gradient(to bottom, #000000 0%, rgba(0, 0, 0, 0.81) 75%, rgba(0, 0, 0, 0.61) 85%, rgba(0, 0, 0, 0.41) 90%, rgba(0, 0, 0, 0) 100%);";
 $form['box1'] = array(
    '#type' => 'fieldset',
    '#title' => 'Brand',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#prefix' => '<div id="brand-area">',
    '#suffix'=>'</div>',
  );   
  $form['box1']['site_title_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Top Header Page Title Text Color',
    '#default_value' => $site_title_color ? $site_title_color : '#ffffff',
    '#attributes'=> array('id' => "site_title_color"),
  );
  $form['box1']['site_container_color'] = array(
    '#type' => 'textarea',
    '#title' => 'Content box Background Color ('. $gradient_picker .')',
    '#default_value' => $site_container_color ? $site_container_color : $site_container,    
  ); 
  //Top Navigation Starts here
 $form['box2'] = array(
    '#type' => 'fieldset',
    '#title' => 'Top Navigation',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#prefix' => '<div id="topnav-area">',
    '#suffix'=>'</div>',
  );
  
   $form['box2']['nav_top_border_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Top Border Color',
    '#default_value' => $nav_top_border_color ? $nav_top_border_color : '#ffffff',
    '#attributes'=> array('id' => "nav_top_border_color"),
  );
  $form['box2']['nav_item_hover_border'] = array(
    '#type' => 'textfield',
    '#title' => 'Link Border Color',
    '#default_value' => $nav_item_hover_border ? $nav_item_hover_border : 'rgba(0, 0, 0, 0.75)',
    '#attributes'=> array('id' => "nav_item_hover_border"),
  );  
  $form['box2']['nav_font_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Link Text Hover Color',
    '#default_value' => $nav_font_color ? $nav_font_color : '#ffffff',
    '#attributes'=> array('id' => "nav_font_color"),
  );  
  $form['box2']['nav_drop_toggle_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Link Hover color',
    '#default_value' => $nav_drop_toggle_color ? $nav_drop_toggle_color : '#ffffff',
    '#attributes'=> array('id' => "nav_drop_toggle_color"),
  );
   $form['box2']['nav_top_background_shadow'] = array(
    '#type' => 'textfield',
    '#title' => 'Top Border Shadow Color',
    '#default_value' => $nav_top_background_shadow ? $nav_top_background_shadow : '#dedede',
    '#attributes'=> array('id' => "nav_top_background_shadow"),
  );
 $nav_default_gradiant_def = "background-image: url('data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0idXRmLTgiPz4gPHN2ZyB2ZXJzaW9uPSIxLjEiIHhtbG5zPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyI+PGRlZnM+PGxpbmVhckdyYWRpZW50IGlkPSJncmFkIiBncmFkaWVudFVuaXRzPSJvYmplY3RCb3VuZGluZ0JveCIgeDE9IjAuNSIgeTE9IjAuMCIgeDI9IjAuNSIgeTI9IjEuMCI+PHN0b3Agb2Zmc2V0PSIwJSIgc3RvcC1jb2xvcj0iIzUxMmMxYyIvPjxzdG9wIG9mZnNldD0iMjElIiBzdG9wLWNvbG9yPSIjNTEyYzFjIi8+PHN0b3Agb2Zmc2V0PSIyNiUiIHN0b3AtY29sb3I9IiM0ZTI5MTkiLz48c3RvcCBvZmZzZXQ9IjI5JSIgc3RvcC1jb2xvcj0iIzRkMjkxYiIvPjxzdG9wIG9mZnNldD0iNjMlIiBzdG9wLWNvbG9yPSIjMmIxNzBlIi8+PHN0b3Agb2Zmc2V0PSI3OSUiIHN0b3AtY29sb3I9IiMyMDExMGEiLz48c3RvcCBvZmZzZXQ9IjEwMCUiIHN0b3AtY29sb3I9IiMyMDExMGMiLz48L2xpbmVhckdyYWRpZW50PjwvZGVmcz48cmVjdCB4PSIwIiB5PSIwIiB3aWR0aD0iMTAwJSIgaGVpZ2h0PSIxMDAlIiBmaWxsPSJ1cmwoI2dyYWQpIiAvPjwvc3ZnPiA=');
    background-size: 100%;
    background-image: -webkit-gradient(linear, 50% 0%, 50% 100%, color-stop(0%, #512c1c), color-stop(21%, #512c1c), color-stop(26%, #4e2919), color-stop(29%, #4d291b), color-stop(63%, #2b170e), color-stop(79%, #20110a), color-stop(100%, #20110c));
    background-image: -moz-linear-gradient(top, #512c1c 0%, #512c1c 21%, #4e2919 26%, #4d291b 29%, #2b170e 63%, #20110a 79%, #20110c 100%);
    background-image: -webkit-linear-gradient(top, #512c1c 0%, #512c1c 21%, #4e2919 26%, #4d291b 29%, #2b170e 63%, #20110a 79%, #20110c 100%);
    background-image: linear-gradient(to bottom, #512c1c 0%, #512c1c 21%, #4e2919 26%, #4d291b 29%, #2b170e 63%, #20110a 79%, #20110c 100%);
    ";
  $nav_hover_gradiant_def = "background-image: url('data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0idXRmLTgiPz4gPHN2ZyB2ZXJzaW9uPSIxLjEiIHhtbG5zPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyI+PGRlZnM+PGxpbmVhckdyYWRpZW50IGlkPSJncmFkIiBncmFkaWVudFVuaXRzPSJvYmplY3RCb3VuZGluZ0JveCIgeDE9IjAuNSIgeTE9IjAuMCIgeDI9IjAuNSIgeTI9IjEuMCI+PHN0b3Agb2Zmc2V0PSIwJSIgc3RvcC1jb2xvcj0iI2U0YjUyYiIvPjxzdG9wIG9mZnNldD0iMTklIiBzdG9wLWNvbG9yPSIjZTRiNTJiIi8+PHN0b3Agb2Zmc2V0PSIyNCUiIHN0b3AtY29sb3I9IiNlMWIxMjkiLz48c3RvcCBvZmZzZXQ9IjI3JSIgc3RvcC1jb2xvcj0iI2RjYWQyOSIvPjxzdG9wIG9mZnNldD0iMzIlIiBzdG9wLWNvbG9yPSIjZDBhNDI3Ii8+PHN0b3Agb2Zmc2V0PSI0MSUiIHN0b3AtY29sb3I9IiNiYTkyMjEiLz48c3RvcCBvZmZzZXQ9IjQ2JSIgc3RvcC1jb2xvcj0iI2E4ODUyMSIvPjxzdG9wIG9mZnNldD0iNjIlIiBzdG9wLWNvbG9yPSIjNzY1ZTE2Ii8+PHN0b3Agb2Zmc2V0PSI3MyUiIHN0b3AtY29sb3I9IiM1ZTRiMTEiLz48c3RvcCBvZmZzZXQ9Ijc2JSIgc3RvcC1jb2xvcj0iIzViNDgxMCIvPjxzdG9wIG9mZnNldD0iMTAwJSIgc3RvcC1jb2xvcj0iIzViNDgxMCIvPjwvbGluZWFyR3JhZGllbnQ+PC9kZWZzPjxyZWN0IHg9IjAiIHk9IjAiIHdpZHRoPSIxMDAlIiBoZWlnaHQ9IjEwMCUiIGZpbGw9InVybCgjZ3JhZCkiIC8+PC9zdmc+IA==');
    background-size: 100%;
    background-image: -webkit-gradient(linear, 50% 0%, 50% 100%, color-stop(0%, #e4b52b), color-stop(19%, #e4b52b), color-stop(24%, #e1b129), color-stop(27%, #dcad29), color-stop(32%, #d0a427), color-stop(41%, #ba9221), color-stop(46%, #a88521), color-stop(62%, #765e16), color-stop(73%, #5e4b11), color-stop(76%, #5b4810), color-stop(100%, #5b4810));
    background-image: -moz-linear-gradient(top, #e4b52b 0%, #e4b52b 19%, #e1b129 24%, #dcad29 27%, #d0a427 32%, #ba9221 41%, #a88521 46%, #765e16 62%, #5e4b11 73%, #5b4810 76%, #5b4810 100%);
    background-image: -webkit-linear-gradient(top, #e4b52b 0%, #e4b52b 19%, #e1b129 24%, #dcad29 27%, #d0a427 32%, #ba9221 41%, #a88521 46%, #765e16 62%, #5e4b11 73%, #5b4810 76%, #5b4810 100%);
    background-image: linear-gradient(to bottom, #e4b52b 0%, #e4b52b 19%, #e1b129 24%, #dcad29 27%, #d0a427 32%, #ba9221 41%, #a88521 46%, #765e16 62%, #5e4b11 73%, #5b4810 76%, #5b4810 100%);";
  $form['box2']['nav_default_gradiant'] = array(
    '#type' => 'textarea',
    '#title' => 'Nav Bar Gradient Color('.$gradient_picker.')',
    '#default_value' => $nav_default_gradiant ? $nav_default_gradiant : $nav_default_gradiant_def,
    '#attributes'=> array('id' => "nav_default_gradiant"),
  );
  $form['box2']['nav_gradiant_hover'] = array(
    '#type' => 'textarea',
    '#title' => 'Nav Bar Gradient Hover color ('.$gradient_picker.')',
    '#default_value' => $nav_gradiant_hover ? $nav_gradiant_hover : $nav_hover_gradiant_def,   
    '#attributes'=> array('id' => "nav_gradiant_hover"),
  );
  //Top Navigation Ends here

  $form['box3'] = array(
    '#type' => 'fieldset',
    '#title' => 'Sub Navigation',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#prefix' => '<div id="subnav-area">',
    '#suffix'=>'</div>',
  );
  $form['box3']['nav_drop_down_item_font_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Link Text Color',
    '#default_value' => $nav_drop_down_item_font_color ? $nav_drop_down_item_font_color : '#ffffff',
    '#attributes'=> array('id' => "nav_drop_down_item_font_color"),
  );  
  $form['box3']['nav_drop_down_item_hover'] = array(
    '#type' => 'textfield',
    '#title' => 'Link Hover Background color',
    '#default_value' => $nav_drop_down_item_hover ? $nav_drop_down_item_hover : '#fdc82f',
    '#attributes'=> array('id' => "nav_drop_down_item_hover"),
  );
  $form['box3']['nav_drop_down_background'] = array(
    '#type' => 'textfield',
    '#title' => 'SubNav dropmenu background color',
    '#default_value' => $nav_drop_down_background ? $nav_drop_down_background : 'rgba(0, 0, 0, 0.75)',
    '#attributes'=> array('id' => "nav_drop_down_background"),
  );
  $form['box3']['nav_drop_down_background_shadow'] = array(
    '#type' => 'textfield',
    '#title' => 'SubNav dropmenu background shadow color',
    '#default_value' => $nav_drop_down_background_shadow ? $nav_drop_down_background_shadow : '#0000',
    '#attributes'=> array('id' => "nav_drop_down_background_shadow"),
  );
  //eof nav
    
  $form['box4'] = array(
    '#type' => 'fieldset',
    '#title' => 'Sign In Button',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,    
    '#prefix' => '<div id="buttons-area">',
    '#suffix'=>'</div>',
  );
  
  $form['box4']['buttons_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Text Color',
    '#default_value' => $buttons_color ? $buttons_color : '#ffffff',
    '#attributes'=> array('id'=>"buttons_color"),
  );
  $form['box4']['buttons_text_hover_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Text Hover Color',
    '#default_value' => $buttons_text_hover_color ? $buttons_text_hover_color : '#ffffff',
    '#attributes'=> array('id'=>"buttons_text_hover_color"),
  );
  
  $form['box4']['buttons_boxshadow_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Box Shadow Color',
    '#default_value' => $buttons_boxshadow_color ? $buttons_boxshadow_color :'#dedede',
    '#attributes'=> array('id' => "buttons_boxshadow_color"),
  );  
  $form['box4']['buttons_border_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Border/Focus color',
    '#default_value' => $buttons_border_color ? $buttons_border_color : 'rgba(25, 28, 31, 0.8)',
    '#attributes'=> array('id' => "buttons_border_color"),
  ); 
  $form['box4']['buttons_border_hover_color'] = array(
    '#type' => 'textfield', 
    '#title' => 'Border Hover Color',
    '#default_value' => $buttons_border_hover_color ? $buttons_border_hover_color : 'rgba(0, 0, 0, 0.75)',
    '#attributes'=> array('id' => "buttons_border_hover_color"),
  );
  $form['box4']['buttons_bg_hover_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Hover BG Color',
    '#default_value' => $buttons_bg_hover_color ? $buttons_bg_hover_color : '#5b4810',
    '#attributes'=> array('id' => "buttons_bg_hover_color"),
  );
  $default_buttons_gradiant_def = "background-image: url(data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0idXRmLTgiP…dpZHRoPSIxMDAlIiBoZWlnaHQ9IjEwMCUiIGZpbGw9InVybCgjZ3JhZCkiIC8+PC9zdmc+IA==);
    background-size: 100%;
    background-image: -webkit-gradient(linear, 50% 0%, 50% 100%, color-stop(0%, #e4b52b), color-stop(19%, #e4b52b), color-stop(24%, #e1b129), color-stop(27%, #dcad29), color-stop(32%, #d0a427), color-stop(41%, #ba9221), color-stop(46%, #a88521), color-stop(62%, #765e16), color-stop(73%, #5e4b11), color-stop(76%, #5b4810), color-stop(100%, #5b4810));
    background-image: -moz-linear-gradient(top, #e4b52b 0%, #e4b52b 19%, #e1b129 24%, #dcad29 27%, #d0a427 32%, #ba9221 41%, #a88521 46%, #765e16 62%, #5e4b11 73%, #5b4810 76%, #5b4810 100%);
    background-image: -webkit-linear-gradient(top, #e4b52b 0%, #e4b52b 19%, #e1b129 24%, #dcad29 27%, #d0a427 32%, #ba9221 41%, #a88521 46%, #765e16 62%, #5e4b11 73%, #5b4810 76%, #5b4810 100%);
    background-image: linear-gradient(to bottom, #e4b52b 0%, #e4b52b 19%, #e1b129 24%, #dcad29 27%, #d0a427 32%, #ba9221 41%, #a88521 46%, #765e16 62%, #5e4b11 73%, #5b4810 76%, #5b4810 100%);
    -webkit-box-shadow: 2px 2px 5px #000000;";
  $form['box4']['buttons_default_color_gradiant'] = array(
    '#type' => 'textarea',
    '#title' => 'BG gradient Color ('.$gradient_picker.')',
    '#default_value' => isset($buttons_default_color_gradiant) ? $buttons_default_color_gradiant : $default_buttons_gradiant_def,
  );
  
  $form['box5'] = array(
    '#type' => 'fieldset',
    '#title' => 'Global Button',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,    
    '#prefix' => '<div id="buttons-area2">',
    '#suffix'=>'</div>',
  );
  
  $form['box5']['global_buttons_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Text Color',
    '#default_value' => $global_buttons_color ? $global_buttons_color : '#ffffff',
    '#attributes'=> array('id' => "global_buttons_color"),
  );
  
  $form['box5']['global_buttons_hover_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Hover Text Color',
    '#default_value' => $global_buttons_hover_color ? $global_buttons_hover_color : '#5b4810',
    '#attributes'=> array('id' => "global_buttons_hover_color"),
  );
  
  $form['box5']['global_buttons_boxshadow_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Box Shadow Color',
    '#default_value' => $global_buttons_boxshadow_color ? $global_buttons_boxshadow_color :'#dedede',
    '#attributes'=> array('id' => "global_buttons_boxshadow_color"),
  );
  $form['box5']['global_buttons_border_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Border/Focus color',
    '#default_value' => $global_buttons_border_color ? $global_buttons_border_color : 'rgba(25, 28, 31, 0.8)',
    '#attributes'=> array('id' => "global_buttons_border_color"),
  ); 
 $form['box5']['global_buttons_border_hover_color'] = array(
    '#type' => 'textfield', 
    '#title' => 'Border Hover Color',
    '#default_value' => $global_buttons_border_hover_color ? $global_buttons_border_hover_color : 'rgba(0, 0, 0, 0.75)',
    '#attributes'=> array('id' => "global_buttons_border_hover_color"),
  );
 $form['box5']['global_buttons_bg'] = array(
    '#type' => 'textfield',
    '#title' => 'Hover BG Color',
    '#default_value' => $global_buttons_bg ? $global_buttons_bg : '#5b4810',
    '#attributes'=> array('id' => "global_buttons_bg"),
  );
  $global_buttons_gradiant_def = "background-image: url(data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0idXRmLTgiP…dpZHRoPSIxMDAlIiBoZWlnaHQ9IjEwMCUiIGZpbGw9InVybCgjZ3JhZCkiIC8+PC9zdmc+IA==);
    background-size: 100%;
    background-image: -webkit-gradient(linear, 50% 0%, 50% 100%, color-stop(0%, #e4b52b), color-stop(19%, #e4b52b), color-stop(24%, #e1b129), color-stop(27%, #dcad29), color-stop(32%, #d0a427), color-stop(41%, #ba9221), color-stop(46%, #a88521), color-stop(62%, #765e16), color-stop(73%, #5e4b11), color-stop(76%, #5b4810), color-stop(100%, #5b4810));
    background-image: -moz-linear-gradient(top, #e4b52b 0%, #e4b52b 19%, #e1b129 24%, #dcad29 27%, #d0a427 32%, #ba9221 41%, #a88521 46%, #765e16 62%, #5e4b11 73%, #5b4810 76%, #5b4810 100%);
    background-image: -webkit-linear-gradient(top, #e4b52b 0%, #e4b52b 19%, #e1b129 24%, #dcad29 27%, #d0a427 32%, #ba9221 41%, #a88521 46%, #765e16 62%, #5e4b11 73%, #5b4810 76%, #5b4810 100%);
    background-image: linear-gradient(to bottom, #e4b52b 0%, #e4b52b 19%, #e1b129 24%, #dcad29 27%, #d0a427 32%, #ba9221 41%, #a88521 46%, #765e16 62%, #5e4b11 73%, #5b4810 76%, #5b4810 100%);
    -webkit-box-shadow: 2px 2px 5px #000000;";
  $form['box5']['global_buttons_gradiant_bg'] = array(
    '#type' => 'textarea',
    '#title' => 'Buttons BG gradient Color gradient Color ('.$gradient_picker.')',
    '#default_value' => $global_buttons_gradiant_bg ? $global_buttons_gradiant_bg : $global_buttons_gradiant_def,
  );
 
  $form['box5']['video_btn_bg_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Video Play Button color',
    '#default_value' => $video_btn_bg_color ? $video_btn_bg_color : '#522b1c',
    '#attributes'=> array('id' => "video_btn_bg_color"),
  );
 $form['box5']['video_btn_bg_hover_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Video Play Button\'s hover color',
    '#default_value' => $video_btn_bg_hover_color ? $video_btn_bg_hover_color : '#522b1c',
    '#attributes'=> array('id' => "video_btn_bg_hover_color"),
  );
 
  $form['box6'] = array(
    '#type' => 'fieldset',
    '#title' => 'Promo Tiles',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#prefix' => '<div id="promo-area">',
    '#suffix'=>'</div>',
  );
  $form['box6']['promo_title_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Head Title Color',
    '#default_value' => $promo_title_color ? $promo_title_color : '#ffffff',
    '#attributes'=> array('id'=>"promo_title_color"),
  );
  $promo_text = 'background-image: url(data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0idXRmLTgiP…dpZHRoPSIxMDAlIiBoZWlnaHQ9IjEwMCUiIGZpbGw9InVybCgjZ3JhZCkiIC8+PC9zdmc+IA==);
    background-size: 100%;
    background-image: -webkit-gradient(linear, 50% 0%, 50% 100%, color-stop(0%, #e4b52b), color-stop(19%, #e4b52b), color-stop(24%, #e1b129), color-stop(27%, #dcad29), color-stop(32%, #d0a427), color-stop(41%, #ba9221), color-stop(46%, #a88521), color-stop(62%, #765e16), color-stop(73%, #5e4b11), color-stop(76%, #5b4810), color-stop(100%, #5b4810));
    background-image: -moz-linear-gradient(top, #e4b52b 0%, #e4b52b 19%, #e1b129 24%, #dcad29 27%, #d0a427 32%, #ba9221 41%, #a88521 46%, #765e16 62%, #5e4b11 73%, #5b4810 76%, #5b4810 100%);
    background-image: -webkit-linear-gradient(top, #e4b52b 0%, #e4b52b 19%, #e1b129 24%, #dcad29 27%, #d0a427 32%, #ba9221 41%, #a88521 46%, #765e16 62%, #5e4b11 73%, #5b4810 76%, #5b4810 100%);
    background-image: linear-gradient(to bottom, #e4b52b 0%, #e4b52b 19%, #e1b129 24%, #dcad29 27%, #d0a427 32%, #ba9221 41%, #a88521 46%, #765e16 62%, #5e4b11 73%, #5b4810 76%, #5b4810 100%);
    -webkit-box-shadow: 2px 2px 5px #000000;';
    $promo_text2 = 'background-image: url(data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0idXRmLTgiP…Igd2lkdGg9IjEwMCUiIGhlaWdodD0iMTAwJSIgZmlsbD0idXJsKCNncmFkKSIgLz48L3N2Zz4g);
    background-size: 100%;
    background-image: -webkit-gradient(linear, 50% 0%, 50% 100%, color-stop(0%, rgba(53, 42, 10, 0.47)), color-stop(1%, rgba(53, 42, 10, 0.5)), color-stop(17%, rgba(56, 45, 10, 0.5)), color-stop(42%, rgba(50, 40, 10, 0.5)), color-stop(55%, rgba(55, 44, 10, 0.5)), color-stop(70%, rgba(56, 44, 10, 0.4)), color-stop(72%, rgba(56, 44, 10, 0.38)), color-stop(89%, rgba(51, 40, 9, 0.17)), color-stop(100%, rgba(47, 37, 9, 0.07)));
    background-image: -moz-linear-gradient(top, rgba(53, 42, 10, 0.47) 0%, rgba(53, 42, 10, 0.5) 1%, rgba(56, 45, 10, 0.5) 17%, rgba(50, 40, 10, 0.5) 42%, rgba(55, 44, 10, 0.5) 55%, rgba(56, 44, 10, 0.4) 70%, rgba(56, 44, 10, 0.38) 72%, rgba(51, 40, 9, 0.17) 89%, rgba(47, 37, 9, 0.07) 100%);
    background-image: -webkit-linear-gradient(top, rgba(53, 42, 10, 0.47) 0%, rgba(53, 42, 10, 0.5) 1%, rgba(56, 45, 10, 0.5) 17%, rgba(50, 40, 10, 0.5) 42%, rgba(55, 44, 10, 0.5) 55%, rgba(56, 44, 10, 0.4) 70%, rgba(56, 44, 10, 0.38) 72%, rgba(51, 40, 9, 0.17) 89%, rgba(47, 37, 9, 0.07) 100%);
    background-image: linear-gradient(to bottom, rgba(53, 42, 10, 0.47) 0%, rgba(53, 42, 10, 0.5) 1%, rgba(56, 45, 10, 0.5) 17%, rgba(50, 40, 10, 0.5) 42%, rgba(55, 44, 10, 0.5) 55%, rgba(56, 44, 10, 0.4) 70%, rgba(56, 44, 10, 0.38) 72%, rgba(51, 40, 9, 0.17) 89%, rgba(47, 37, 9, 0.07) 100%);';
  $form['box6']['promo_color'] = array(
    '#type' => 'textarea',
    '#title' => 'Head Title Background Color ('.$gradient_picker.')',
    '#default_value' => $promo_color ? $promo_color : $promo_text,
    '#attributes'=> array('id'=>"promo_color"),
  );
  $form['box6']['promo_hover_color'] = array(
    '#type' => 'textarea',
    '#title' => 'Promo Box Hover Color ('.$gradient_picker.')',
    '#default_value' => $promo_hover_color ? $promo_hover_color : $promo_text2,
    '#attributes'=> array('id'=>"promo_hover_color"),
  );
$form['box7'] = array(
    '#type' => 'fieldset',
    '#title' => 'Calendar',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,    
    '#prefix' => '<div id="calendar-area">',
    '#suffix'=>'</div>',
  );
$calender_gradiant = "url('data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0idXRmLTgiPz4gPHN2ZyB2ZXJzaW9uPSIxLjEiIHhtbG5zPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyI+PGRlZnM+PGxpbmVhckdyYWRpZW50IGlkPSJncmFkIiBncmFkaWVudFVuaXRzPSJvYmplY3RCb3VuZGluZ0JveCIgeDE9IjAuNSIgeTE9IjAuMCIgeDI9IjAuNSIgeTI9IjEuMCI+PHN0b3Agb2Zmc2V0PSIwJSIgc3RvcC1jb2xvcj0iI2U0YjUyYiIvPjxzdG9wIG9mZnNldD0iMTklIiBzdG9wLWNvbG9yPSIjZTRiNTJiIi8+PHN0b3Agb2Zmc2V0PSIyNCUiIHN0b3AtY29sb3I9IiNlMWIxMjkiLz48c3RvcCBvZmZzZXQ9IjI3JSIgc3RvcC1jb2xvcj0iI2RjYWQyOSIvPjxzdG9wIG9mZnNldD0iMzIlIiBzdG9wLWNvbG9yPSIjZDBhNDI3Ii8+PHN0b3Agb2Zmc2V0PSI0MSUiIHN0b3AtY29sb3I9IiNiYTkyMjEiLz48c3RvcCBvZmZzZXQ9IjQ2JSIgc3RvcC1jb2xvcj0iI2E4ODUyMSIvPjxzdG9wIG9mZnNldD0iNjIlIiBzdG9wLWNvbG9yPSIjNzY1ZTE2Ii8+PHN0b3Agb2Zmc2V0PSI3MyUiIHN0b3AtY29sb3I9IiM1ZTRiMTEiLz48c3RvcCBvZmZzZXQ9Ijc2JSIgc3RvcC1jb2xvcj0iIzViNDgxMCIvPjxzdG9wIG9mZnNldD0iMTAwJSIgc3RvcC1jb2xvcj0iIzViNDgxMCIvPjwvbGluZWFyR3JhZGllbnQ+PC9kZWZzPjxyZWN0IHg9IjAiIHk9IjAiIHdpZHRoPSIxMDAlIiBoZWlnaHQ9IjEwMCUiIGZpbGw9InVybCgjZ3JhZCkiIC8+PC9zdmc+IA==');
  background-size: 100%;
  background-image: -webkit-gradient(linear, 50% 0%, 50% 100%, color-stop(0%, #e4b52b), color-stop(19%, #e4b52b), color-stop(24%, #e1b129), color-stop(27%, #dcad29), color-stop(32%, #d0a427), color-stop(41%, #ba9221), color-stop(46%, #a88521), color-stop(62%, #765e16), color-stop(73%, #5e4b11), color-stop(76%, #5b4810), color-stop(100%, #5b4810));
  background-image: -moz-linear-gradient(top, #e4b52b 0%, #e4b52b 19%, #e1b129 24%, #dcad29 27%, #d0a427 32%, #ba9221 41%, #a88521 46%, #765e16 62%, #5e4b11 73%, #5b4810 76%, #5b4810 100%);
  background-image: -webkit-linear-gradient(top, #e4b52b 0%, #e4b52b 19%, #e1b129 24%, #dcad29 27%, #d0a427 32%, #ba9221 41%, #a88521 46%, #765e16 62%, #5e4b11 73%, #5b4810 76%, #5b4810 100%);
  background-image: linear-gradient(to bottom, #e4b52b 0%, #e4b52b 19%, #e1b129 24%, #dcad29 27%, #d0a427 32%, #ba9221 41%, #a88521 46%, #765e16 62%, #5e4b11 73%, #5b4810 76%, #5b4810 100%);";

 $form['box7']['calender_lr_bg'] = array(
    '#type' => 'textarea',
    '#title' => 'Calendar home game and full schedule tabs BG ('.$gradient_picker.')',
    '#default_value' => $calender_lr_bg ? $calender_lr_bg : $calender_gradiant,
    '#attributes'=> array('id'=>"calender_lr_bg"),
  );
 $form['box7']['calender_lr_border'] = array(
    '#type' => 'textfield',
    '#title' => 'Calendar home game and full schedule tabs border',
    '#default_value' => $calender_lr_border ? $calender_lr_border : 'rgba(0, 0, 0, 0.75)',
    '#attributes'=> array('id'=>"calender_lr_border"),
  );

$form['box7']['calender_events_heading_bg'] = array(
    '#type' => 'textfield',
    '#title' => 'Calendars Events Modal Pop UP Header',
    '#default_value' => $calender_events_heading_bg ? $calender_events_heading_bg : 'rgba(0, 0, 0, 0.75)',
    '#attributes'=> array('id'=>"calender_events_heading_bg"),
  );
$form['box7']['calender_home_game_squre'] = array(
    '#type' => 'textfield',
    '#title' => 'Calendar table td - Home Game',
    '#default_value' => $calender_home_game_squre ? $calender_home_game_squre : 'rgba(82, 43, 28, 0.75)',
    '#attributes'=> array('id'=>"calender_home_game_squre"),
  );
$form['box7']['calender_away_game_squre'] = array(
    '#type' => 'textfield',
    '#title' => 'Calendar table td - Away Game.',
    '#default_value' => $calender_away_game_squre ? $calender_away_game_squre : 'rgba(255, 255, 255, 0.6)',
    '#attributes'=> array('id'=>"calender_away_game_squre"),
  );
$form['box7']['calender_home_game_bg'] = array(
    '#type' => 'textfield',
    '#title' => 'Calendar bottom - Home Game',
    '#default_value' => $calender_home_game_bg ? $calender_home_game_bg : '#522b1c',
    '#attributes'=> array('id'=>"calender_home_game_bg"),
  );
$form['box7']['calender_home_border'] = array(
    '#type' => 'textfield',
    '#title' => 'Calendar bottom - Home border',
    '#default_value' => $calender_home_border ? $calender_home_border : '#000000',
    '#attributes'=> array('id'=>"calender_home_border"),
  );
$form['box7']['calender_away_game_bg'] = array(
    '#type' => 'textfield',
    '#title' => 'Calendar bottom - Away Game.',
    '#default_value' => $calender_away_game_bg ? $calender_away_game_bg : '#ffffff',
    '#attributes'=> array('id'=>"calender_away_game_bg"),
  );

  
  $list_view_cal = "background-image: url(data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0idXRmLTgiP…dpZHRoPSIxMDAlIiBoZWlnaHQ9IjEwMCUiIGZpbGw9InVybCgjZ3JhZCkiIC8+PC9zdmc+IA==);
    background-size: 100%;
    background-image: -webkit-gradient(linear, 50% 0%, 50% 100%, color-stop(0%, #e4b52b), color-stop(19%, #e4b52b), color-stop(24%, #e1b129), color-stop(27%, #dcad29), color-stop(32%, #d0a427), color-stop(41%, #ba9221), color-stop(46%, #a88521), color-stop(62%, #765e16), color-stop(73%, #5e4b11), color-stop(76%, #5b4810), color-stop(100%, #5b4810));
    background-image: -moz-linear-gradient(top, #e4b52b 0%, #e4b52b 19%, #e1b129 24%, #dcad29 27%, #d0a427 32%, #ba9221 41%, #a88521 46%, #765e16 62%, #5e4b11 73%, #5b4810 76%, #5b4810 100%);
    background-image: -webkit-linear-gradient(top, #e4b52b 0%, #e4b52b 19%, #e1b129 24%, #dcad29 27%, #d0a427 32%, #ba9221 41%, #a88521 46%, #765e16 62%, #5e4b11 73%, #5b4810 76%, #5b4810 100%);
    background-image: linear-gradient(to bottom, #e4b52b 0%, #e4b52b 19%, #e1b129 24%, #dcad29 27%, #d0a427 32%, #ba9221 41%, #a88521 46%, #765e16 62%, #5e4b11 73%, #5b4810 76%, #5b4810 100%);";
   $form['box7']['calender_listview_bg'] = array(
    '#type' => 'textarea',
    '#title' => 'Calendar List View BG ('.$gradient_picker.')',
    '#default_value' => $calender_listview_bg ? $calender_listview_bg : $list_view_cal,
    );
   $form['box7']['calender_listview_bg_hover'] = array(
    '#type' => 'textarea',
    '#title' => 'Calendar List View BG Hover ('.$gradient_picker.')',
    '#default_value' => $calender_listview_bg_hover ? $calender_listview_bg_hover : $list_view_cal,
    );
   $form['box7']['contact_us_bg_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Contact Us BG Color',
    '#default_value' => $contact_us_bg_color ? $contact_us_bg_color : 'rgba(82, 43, 28, 0.75)',
    '#attributes'=> array('id' => "contact_us_bg_color"),
  );
   $form['box8'] = array(
      '#type' => 'fieldset',
      '#title' => t('Additional'),
      '#weight' => 6,
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#prefix' => '<div id="Additional-css">',
      '#suffix' => '</div>',        
    );
    $form['box8']['addtional_css_overwrite'] = array(
      '#type' => 'textarea',
      '#title' => t('Additional CSS for overwrite on specific changes'),
      '#default_value' => $addtional_css_overwrite ? $addtional_css_overwrite : '',
    ); 
  $form['submit'] = array(
    '#type' => 'submit',
    '#weight' => 7,
    '#value' => 'Submit',
    );
 return $form;
}

function iom_color_settings_form_submit($form, &$form_state){
 if ($form_state['values']){
 //print '<pre>';
 //print_r($form_state['values']); exit;
  
  global $user;
  $brand_fields = iom_color_picker_get_brands_fields();
  $record = array();
  foreach ($brand_fields as $key => $val){    
    //Dynamically create blank variable;
    //print_r($val);
    if (in_array($val, array_keys($form_state['values']))){
      $css_data[$val] = $form_state['values'][$val] ? $form_state['values'][$val] : '';
    }
  }

  $record['uid'] = $user->uid;
  $record['timestamp'] = time();
  $record['css'] = serialize($css_data);
   $key_id = array();
   if (isset($record['cid']) && $record['cid']){       
     $key_id = 'cid'; 
     //if you want to keep the last updated and new record will be created always uncomment below two lines.
     //$key_id = array();
     //unset($record['cid']);     
   }
   
   $file_created = '';
   $result = drupal_write_record('tbl_manage_branding', $record, $key_id);
   if ($result){
     $file_created = iom_color_settings_create_css($form_state['values']);     
   }
   switch($result){
    case 1:
      drupal_set_message(t('A new record has been created successfully'));
      if ($file_created){
       drupal_set_message(t('A new css has been added successfully'));
      }
    break;
    case 2:
      drupal_set_message(t('Record has been updated successfully'));
      if ($file_created){
       drupal_set_message(t('A new css has been updated successfully'));
      }
    break;
    default:
      drupal_set_message(t('Submission failed!'), 'error');
   }
  }
  drupal_goto('admin/manage/site-branding');
}

function iom_color_settings_fav_form(){
 $form = array();
 $form['fav'] = array(
    '#type' => 'fieldset',
    '#title' => 'Favicon icon and site logo overwrite',
    '#description' => 'Site Name, Logo and favicon icon can be updated from here.',
  );

  $form['fav']['site_name'] = array(
      '#type' => 'textfield',
      '#title' => 'Site name',
      '#default_value' => variable_get('site_name'),
      '#description' => 'Override site name from here.',
  );
  $form['fav']['iom_color_logo'] = array(
    '#type' => 'managed_file',
    '#name' => 'iom_color_logo',
    '#title' => t('Logo image'),
    '#default_value' => variable_get('iom_color_logo'),
    '#size' => 40,
    '#description' => t("Logo Image should be less than 400 pixels wide and in gif/png/jpg/jpeg format."),
    '#upload_location' => 'public://',
    '#upload_validators' => array(
                            'file_validate_extensions' => array('gif png jpg jpeg'), 
                           ),
  );
  $form['fav']['iom_color_favicon'] = array(
    '#type' => 'managed_file',
    '#name' => 'iom_color_favicon',
    '#title' => t('Favicon icon'),
    '#size' => 40,    
    '#default_value' => variable_get('iom_color_favicon'),
    '#description' => t("Icon Image should be less than 26x26 pixels wide and in .ico format."),
    '#upload_location' => 'public://',
    '#upload_validators' => array(
                            'file_validate_extensions' => array('ico'), 
                           ),
  );
  // $form['fav']['submit'] = array(
    // '#type' => 'submit',
    // '#value' => 'Submit',
    // );
 $form['#submit'][] = 'iom_color_settings_fav_form_submit';
 return system_settings_form($form);
}

/**
 * hook_form_submit().
 * Make the file permanent.
 */
function iom_color_settings_fav_form_submit($form, &$form_state){
  $file = file_load($form_state['values']['iom_color_logo']);
  if($form_state['values']['site_name']){
    variable_set('site_name', $form_state['values']['site_name']);
  }
  if ($file) {
    $file->status = FILE_STATUS_PERMANENT;    
    file_save($file);
    variable_set('iom_color_logo', $file->fid);
    unset($form_state['values']['iom_color_logo']); // make sure it is unset for system submit
    file_usage_add($file, 'iom_color', 'iom_color_logo', $file->fid);
  }
  $file = file_load($form_state['values']['iom_color_favicon']);
  if ($file) {
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    variable_set('iom_color_favicon', $file->fid);
    unset($form_state['values']['iom_color_favicon']); // make sure it is unset for system submit
    file_usage_add($file, 'iom_color', 'iom_color_favicon', $file->fid);
  }
}

/**
 * Hook_form_submit().
 * Purpose: get the submitted value and create a css named:"theme-css.css"
 * @return: $file_created, TRUE OR FALSE.
 */
function iom_color_settings_create_css($form_state_value){
 if(!$form_state_value){
  return FALSE;
 }
    //key is the tables col and value are css variables will be write in css.
    $path = drupal_get_path('module', 'iom_color_picker');    
    $filename = $path ."/theme-css.php";
    
    $handle = fopen($filename, "r");
    if (FALSE === $handle) {
      exit("Failed to open stream to URL");
    }

   //mapping form field (key) and theme-css.php field (value)
   //which will be replace by...
   //name and key may not same.
    $css_variables = array(        
        'site_title_color' => 'site_title_color',
        'site_container_color' => 'site_container_color',
        'buttons_color' => 'buttons_color',
        'buttons_bg_hover_color' => 'buttons_bg_hover_color',
        'buttons_boxshadow_color' => 'buttons_boxshadow_color',
        'buttons_border_color' => 'buttons_border_color',
        'buttons_default_color_gradiant' => 'buttons_default_color_gradiant',
        'buttons_border_hover_color' => 'buttons_border_hover_color',
        'buttons_text_hover_color' => 'buttons_text_hover_color',   
        'global_buttons_color' => 'global_buttons_color',
        'global_buttons_hover_color' => 'global_buttons_hover_color',
        'global_buttons_boxshadow_color' => 'global_buttons_boxshadow_color',
        'global_buttons_border_color' => 'global_buttons_border_color',   
        'global_buttons_border_hover_color' => 'global_buttons_border_hover_color',
        'global_buttons_bg' => 'global_buttons_bg',
        'global_buttons_gradiant_bg' => 'global_buttons_gradiant_bg',
        'video_btn_bg_color' => 'video_btn_bg_color',
        'video_btn_bg_hover_color' => 'video_btn_bg_hover_color',        
        'nav_font_color' => 'nav_font_color',
        'nav_top_border_color' => 'nav_top_border_color',
        'nav_top_background_shadow' => 'nav_top_background_shadow',
        'nav_item_hover_border' => 'nav_item_hover_border',
        'nav_drop_toggle_color' => 'nav_drop_font_color',
        'nav_drop_down_item_hover' => 'nav_drop_down_item_hover',
        'nav_drop_down_item_font_color' => 'nav_drop_down_item_font_color',
        'promo_color' => 'primotile_and_title_head',
        'promo_hover_color' => 'primotile_hover',
        'promo_title_color'=> 'promo_title_color',        
        'nav_default_gradiant' => 'nav_gradient',
        'nav_gradiant_hover' => 'nav_gradient_hover',
        'nav_drop_down_background' =>'nav_drop_down_background',
        'nav_drop_down_background_shadow'=>'nav_drop_down_background_shadow',
        'calender_lr_bg' => 'calender_lr_bg',
        'calender_lr_border' => 'calender_lr_border',
        'calender_events_heading_bg' =>  'calender_events_heading_bg',
        'calender_home_game_bg' => 'calender_home_game_bg',
        'calender_home_border' => 'calender_home_border',
        'calender_away_game_bg' => 'calender_away_game_bg',
        'calender_home_game_squre' => 'calender_home_game_squre',
        'calender_away_game_squre' => 'calender_away_game_squre',
        'calender_listview_bg' => 'calender_listview_bg',
        'calender_listview_bg_hover' => 'calender_listview_bg_hover',
        'addtional_css_overwrite' => 'addtional_css_overwrite',
        'contact_us_bg_color' => 'contact_us_bg_color',
    );
    $ct = 1;
    $number_of_var = count($css_variables);
    foreach ($css_variables as $key => $var) {
      $color = $form_state_value[$key];
      $val1 = $var;
      //$var = $color;
      $$val1 = $color; //Create real variable to replace php variable in css var.
      if ($number_of_var == $ct){
        $filename = "theme-css.php";
        include_once $filename;
        $updated_css = $css; //$css variable defined in theme-css.php
      }
      $ct++;
    }
    
    //$path_to_public_file = iom_color_picker_get_files_path();
    $path_to_public_file = '';
     if (function_exists('iom_color_picker_get_files_path')) {
      $path_to_public_file = iom_color_picker_get_files_path();
     }  
    $fp = fopen($path_to_public_file."/theme-css.css", 'w+');
    $file_opened = TRUE;
    if (FALSE === $fp) {
      $file_opened = FALSE;
      drupal_set_message(t('Failed to open stream to URL and css unable to update!'));
    }
    else if ($file_opened){
       fwrite($fp, $updated_css);
       drupal_chmod($path_to_public_file."/theme-css.css");
       cache_clear_all();
       fclose($fp);
     }
    fclose($fp);
    return $file_opened;
}

/**
 * Hook_form_submit().
 * Purpose: get the submitted value and create a css named:"mobile_css.css"
 * @return: $file_created, TRUE OR FALSE.
 */
function iom_color_settings_create_mobile_css($form_state_value){
     global $user;
     if (!$form_state_value){
      return FALSE;
     }

    //key is the tables col and value are css variables will be write in css.
    $path = drupal_get_path('module', 'iom_color_picker');    
    $filename = $path ."/mobile_css.php";
    
    $handle = fopen($filename, "r");
    if (FALSE === $handle) {
      exit("Failed to open stream to URL");
    }

   //mapping form field (key) and mobile_css.php field (value)
   //which will be replace by...
   //name and key may not same.
    $css_variables = iom_color_picker_get_brands_fields_mobile();
    $ct = 1;
    $number_of_var = count($css_variables);

    $images_in_css = array('mob_header_top_bg','mob_splash_img_home', 'brand_color', 'stp_calendar_mob_bg');
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
      
      if (in_array($var, $images_in_css) && $color){        
        $file = file_load($color);
        $file_full_path = file_create_url($file->uri);
        if ($val1 == 'brand_color'){
          $val1 = 'mob_splash_img_landscape';
        }
        $$val1 = $file_full_path;
      }
            
      if ($number_of_var == $ct){
        $filename = "mobile_css.php";
        include_once $filename;
        $updated_css = $css; //$css variable defined in mobile_css.php
      }
      $ct++;
    }
    
    if (function_exists('iom_color_picker_get_files_path')) {  
      $path_to_public_file = iom_color_picker_get_files_path();
    }


    $fp = fopen($path_to_public_file."/mobile-theme.css", 'w+');
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

/**
 * STP Footer block settings
 */
function iom_color_settings_copyright_form() {
    $validate =  array(
        'file_validate_extensions' => array('png jpg jpeg gif'),
        // Pass the maximum file size in bytes
        'file_validate_size' => array(1024*1024*2),
    );
    $form['box2'] = array(
        '#type' => 'fieldset',
        '#attributes' => array('class' => array('stp-footer-texts')),
    );
    $form['box2']['stp_footer_text1'] = array(
        '#type' => 'textarea',
        '#title' => t('Client Copyright Text'),
        '#description' => t('Please enter client footer text. Available Token: !YEAR'),
        '#default_value' => variable_get('stp_footer_text1', '')
    );
    $form['box2']['stp_footer_text2'] = array(
        '#type' => 'textarea',
        '#title' => t('League Copyright Text'),
        '#description' => t('Please enter League footer text. Available Token: !YEAR'),
        '#default_value' => variable_get('stp_footer_text2', '')
    );
    $form['box2']['stp_footer_text3'] = array(
        '#type' => 'textarea',
        '#title' => t('IOMEDIA Copyright Text'),
        '#description' => t('Please enter IOMEDIA copyright footer text. Available Token: !YEAR'),
        '#required' => TRUE,
        '#default_value' => variable_get('stp_footer_text3', '�!YEAR IOMEDIA, Inc. All Rights Reserved. �Season Ticket Portal� and the �STP� shield are registered trademarks of IOMEDIA, Inc.')
    );
    $form['box1'] = array(
        '#type' => 'fieldset',
        '#attributes' => array('class' => array('stp-footer-logo')),
    );
    $form['box1']['desktop_stp_footer_logo'] = array(
        '#type' => 'managed_file',
        '#title' => t('Footer Logo'),
        '#required' => TRUE,
        '#description' => t('Please upload footer logo.'),
        '#default_value' => variable_get('stp_footer_logo_fid', ''),
        '#upload_validators' => $validate,
        '#upload_location' => 'public://footer/',
    );
    // Perform our custom submit before system submit
    $form['#submit'][] = 'iom_color_settings_copyright_form_submit';
    return system_settings_form($form);
}

function iom_color_settings_copyright_form_submit($form, &$form_state) {
    // global $user;
    // Load the file via file.fid.
    $file = file_load($form_state['values']['desktop_stp_footer_logo']);
    if ($file) {
        // Change status to permanent.
        $file->status = FILE_STATUS_PERMANENT;
        file_save($file);
        // Save file to variable
        variable_set('stp_footer_logo_fid', $file->fid);
        unset($form_state['values']['desktop_stp_footer_logo']); // make sure it is unset for system submit
        file_usage_add($file, 'file', 'file', $file->fid);
    }
}

/**
 * STP meta data settings
 *
 */
function iom_color_picker_metadata_settings() {
    $form['box1'] = array(
        '#type' => 'fieldset',
        '#attributes' => array('class' => array('stp-footer-texts')),
    );
    $form['box1']['stp_meta_title'] = array(
        '#type' => 'textfield',
        '#title' => t('Title'),
        '#required' => TRUE,
        '#description' => t('Please enter the title for STP'),
        '#default_value' => variable_get('stp_meta_title', '')
    );
    $form['box1']['stp_metadata'] = array(
        '#type' => 'textarea',
        '#title' => t('Meta Data'),
        '#required' => TRUE,        
        '#description' => t('Please enter meta data for the STP'),
        '#default_value' => variable_get('stp_metadata', '')
    );
    $form['box1']['iomedia_gaid'] = array(
        '#type' => 'textfield',
        '#title' => t('IOMEDIA GA ID'),        
        '#description' => t('Please enter IOMEDIA GA ID '),
        '#default_value' => variable_get('iomedia_gaid', '')
    );
    $form['box1']['client_gaid'] = array(
        '#type' => 'textfield',
        '#title' => t('Client GA ID'),        
        '#description' => t('Please enter Client GA ID'),
        '#default_value' => variable_get('client_gaid', '')
    );
    $form['box1']['remarketing_pixels'] = array(
        '#type' => 'textarea',
        '#title' => t('Remarketing Pixels'),        
        '#description' => t('Please enter meta data for the STP'),
        '#default_value' => variable_get('remarketing_pixels', '')
    );
    return system_settings_form($form);
}

/** 
 * General color setting form for Mobile.
 */
function iom_color_picker_mobile_setting_form() {
  $form = array();
  iom_color_picker_add_reqired_js();
  $fields_mob = iom_color_picker_get_brands_fields_mobile();
  foreach ($fields_mob as $key => $val){    
    //Dynamically create blank variable;
    $$val = '';
  }
  //Get data from database;
  $select = db_select('tbl_manage_branding_mobile', 'tbs');  
  $select->fields('tbs');//We need all field always.
  $select->orderBy('mid', 'DESC');
  $select->range(0,1);
  $results = $select->execute()->fetchObject();
  $mid = $results->mid;
  $timestamp = $results->timestamp;
  $row = unserialize($results->css);
  
  foreach ($fields_mob as $key1 => $val1){     
    //Dynamically create real variable;
    if (in_array($val1, array_keys($row))){
      $$val1 = $row[$val1];
    }
  }
    
   
  $form['mid'] = array(
    '#type' => 'hidden',
    '#default_value' => $mid,
  );

  $form['contact_information'] = array(
  '#markup' => '<strong>Last Updated: '. format_date($timestamp).'</strong> ',
  );
    
    $gradient_picker = 'To update/create the gradient color <a href="http://www.colorzilla.com/gradient-editor"  target="_blank"><span>Click Here</span></a>';
    $form['box1'] = array(
        '#type' => 'fieldset',
        '#title' => t('API and Other Settings.'),
        '#weight' => 1,
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
    );
    // Enable Mobile Site
    $enabled = $en_mob_site ? 'Enabled' : 'Enable';
    $form['box1']['en_mob_site'] = array(
        '#type' => 'checkbox',
        '#title' => t($enabled),
        '#description'=>'Please enable Mobile site',
        '#default_value' => $en_mob_site ? $en_mob_site : '',
    );
  // Enable Theater Calendar
    $form['box1']['en_theater_calendar'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enabled Theater Calender'),
      '#default_value' => variable_get('en_theater_calendar', ''),
      '#description' => t('Please enable Theater calender'),    
    );
   // For mobile domain
    $form['box1']['mob_domain'] = array(
        '#type' => 'textfield',
        '#title' => t('Mobile Domain'),
        '#default_value' => $mob_domain ? $mob_domain : '',      
    );
    
    $form['box1']['stp_buypage_url'] = array(
        '#type' => 'textfield',
        '#title' => t('STP Buy/Upgrade Page url'),
        '#default_value' => $stp_buypage_url ? $stp_buypage_url : '',    
    );
    // Account manager link for mobile
    $form['box1']['account_manager_link'] = array(
        '#type' => 'textfield',
        '#title' => t('Account manager link'),
        '#default_value' => $account_manager_link ? $account_manager_link : 'https://oss.ticketmaster.com/html/home.htmI?l=EN&team=falcons',
        '#required' => TRUE,
    );
    
    $date_year = date('Y');
          $footer_default = '<div>
    <p>©'.$date_year.' <a href="http://www.io-media.com/" target="_blank">IOMEDIA, Inc.</a> All Rights Reserved. “Season Ticket Portal” and the “STP” shield are registered trademarks of IOMEDIA, Inc.</p>
    </div>';
    if ($stp_mob_footer) {
      $stp_mob_footer = str_replace('2016', $date_year, $stp_mob_footer);
    }
   $form['box1']['stp_mob_footer'] = array(
        '#type' => 'textarea',
        '#title' => t('Mobile Footer Text'),        
        '#default_value' => $stp_mob_footer ? $stp_mob_footer : $footer_default,
        '#required' => TRUE,
    );
   //Images required
    $form['box2'] = array(
        '#type' => 'fieldset',
        '#title' => t('Logos & Images'),
        '#weight' => 2,
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
    );
   $form['box2']['mob_logo'] = array(
    '#type' => 'managed_file',
    '#name' => 'mob_logo',
    '#title' => t('Logo'),
    '#default_value' => $mob_logo ? $mob_logo : '',
    '#upload_location' => 'public://',
    '#upload_validators' => array(
                            'file_validate_extensions' => array('jpg png gif jpeg'),
                            'iom_color_file_validate_logo' => array('0x96', '0x96'),
                           ),
     '#description' => t('Logo Image resolution must be 0x59, 0 is any width'),
    );   
    $form['box2']['mob_splash_img_home'] = array(
    '#type' => 'managed_file',
    '#name' => 'mob_service_img_home',
    '#title' => t('Splash Image - Portrait Mode'),
    '#default_value' => $mob_splash_img_home ? $mob_splash_img_home : '',
    '#upload_location' => 'public://',
    '#upload_validators' => array(
                            'file_validate_extensions' => array('jpg png gif jpeg'),
                            'iom_color_file_validate_onfly' => array('640x1138', '640x1138'),
                           ),
    '#description' => t('Maximum and minimum resolution is 640x1138'),
    
    );
    $form['box2']['brand_color'] = array(
        '#type' => 'managed_file',
    '#name' => 'brand_color',
    '#title' => t('Splash Image - Landscape Mode'),
    '#default_value' => $brand_color ? $brand_color : '',
    '#upload_location' => 'public://',
    '#upload_validators' => array(
                            'file_validate_extensions' => array('jpg png gif jpeg'),
                            'iom_color_file_validate_onfly' => array('1138x640', '1138x640'),
                           ),
    '#description' => t('Maximum and minimum resolution is 1138x640'),
    );
   $form['box2']['sign_in_bg_image'] = array(
    '#type' => 'managed_file',
    '#name' => 'sign_in_bg_image',
    '#title' => t('Sign In BG Image'),
    '#default_value' => $sign_in_bg_image ? $sign_in_bg_image : '',
    '#upload_location' => 'public://',
    '#upload_validators' => array(
                            'file_validate_extensions' => array('jpg png gif jpeg'),
                            'iom_color_file_validate_onfly' => array('640x1136', '640x1136'),
                           ),
     '#description' => t('Maximum and minimum resolution is 640x1136'),
    );
   
   //11/11
   $form['box2']['mob_header_top_bg'] = array(
    '#type' => 'managed_file',
    '#name' => 'mob_header_top_bg',
    '#title' => t('Header top BG Image'),
    '#default_value' => $mob_header_top_bg ? $mob_header_top_bg : '',
    '#upload_location' => 'public://',
    '#upload_validators' => array(
                            'file_validate_extensions' => array('jpg png gif jpeg'),
                            'iom_color_file_validate_onfly' => array('640x80', '640x80'),
                           ),
    '#description' => t('Maximum and minimum resolution is 640x80'),
    );
   $form['box2']['sitebar_top_landing_logo'] = array(
    '#type' => 'managed_file',
    '#name' => 'sitebar_top_landing_logo',
    '#title' => t('Sidebar Top Brand Logo'),
    '#default_value' => $sitebar_top_landing_logo ? $sitebar_top_landing_logo : '',
    '#upload_location' => 'public://',
    '#upload_validators' => array(
                            'file_validate_extensions' => array('jpg png gif jpeg'),
                            'iom_color_file_validate_onfly' => array('510x81', '510x81'),
                           ),
    '#description' => t('Maximum and minimum resolution is 510x81'),
    );
  
   $form['box2']['sitebar_stp_logo'] = array(
    '#type' => 'managed_file',
    '#name' => 'sitebar_stp_logo',
    '#title' => t('Sidebar STP top Logo'),
    '#default_value' => $sitebar_stp_logo ? $sitebar_stp_logo : '',
    '#upload_location' => 'public://',
    '#upload_validators' => array(
                            'file_validate_extensions' => array('jpg png gif jpeg'),
                            'iom_color_file_validate_onfly' => array('191x43', '191x43'),
                           ),
    '#description' => t('Maximum and minimum resolution is 191x43'),
    );
    $form['box2']['stp_footer_logo'] = array(
        '#type' => 'managed_file',
        '#title' => t('STP footer Image for mobile'),
        '#default_value' => $stp_footer_logo ? $stp_footer_logo : '',
        '#upload_location' => 'public://',
        '#upload_validators' => array(
                                'file_validate_extensions' => array('jpg png gif jpeg'),
                                'iom_color_file_validate_onfly' => array('54x45', '54x45'),
                               ),
        '#description' => t('Maximum and minimum resolution is 54x45'),       
    );
    $form['box2']['stp_calendar_mob_bg'] = array(
    '#type' => 'managed_file',
    '#name' => 'stp_calendar_mob_bg',
    '#title' => t('Calendar BG Image'),
    '#default_value' => $stp_calendar_mob_bg ? $stp_calendar_mob_bg : '',
    '#upload_location' => 'public://',
    '#upload_validators' => array(
                            'file_validate_extensions' => array('jpg png gif jpeg'),
                            'iom_color_file_validate_onfly' => array('640x506', '640x506'),
                           ),
    '#description' => t('Maximum and minimum resolution is 640x506'),
    );
    $form['box2']['sth_event_logo'] = array(
    '#type' => 'managed_file',
    '#name' => 'sth_event_logo',
    '#title' => t('STH EVENT LOGO'),
    '#default_value' => $sth_event_logo ? $sth_event_logo : '',
    '#upload_location' => 'public://',
    '#upload_validators' => array(
                            'file_validate_extensions' => array('jpg png gif jpeg'),
                            'iom_color_file_validate_logo' => array('0x61', '0x61'),
                           ),
     '#description' => t('Image resolution must be 0x61, 0 is any width'),
    );
    //CSS variables
    $form['box3'] = array(
        '#type' => 'fieldset',
        '#title' => t('Styles and Color'),
        '#weight' => 3,
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
        '#prefix' => '<div id="mobile-styles">',
        '#suffix' => '</div>',
    );
    $form['box3']['top_header_button_bg_color'] = array(
      '#type' => 'textfield',
      '#title' => t('Back button BG color'),
      '#default_value' => $top_header_button_bg_color ? $top_header_button_bg_color : '#fdc82f',
      '#attributes'=>array('id'=>'top_header_button_bg_color'),
    );

    $form['box3']['header_bg_color'] = array(
        '#type' => 'textfield',
        '#title' => t('Top Header image fallback BG Color'),
        '#default_value' => $header_bg_color ? $header_bg_color : '#000000',
        '#attributes'=>array('id'=>'header_bg_color'),
    );
    $form['box3']['header_border_bottom_color'] = array(
        '#type' => 'textfield',
        '#title' => t('Top Header border bottom color'),
        '#default_value' => $header_border_bottom_color ? $header_border_bottom_color : '#000000',
        '#attributes'=>array('id'=>'header_border_bottom_color'),
    );
 
    /*$form['box3']['brand_color'] = array(
        '#type' => 'textfield',
        //'#title' => t('Hamburger side bar STP logo BG color'),
        '#title' => t('Brand color'),
        '#default_value' => $brand_color ? $brand_color : '#512b1b',
        '#attributes'=>array('id'=>'brand_color'),
    );*/
     $form['box3']['site_color'] = array(
        '#type' => 'textfield',
        '#title' => t('Hamburger icon/brand color'),
        '#default_value' => $site_color ? $site_color : '#fdc82f',
        '#attributes'=>array('id'=>'site_color'),
    );
     $form['box3']['mob_pager_circle'] = array(
        '#type' => 'textfield',
        '#title' => t('Pager circle color'),
        '#default_value' => $mob_pager_circle ? $mob_pager_circle : '#fdc82f',
        '#attributes'=>array('id'=>'mob_pager_circle'),
    );
    $form['box3']['side_bar_stp_logo_bg'] = array(
        '#type' => 'textarea',
        '#title' => t('Side bar top STP logo BG color'). ' ('.$gradient_picker.')',
        '#default_value' => $side_bar_stp_logo_bg ? $side_bar_stp_logo_bg : 'background: linear-gradient(to bottom, #310302 0%, #000000 100%);',
    );
    $form['box3']['side_bar_bg'] = array(
        '#type' => 'textfield',
        '#title' => t('Side bar BG color'),
        '#default_value' => $side_bar_bg ? $side_bar_bg : '#562714',
        '#attributes'=>array('id'=>'side_bar_bg'),
    );
    $form['box3']['side_bar_bg_border_rt'] = array(
        '#type' => 'textfield',
        '#title' => t('Side bar BG border right color'),
        '#default_value' => $side_bar_bg_border_rt ? $side_bar_bg_border_rt : '#787878',
        '#attributes'=>array('id'=>'side_bar_bg_border_rt'),
    );
    $form['box3']['side_bar_bg_border_bottom'] = array(
        '#type' => 'textfield',
        '#title' => t('Side bar BG bottom border color'),
        '#default_value' => $side_bar_bg_border_bottom ? $side_bar_bg_border_bottom : '#9a8035',
        '#attributes'=>array('id'=>'side_bar_bg_border_bottom'),
    );   
    

    $form['box3']['menu_item_text_color'] = array(
      '#type' => 'textfield',
      '#title' => t('Side bar menu items text color'),
      '#default_value' => $menu_item_text_color ? $menu_item_text_color : '#ffffff;',
      '#attributes'=>array('id'=>'menu_item_text_color'),
    );
    
    $form['box3']['menu_item_bg_color'] = array(
      '#type' => 'textfield',
      '#title' => t('Side bar menu items bg color'),
      '#default_value' => $menu_item_bg_color ? $menu_item_bg_color : 'rgba(0, 0, 0, 0.5)',
      '#attributes'=>array('id'=>'menu_item_bg_color'),
    );
    $form['box3']['menu_items_border'] = array(
      '#type' => 'textfield',
      '#title' => t('Side bar menu items border color'),
      '#default_value' => $menu_items_border ? $menu_items_border : '#383838',
      '#attributes'=>array('id'=>'menu_items_border'),
    );
    $nav_gradient = $nav_gradient ? $nav_gradient : 'background: -moz-linear-gradient(#785f16 0%, #f7c32e 100%);
    background: -webkit-gradient(linear, left, right, color-stop(0%, #785f16), color-stop(100%, #f7c32e));
    background: -webkit-linear-gradient(#785f16 0%, #f7c32e 100%);
    background: -o-linear-gradient(#785f16 0%, #f7c32e 100%);
    background: -ms-linear-gradient(#785f16 0%, #f7c32e 100%);
    background: linear-gradient(to right, #785f16 0%, #f7c32e 100%);';

    $form['box3']['nav_gradient'] = array(
        '#type' => 'textarea',
        '#title' => t('Side bar menu gradient'). ' ('.$gradient_picker.')',
        '#default_value' => $nav_gradient,
    );
    
    $form['box4'] = array(
      '#type' => 'fieldset',
      '#title' => t('Calendar and STH Event'),
      '#weight' => 4,
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#prefix' => '<div id="calendar">',
      '#suffix' => '</div>',        
    );

    $form['box4']['home_game_legend'] = array(
      '#type' => 'textfield',
      '#title' => t('Home Game Legend'),
      '#default_value' => $home_game_legend ? $home_game_legend : '#fdc82f',
      '#attributes'=>array('id'=>'home_game_legend'),
    );
   
    $form['box4']['away_game_legend'] = array(
      '#type' => 'textfield',
      '#title' => t('Away Game Legend'),
      '#default_value' => $away_game_legend ? $away_game_legend : '#ffffff',
      '#attributes'=>array('id'=>'away_game_legend'),
    );
    $form['box4']['home_game_color'] = array(
      '#type' => 'textfield',
      '#title' => t('Home Game Tabular BG'),
      '#default_value' => $home_game_color ? $home_game_color : 'rgba(253, 200, 47, 0.75)',
      '#attributes'=>array('id'=>'home_game_color'),
    );
    $form['box4']['away_game_color'] = array(
      '#type' => 'textfield',
      '#title' => t('Away Game Tabular BG'),
      '#default_value' => $away_game_color ? $away_game_color : 'rgba(255, 255, 255, 0.6)',
      '#attributes'=>array('id'=>'away_game_color'),
    );
    $form['box4']['cal_today_bg_color'] = array(
      '#type' => 'textfield',
      '#title' => t('Today BG color'),
      '#default_value' => $cal_today_bg_color ? $cal_today_bg_color : 'rgba(253, 200, 47, 0.5)',
      '#attributes'=>array('id'=>'cal_today_bg_color'),
    );
    
    //11/11
    $form['box4']['cal_table_header_color'] = array(
      '#type' => 'textarea',
      '#title' => t('Table header color'). ' ('.$gradient_picker.')',
      '#default_value' => $cal_table_header_color ? $cal_table_header_color : 'background: linear-gradient(to bottom, #ecc304 0%, #bb9a03 100%); ',
      '#attributes'=>array('id'=>'cal_table_header_color'),
    );   
   $form['box4']['cal_sth_event_color'] = array(
      '#type' => 'hidden',
      '#title' => t('STH Events BG color'),
      '#default_value' => $cal_sth_event_color ? $cal_sth_event_color : '',
      '#attributes'=>array('id'=>'cal_sth_event_color'),
    );
   $form['box4']['cal_sth_event_logobg'] = array(
      '#type' => 'textfield',
      '#title' => t('STH Event Logo BG'),
      '#default_value' => $cal_sth_event_logobg ? $cal_sth_event_logobg : '#fdc82f',
      '#attributes'=>array('id' => 'cal_sth_event_logobg'),
    );
    $cal_sth_event_bg_gradiant = $cal_sth_event_bg_gradiant ? $cal_sth_event_bg_gradiant : 'background: #ebebeb;
    background: -moz-linear-gradient(top, #ebebeb 0%, #b3b3b3 100%);
    background: -webkit-gradient(linear, left top, left bottom, color-stop(0%, #ebebeb), color-stop(100%, #b3b3b3));
    background: -webkit-linear-gradient(top, #ebebeb 0%, #b3b3b3 100%);
    background: -o-linear-gradient(top, #ebebeb 0%, #b3b3b3 100%);
    background: -ms-linear-gradient(top, #ebebeb 0%, #b3b3b3 100%);
    background: linear-gradient(to bottom, #ebebeb 0%, #b3b3b3 100%);';
    $form['box4']['cal_sth_event_bg_gradiant'] = array(
      '#type' => 'textarea',
        '#title' => t('STH Event Header Top BG Gradient'). ' ('.$gradient_picker.')',
      '#default_value' => $cal_sth_event_bg_gradiant,
      '#attributes'=>array('id' => 'cal_sth_event_bg_gradiant'),
    );
    
    $cal_sth_event_top_bg_gradiant = $cal_sth_event_top_bg_gradiant ? $cal_sth_event_top_bg_gradiant : 'background: #fdc82f;
background: -moz-linear-gradient(#fdc82f 0%, #161717 100%);
background: -webkit-gradient(linear, left, right, color-stop(0%, #fdc82f), color-stop(100%, #161717));
background: -webkit-linear-gradient(#fdc82f 0%, #161717 100%);
background: -o-linear-gradient(#fdc82f 0%, #161717 100%);
background: -ms-linear-gradient(#fdc82f 0%, #161717 100%);
background: linear-gradient(to right, #fdc82f 0%, #161717 100%);';
    $form['box4']['cal_sth_event_top_bg_gradiant'] = array(
      '#type' => 'textarea',
        '#title' => t('STH Event Header Top Border Gradient'). ' ('.$gradient_picker.')',
      '#default_value' => $cal_sth_event_top_bg_gradiant,
      '#attributes'=>array('id' => 'cal_sth_event_top_bg_gradiant'),
    );
    $form['box5'] = array(
      '#type' => 'fieldset',
      '#title' => t('Buttons'),
      '#weight' => 5,
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#prefix' => '<div id="mobile-buttons">',
      '#suffix' => '</div>',        
    );
      $form['box5']['mob_global_btn_color'] = array(
        '#type' => 'textfield',
        '#title' => t('Text Color'),
        '#default_value' => $mob_global_btn_color ? $mob_global_btn_color : '#ffffff',
        '#attributes'=>array('id'=>'mob_global_btn_color'),
      );     
      $form['box5']['mob_global_btn_blk_border_color'] = array(
        '#type' => 'textfield',
        '#title' => t('Border Color'),
        '#default_value' => $mob_global_btn_blk_border_color ? $mob_global_btn_blk_border_color : '#000000',
        '#attributes'=>array('id'=>'mob_global_btn_blk_border_color'),
      );
      $default_button_gradient = $default_button_gradient ? $default_button_gradient : 'background-image: linear-gradient(to bottom, #f5c904 0%, #ecc403 18%, #eac105 20%, #cea903 54%, #b89903 75%, #aa8d03 100%);
    background-image: linear-gradient(to bottom, #f5c904 0%, #ecc403 18%, #eac105 20%, #cea903 54%, #b89903 75%, #aa8d03 100%);
    background-image: linear-gradient(to bottom, #f5c904 0%, #ecc403 18%, #eac105 20%, #cea903 54%, #b89903 75%, #aa8d03 100%);';
   
    $form['box5']['default_button_gradient'] = array(
        '#type' => 'textarea',
        '#title' => t('Default Button Background Gradient'). ' ('.$gradient_picker.')',
        '#default_value' => $default_button_gradient,
    );
      $form['box5']['mob_global_btn_active_color'] = array(
        '#type' => 'textfield',
        '#title' => t('Default Button Active/Focus Color'),
        '#default_value' => $mob_global_btn_active_color ? $mob_global_btn_active_color : '#aa8d03',
        '#attributes'=>array('id'=>'mob_global_btn_active_color'),
      );
          
    $form['box5']['mob_invoice_btn_color'] = array(
        '#type' => 'textfield',
        '#title' => t('Invoice Account ID text color'),
        '#default_value' => $mob_invoice_btn_color ? $mob_invoice_btn_color : '#fdc82f',
        '#attributes'=>array('id'=>'mob_invoice_btn_color'),
      );

    $form['box6'] = array(
      '#type' => 'fieldset',
      '#title' => t('Additional'),
      '#weight' => 6,
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#prefix' => '<div id="Additional-css">',
      '#suffix' => '</div>',        
    );
    $form['box6']['addtional_css_overwrite'] = array(
      '#type' => 'textarea',
      '#title' => t('Additional CSS for overwrite on specific changes'),
      '#default_value' => $addtional_css_overwrite ? $addtional_css_overwrite : '',
    );   
  
    $form['submit'] = array(
     '#type' => 'submit',
     '#weight' => 6,
     '#value' => 'Submit',
    );
       
    return $form;

}

/**
 * Validate only width and height of the image.
 */
function iom_color_file_validate_onfly(stdClass $file, $maximum_dimensions = 0, $minimum_dimensions = 0){
  $errors = array();
return $errors;
  // Check first that the file is an image.
  if ($info = image_get_info($file->uri)) {
    if ($maximum_dimensions) {
      // Check that it is smaller than the given dimensions.
      list($width, $height) = explode('x', $maximum_dimensions);
      if ($info['width'] > $width || $info['height'] > $height) {        
          $errors[] = t('The image is too large; the maximum dimensions are %dimensions pixels.', array('%dimensions' => $maximum_dimensions));
          return $errors;
      }
    }

    if ($minimum_dimensions) {
      // Check that it is larger than the given dimensions.
      list($width, $height) = explode('x', $minimum_dimensions);
      if ($info['width'] < $width || $info['height'] < $height) {
        $errors[] = t('The image is too small; the minimum dimensions are %dimensions pixels.', array('%dimensions' => $minimum_dimensions));
        return $errors;
      }
    }
  }
  return $errors;
}

/**
 * Validate only height of the image.
 */
function iom_color_file_validate_logo(stdClass $file, $maximum_dimensions = 0, $minimum_dimensions = 0){
  $errors = array();
return $errors;
  // Check first that the file is an image.
  if ($info = image_get_info($file->uri)) {
    if ($maximum_dimensions) {
      $res = explode('x', $maximum_dimensions);
      $width = $res[0];
      $height = $res[1];
      if ($info['height'] > $height) {        
          $errors[] = t('The image is too large; the maximum dimensions are %dimensions pixels.', array('%dimensions' => $maximum_dimensions));
          return $errors;
      }
    }

    if ($minimum_dimensions) {
      if ($info['height'] < $height) {
        $errors[] = t('The image is too small; the minimum dimensions are %dimensions pixels.', array('%dimensions' => $minimum_dimensions));
        return $errors;
      }
    }
  }
  return $errors;
}

/**
 * Hook_form_submit().
 * Purpose: submit all files and updated data in table. 
 */
function iom_color_picker_mobile_setting_form_submit($form, &$form_state){
  global $user;
  $data_fields = iom_color_picker_get_brands_fields_mobile();

  //Make file object and save permanent
  $files = array();
  $files['mob_logo'] = file_load($form_state['values']['mob_logo']);
  $files['mob_splash_img_home'] = file_load($form_state['values']['mob_splash_img_home']);
  $files['mob_splash_landscape_img'] = file_load($form_state['values']['brand_color']);
  $files['sign_in_bg_image'] = file_load($form_state['values']['sign_in_bg_image']);
  $files['stp_calendar_mob_bg'] = file_load($form_state['values']['stp_calendar_mob_bg']);
  $files['mob_header_top_bg'] = file_load($form_state['values']['mob_header_top_bg']);
  $files['sitebar_top_landing_logo'] = file_load($form_state['values']['sitebar_top_landing_logo']);
  $files['sitebar_stp_logo'] = file_load($form_state['values']['sitebar_stp_logo']);
  $files['sth_event_logo'] = file_load($form_state['values']['sth_event_logo']);
  $files['mob_fav_icon'] = file_load($form_state['values']['mob_fav_icon']);
  $files['stp_footer_logo'] = file_load($form_state['values']['stp_footer_logo']);

  variable_set('en_mob_site', $form_state['values']['en_mob_site']);
  variable_set('mob_domain', $form_state['values']['mob_domain']);
  variable_set('stp_buypage_url', $form_state['values']['stp_buypage_url']);
  variable_set('account_manager_link', $form_state['values']['account_manager_link']);
  variable_set('stp_mob_footer', $form_state['values']['stp_mob_footer']);
  variable_set('en_theater_calendar', $form_state['values']['en_theater_calendar']);

  $f_record = array();
  $record = array();
  foreach ($files as $key=>$file){
     if ($file) {
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      $record[$key] = $file->fid;
      file_usage_add($file, 'iom_color', $key, $file->fid);
     }
  }
  
  foreach ($data_fields as $key => $val) {
    //Dynamically create blank variable;
    if (in_array($val, array_keys($form_state['values']))) {
      $mobile_css[$val] = $form_state['values'][$val] ? $form_state['values'][$val] : '';
    }
  }
  $record['css'] = serialize($mobile_css);
  $record['uid'] = $user->uid;
  $record['timestamp'] = time();
  $key_id = array();
  if (isset($record['mid']) && $record['mid']){
     $key_id = 'mid'; 
     //if you want to keep the last updated and new record will be created always uncomment below two lines.
     //$key_id = array();
     //unset($record['mid']);     
   }
   
   //Merge files array with other fields
   //$record = array_merge($record, $f_record);
   $result = drupal_write_record('tbl_manage_branding_mobile', $record, $key_id);   
   if ($result){
     $file_created = iom_color_settings_create_mobile_css($form_state['values']);     
   }
   switch($result){
    case 1:
      drupal_set_message(t('A new record has been created successfully'));
      if ($file_created){
       drupal_set_message(t('A new mobile css has been added successfully'));
      }
    break;
    case 2:
      drupal_set_message(t('Record has been updated successfully'));
      if ($file_created){
       drupal_set_message(t('Mobile css has been updated successfully'));
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
function iom_color_picker_get_brands_fields(){ 
 $brand_fields = array(
   'site_title_color',
   'site_container_color',
   'buttons_color',
   'buttons_bg_hover_color',
   'buttons_boxshadow_color',
   'buttons_border_color',   
   'buttons_default_color_gradiant',
   'buttons_border_hover_color',
   'buttons_text_hover_color',   
   'global_buttons_color',
   'global_buttons_hover_color',
   'global_buttons_boxshadow_color',
   'global_buttons_border_color',   
   'global_buttons_border_hover_color',
   'global_buttons_bg',
   'global_buttons_gradiant_bg',
   'video_btn_bg_color',
   'video_btn_bg_hover_color',
   'nav_top_border_color',
   'nav_top_background_shadow',
   'nav_item_hover_border',
   'nav_font_color',
   'nav_drop_toggle_color',
   'nav_drop_down_item_font_color',
   'nav_drop_down_item_hover',
   'nav_default_gradiant',
   'nav_gradiant_hover',
   'nav_drop_down_background',
   'nav_drop_down_background_shadow',
   'promo_color',
   'promo_hover_color',
   'promo_title_color',
   'calender_lr_bg',
   'calender_lr_border',
   'calender_events_heading_bg',
   'calender_home_game_bg',
   'calender_home_border',
   'calender_away_game_bg',
   'calender_home_game_squre', 
   'calender_away_game_squre',
   'calender_listview_bg',
   'calender_listview_bg_hover',
   'addtional_css_overwrite',
   'contact_us_bg_color',
   'timestamp',
   'status',
   'uid',
   'cid',
  );
  return $brand_fields;
}

/**
 * //These fields are related to db field.
 * Purpose: Needs these field to display current value in the text area/text fields,
   also required for initialize of variable before display to.
 */
function iom_color_picker_get_brands_fields_mobile(){
 
 $brand_fields = array(
'mid',
'en_mob_site',
'stp_mob_footer',
'stp_buypage_url',
'account_manager_link',
'mob_domain',
'mob_logo',
'mob_splash_img_home',
'sign_in_bg_image',
'mob_header_top_bg',
'sitebar_top_landing_logo',
'sitebar_stp_logo',
'stp_footer_logo',
'stp_calendar_mob_bg',
'sth_event_logo',
'top_header_button_bg_color',
'header_bg_color',
'header_border_bottom_color',
'brand_color',
'site_color',
'side_bar_stp_logo_bg',
'side_bar_bg',
'side_bar_bg_border_rt',
'side_bar_bg_border_bottom',
'mob_pager_circle',
'nav_gradient',
'menu_item_text_color',
'menu_item_bg_color',
'menu_items_border',
'home_game_legend',
'away_game_legend',
'home_game_color',
'away_game_color',
'cal_today_bg_color',
'cal_table_header_color',
'cal_sth_event_color',
'cal_sth_event_bg_gradiant',
'cal_sth_event_logobg',
'cal_sth_event_top_bg_gradiant',
'mob_global_btn_color',
'mob_global_btn_blk_border_color',
'mob_global_btn_active_color',
'default_button_gradient',
'mob_invoice_btn_color',
'addtional_css_overwrite',
'timestamp',
'uid',
  );
  return $brand_fields;
}

/*
function iom_color_picker_add_reqired_js(){
  drupal_add_js('misc/collapse.js');
  $path = drupal_get_path('module', 'iom_color_picker');
  $color_path = $path ."/color";
  $js_options = array('cache'=>FALSE,'preprocess'=>FALSE,'requires_jquery'=>FALSE);
  drupal_add_css($color_path."/css/bootstrap.min.css", array('cache'=>FALSE,'preprocess'=>FALSE));
  //drupal_add_css($color_path."/css/bootstrap.css", array('cache'=>FALSE,'preprocess'=>FALSE));

  drupal_add_css($color_path."/dist/css/bootstrap-colorpicker.min.css", array('cache'=>FALSE,'preprocess'=>FALSE));
  drupal_add_js($color_path."/js/bootstrap.min.js", $js_options);
  drupal_add_js($color_path."/dist/js/bootstrap-colorpicker.js", $js_options);
  drupal_add_js($path."/js/colorjs.js", $js_options);
} */

