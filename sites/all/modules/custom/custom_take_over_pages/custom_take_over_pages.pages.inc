<?php
 function take_over_pages_view(){
    $output = '';
    $type = "take_over_pages";
    $nodes = node_load_multiple(array(), array('type' => $type)); 
    foreach($nodes as $products){
        $output .= '<div class="custom-form-tk-node-'.$products->nid.' custom-form-tk-node">'.render(drupal_get_form('custom_take_over_pages_form',$products->nid)).'</div>';
    }     
    return $output; 
 }

 
 
 
//form to create Take over Page
function custom_add_take_over($form, $form_state){
    //drupal_add_js('http://code.jquery.com/ui/1.11.2/jquery-ui.js');
     drupal_add_js('jQuery(document).ready(function () {
            jQuery(".form-item-layout .form-radio").change(function() {
                if (jQuery(this).attr("checked")) {
                    jQuery(".to-img1").addClass("active");
                    jQuery(".to-img2").removeClass("active");
                }
                else {
                    jQuery(".to-img2").addClass("active");
                    jQuery(".to-img1").removeClass("active");
                }
            });             
        });',
    array('type' => 'inline', 'scope' => 'footer', 'weight' => 5)
  );
     
    global $base_path;
    $form['title'] = array(
        '#type' => 'textfield',
        '#title' => t('Title'),
        '#required' => true,
    );
    
    $dateformat = 'Y-m-d';
    $form['start_date'] = array(
        '#type' => 'date_popup',
        '#title' => t('Start Date'),
        '#required' => true,
        '#date_label_position' => '',
        '#date_format' => $dateformat,
    );
    $form['end_date'] = array(
        '#type' => 'date_popup',
        '#title' => t('End Date'),
        '#required' => true,
        '#date_label_position' => '',        
        '#date_format' => $dateformat,
    );
    
    $cuurence = array(1 => t('Single'), 2 => t('Multiple'));
    $form['takeover_occurences'] = array(
        '#type' => 'radios',
        '#title' => t('Takeover Occurences'),
        '#default_value' => 1,   
        '#options' => $cuurence,
    );
    
    $path = drupal_get_path('theme','sports');
    $layout = array(1 => t('Top And Bottom Text With Single Button').'<div class="to-img1 active"><img src="'.$base_path.$path.'/images/layout-1.png'.'"></div>', 2 => t('Top And Bottom Text With Two Buttons').'<div class="to-img2"><img src="'.$base_path.$path.'/images/layout-2.png'.'"></div>');
    $form['layout'] = array(
        '#type' => 'radios',
        '#title' => t('Select Layout'),
        '#default_value' => 1,   
        '#options' => $layout,
    );
    
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Create and move to edit'),
    );
    return $form;
}



function custom_add_take_over_validate($form, $form_state) {
    $val = $form_state['values']; 
    $time = time();
    if(strtotime($val['start_date']) < $time){
        form_set_error('start_date','Start Date can\'t be past date');
    }
    if(strtotime($val['end_date']) < $time){
        form_set_error('start_date','End Date can\'t be past date');
    }
    if(strtotime($val['start_date']) > strtotime($val['end_date'])){
        form_set_error('start_date','Start Date can\'t be bigger then End Date');
    }
    
}
function custom_add_take_over_submit($form, $form_state) {
    global $user,$base_url;
    $val = $form_state['values'];
    
    $node = new stdClass();
    $node->title = $val['title'];
    $node->type = "take_over_pages";
    node_object_prepare($node); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
    $node->language = LANGUAGE_NONE; // Or e.g. 'en' if locale is enabled
    $node->uid = $user->uid;
    $node->status = 1; //(1 or 0): published or not
    $node->promote = 0; //(1 or 0): promoted to front page
    $node->comment = 1; // 0 = comments disabled, 1 = read only, 2 = read/write
    $node->body[$node->language][0]['value'] = $node->field_bottom_text[$node->language][0]['value']  = t('Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged.');  
    $node->field_take_over_date[$node->language][0]['value'] = date('Y-m-d',strtotime($val['start_date'])).' 00:00:00'; //2014-10-29 date('Y-m-d',strtotime($val['start_date']))
    $node->field_take_over_date[$node->language][0]['value2'] = date('Y-m-d',strtotime($val['end_date'])).' 00:00:00'; //date_popup
    $node->field_occurence[$node->language][0]['value'] = $val['takeover_occurences'];
    $node->field_button_1[$node->language][0]['title'] = t('button 1');
    $node->field_button_1[$node->language][0]['url'] = 'http://www.io-media.com/';
    $node->field_button_1[$node->language][0]['attributes']['target'] = '_blank';
    if($val['layout'] == 2) {
        $node->field_button_2[$node->language][0]['title'] = t('button 2');
        $node->field_button_2[$node->language][0]['url'] = 'http://www.io-media.com/';    
        $node->field_button_2[$node->language][0]['attributes']['target'] = '_blank';        
    }
    $node = node_submit($node); // Prepare node for saving
    node_save($node);    
        
    $a = array('query' => array('edit' =>'true'));
    drupal_goto('node/'.$node->nid, $a);
} 


function take_over_page_config($form, $form_state){
    $form['birth_nid'] = array(
        '#type' => 'textfield',
        '#title' => t('Birth NID'),
        '#description' => t('NID for the Birthday page'),
        '#default_value' => variable_get('birth_nid',0),
    );
    $form['pawp_nid'] = array(
        '#type' => 'textfield',
        '#title' => t('PAWP NID'),
        '#description' => t('NID for the PAWP page'),
        '#default_value' => variable_get('pawp_nid',0),
    );
    return system_settings_form($form);
}

?>