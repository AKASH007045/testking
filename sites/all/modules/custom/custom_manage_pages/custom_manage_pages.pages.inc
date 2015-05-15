<?php
function custom_manage_pages_callback(){  
  drupal_add_css(drupal_get_path('module', 'custom_manage_pages') . '/css/tree.css');
  drupal_add_js(drupal_get_path('module', 'custom_manage_pages') . '/js/tree.js', 'file');
  drupal_add_js(drupal_get_path('module', 'custom_manage_pages') . '/js/nest.js', 'file');
  
  $markup = getListHtml();
  
  $form = array();
  
  $form['manage_pages'] = array(
    '#type' => 'fieldset',
    '#title' => t('Manage Pages'),
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => 1
  );
  
  $form['editset'] = array(
    '#type' => 'fieldset',
    '#title' => t(''),
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => 2
  );
  
  /*$form['title'] = array(
    '#type' => 'item',
    '#markup' => 'Manage Pages',
    '#weight' => 0
  );*/
  $form['manage_pages']['tree_data'] = array(
    '#type' => 'hidden',
    '#attributes' => array('id' => 'navlist_input'),
    '#weight' => 3
  );
  $form['manage_pages']['form_data'] = array(
    '#type' => 'hidden',
    '#attributes' => array('id' => 'form_data_input'),
    '#weight' => 4
  );
  
  $form['manage_pages']['nav_list'] = array(
    '#type' => 'item',
    '#markup' => $markup,
    '#weight' => 5
  );
  
  $form['editset']['submit'] = array(
    '#type' => 'submit', 
    '#value' => t('Save'),
    '#weight' => 6
  );
  
  //return "<pre>".print_r(array_keys($menu), true)."</pre>";
  return $form;
}

function delete_manage_pages($mlid=NULL){
  if(!empty($mlid)){
    $link = menu_link_load($mlid);    
    //Make sure it is node
    if(strstr($link['href'], 'node')){
      $nid = str_replace("node/", "", $link['href']);
      menu_link_delete($mlid);
      node_delete($nid);
    }
    else{
      updateMenuLinks($mlid, '868', $link['link_title'], $link['depth'], $link['weight']);
    }
    //Rebuild menus
    menu_rebuild();
    drupal_set_message("Menu item deleted Successfully");
    drupal_goto('/admin/manage/manage-pages');
  }
}

function custom_manage_pages_callback_submit($form, &$form_state){
  $formData = parseData("smp", $form_state['values']['form_data']); 
  $jsonData = parseData("json", $form_state['values']['tree_data']); 
  /*echo "<pre>"; 
  print_r($_POST);
  print_r($jsonData);
  echo "</pre>";
  exit;
  */
  $weight = -50;
  $parentWeight = -50;

  foreach($jsonData as $jsonItem){    
    if(isset($jsonItem->children)){
      $weight = -50;
      foreach($jsonItem->children as $child){
          updateMenuLinks($child->id, $jsonItem->id, t($_POST['title_'.$child->id]), t($_POST['depth_'.$child->id]), $weight);
          $weight++;
      }
    }
  }
  menu_rebuild();
  drupal_set_message("Menu updated Successfully");
}

function updateMenuLinks($mlid=NULL, $parentID=NULL, $title=NULL, $depth=NULL, $weight=NULL){
  $menu_link = menu_link_load($mlid);
  if($depth==1){
    $menu_link['p1'] = 0;
  }
  elseif($depth==2){
    $menu_link['p1'] = $parentID;
    $menu_link['p2'] = $mlid;
  }
  $menu_link['plid'] = !empty($parentID)?$parentID:$menu_link['plid'];
  $menu_link['link_title'] = !empty($title)?$title:$menu_link['link_title'];
  $menu_link['depth'] = !empty($depth)?$depth:$menu_link['depth'];
  $menu_link['weight'] = !empty($weight)?$weight:$menu_link['weight'];
  $result = menu_link_save($menu_link); 
  menu_rebuild();
  return $result;
}

function parseData($format="json", $data=NULL){
  if($format=="json" && !empty($data)){
    $parsedData = json_decode($data);
  }
  else{
    $tempVars = array();
    $tempArr = explode("&", $data);
    
    foreach($tempArr as $value){
      $tmpValue = explode("=", $value);
      $tempVars[$tmpValue[0]] = str_replace("+", " ", $tmpValue[1]);
    }
    $parsedData = $tempVars;
  } 
  return $parsedData; 
}
function getAllMenuLinks($item=NULL){
  if(empty($item)){
    return false;
  }
  $sql = "
    SELECT m.load_functions, m.to_arg_functions, m.access_callback, m.access_arguments, m.page_callback, m.page_arguments, m.delivery_callback, m.title, m.title_callback, m.title_arguments, m.type, m.description, ml.*
    FROM {menu_links} ml LEFT JOIN {menu_router} m ON m.path = ml.router_path
    WHERE ml.menu_name = :menu
    ORDER BY p1 ASC, p2 ASC, p3 ASC, p4 ASC, p5 ASC, p6 ASC, p7 ASC, p8 ASC, p9 ASC";
  $result = db_query($sql, array(':menu' => 'main-menu'), array('fetch' => PDO::FETCH_ASSOC));
  $links = array();
  foreach ($result as $item) {
    $links[] = $item;
  }
  $tree = menu_tree_data($links);
  $node_links = array();
  menu_tree_collect_node_links($tree, $node_links);

  if(isset($tree) && count($tree)>0){
    return $tree;
  }
  else{
    return false;
  }
}
function getListHtml(){
  $all_menus = array('main-menu'); //, 'user-menu'
  $nestHtml = "";
  $nestHtml .= '<div class="select"><select><option value="anon">Anonymous</option><option value="loggedin">Logged In</option></select><label>User Type: </label>';
  $headhtml = '<div class="headings"><ul><li class="pages">Pages</li><li class="deletehead">Delete</li><li class="edithead">Edit</li></ul></div>';
  $nestHtml .= $headhtml.'<div class="dd" id="nestable"><ol class="dd-list">';
  
  foreach($all_menus as $item){
    $menu = getAllMenuLinks($item);
    foreach($menu as $key => $value){
      if(isset($value['link']['depth']) && $value['link']['depth']==1){
        //Add class to menu items 
        //to distinguish betwween logged in and anonymous accessible items
        $class = "loggedin";
        $value['link']['href'] = $value['link']['link_path'];
        $value['link']['title'] = $value['link']['link_title'];
        $status_text = ($value['link']['hidden'])?'(Disabled)':"";
        
        //If Other is the menu item then hide the edit link
        $shedit = false;
        if($value['link']['mlid']=='868'){
          $shedit = false;
        }
        $anon = drupal_anonymous_user();
        if(strstr($value['link']['href'], 'node')){
          $nid = str_replace("node/", "", $value['link']['href']);
          $node = node_load($nid);   
          if(node_access('view', $node, $anon)){
            $class = "anonymous";
            $shedit = true;
          }
          else{
            $class = "loggedin";
          }
        }
        else{
          if($value['link']['mlid']=='868' || $value['link']['mlid']=='729' || $value['link']['href']=='calendar'){ //hardcoded for calendar page
            $class = "anonymous";
          }
          else{
            $class = "loggedin";
          }
        }
        
        //For triggering edit click on edit links landing pages
        $pth = drupal_get_path_alias($value['link']['href']);
        if($pth=="<front>"){
          $pth = "";
          $alias = ''; ///#edit
        }
        else{
          $alias = '/'.$pth; //.'/#edit'
        }
        
        $chk_title = strtolower($value['link']['title']);
        
        if($chk_title=='contact us'){
          $class = $class.' loggedin';
          //$shedit = false;
        }
        
          $nestHtml .= '<li class="dd-item prnt '.$class.'" data-id="'.$value['link']['mlid'].'">';        
          $nestHtml .= '<div class="dd-handle prnt">'.$value['link']['title'].' '.$status_text.'</div>';
          if($shedit){
            $nestHtml .= '<div class="link-'.$value['link']['mlid'].' accordionChild edit">'.'<a href="'.$alias.'" target="_blank"><i class="fa fa-edit"></i></a>'.'</div>';
          }        
          $nestHtml .= getFormForNavItems($value['link']['mlid'], $value['link']['title'], $value);
          if( isset($value['link']['has_children']) && $value['link']['has_children'] ){
            if( isset($value['below']) && count($value['below'])>0 ){
              $nestHtml .= getChildListHtml($value['below']);
            }
          }
          //$nestHtml .= '<div class="settings"><a class="showdraft" href="javascript:void(0);">show</a></div>';
          $nestHtml .= '</li>';        
      }
    }
  }
  $nestHtml .= '</ol></div>';
  return $nestHtml;
}

function getChildListHtml($childElements=array()){
  $childHtml = '';
  if(isset($childElements) && count($childElements)>0){    
    $childHtml .= '<ol class="dd-list">';
    foreach($childElements as $key => $child){
      $child['link']['href'] = $child['link']['link_path'];
      $child['link']['title'] = $child['link']['link_title'];
      $status_text = ($child['link']['hidden'])?'(Disabled)':"";
      $alias = '/'.drupal_get_path_alias($child['link']['href']).'/#edit';
      
      $chkTitle = strtolower($child['link']['title']);
      //hardcoded to prevent schedule from getting displayed
      if( !strstr($chkTitle, 'schedule') ){
        $childHtml .= '<li class="dd-item" data-id="'.$child['link']['mlid'].'" data-type="menu">';
        $childHtml .= '<div class="dd-handle accordionParent">'.$child['link']['title'] . ' ' . $status_text .'</div>';
        $childHtml .= '<!--<div class="settings"><a class="showdraft" href="javascript:void(0);">show</a></div>-->';
        $childHtml .= '<div class="'.$child['link']['mlid'].' accordionChild">'.getFormForNavItems($child['link']['mlid'], $child['link']['title'], $child).'</div>';
        $childHtml .= '<div class="link-'.$child['link']['mlid'].' accordionChild edit">'.'<a href="'.$alias.'" target="_blank"><i class="fa fa-edit"></i></a>'.'</div>';
        $childHtml .= '<div class="link-'.$child['link']['mlid'].' accordionChild delete">'.'<a href="/admin/manage/delete-pages/'.$child['link']['mlid'].'" onclick="return confirm(\'Are you sure, you want to delete this menu item and its associated page?\');"><i class="fa fa-trash-o"></i> </a>'.'</div>';
        //$childHtml .= '<div class="'.$child['link']['mlid'].' accordionChild"><input type="text" name="abc" value="'.$child['link']['mlid'].'" /></div>';
  	    $childHtml .= '</li>';
	    }
    }
    $childHtml .= '</ol>';
  }
  return $childHtml;
}

function getFormForNavItems($mlid=NULL, $title=NULL, $child=array()){
  $form = '';
  if(!empty($mlid) && !empty($title)){
    $node_id = getNodeID($child);
    
    $form = array();
    
    $form['navItems'] = array(
      '#type' => 'fieldset',
      '#title' => t('Edit Title'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );
    
    $form['navItems']['title_'.$mlid] = array(
      '#type' => 'textfield',
      '#title' => t('Link Title'),
      '#attributes' => array('name' => 'title_'.$mlid),
      '#default_value' => $title,
      '#value' => $title,
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => TRUE,
    );
    
    $form['navItems']['depth_'.$mlid] = array(
      '#type' => 'hidden',
      '#attributes' => array('name' => 'depth_'.$mlid),
      '#value' => $child['link']['depth']
    );
    
    if($child['link']['access_callback']=='node_access'){      
        
      /*$form['navItems']['meta_keywords_'.$node_id] = array(
        '#type' => 'textarea',
        '#title' => t('Meta Keywords'),
        '#attributes' => array('name' => 'meta_keywords_'.$node_id),
        '#value' => $node_id
      );
      
      $form['navItems']['meta_description_'.$node_id] = array(
        '#type' => 'textarea',
        '#title' => t('Meta Description'),
        '#attributes' => array('name' => 'meta_description_'.$node_id),
        '#value' => $node_id
      );*/
      
      //$form['navItems']['check'] = array(
      //  '#markup' => print_r($node, true)
      //);
    }
    
    //node_access('view', $node, user_load(0))
  }
  return drupal_render($form);
}

function getNodeID($child){
  $href = $child['link']['href'];
  $node_id = strstr($href, '/');
  $node_id = str_replace("/","",$node_id);
  return $node_id;
}
