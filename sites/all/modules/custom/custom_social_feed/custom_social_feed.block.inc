<?php
/**
 * function to return twitter feed.
 */
function custom_block_social_feed() {
  drupal_add_js(drupal_get_path('module', 'custom_social_feed') . '/js/custom_social_feed.js');
  drupal_add_css(drupal_get_path('module', 'custom_social_feed') . '/css/custom_social_feed.css');
  $t_object = cache_get('twitter_feed');
  $f_object = cache_get('fb_feed');
  $i_object = custom_social_instagram_feed();
  global $base_url;
  $t_data = $f_data = $data = array();
  // twitter feed
  if (!$t_object || (isset($t_object->data) && !is_array($t_object->data))) {
    if (function_exists('custom_social_twitter_pull_render')) {
      $t_data = custom_social_twitter_pull_render(variable_get('twitter_feed_url', '@Orlando_Magic'), NULL, 5, 'twitter_feed');
      cache_set('twitter_feed', $t_data, 'cache', 6 * 60 * 60);
    }
  } else {
    $t_data = $t_object->data;
  }
  // facebook feed
  if (!$f_object || (isset($f_object->data) && !count($f_object->data))) {
    if (function_exists('custom_social_fb_feed')) {
      $profile_id = variable_get('fb_graph_id', '74947776908');
      $app_id = variable_get('fb_app_id', '497242420383769');
      $app_secret = variable_get('fb_app_secret', 'd1171bfb6c975b9557d04b3e1570dc4b');
      $authToken = custom_social_fb_feed("https://graph.facebook.com/oauth/access_token?type=client_cred&client_id={$app_id}&client_secret={$app_secret}");
      $json_object = custom_social_fb_feed("https://graph.facebook.com/{$profile_id}/posts?{$authToken}");
      $json_array = json_decode($json_object);
      if (isset($json_array->data) && count($json_array->data)) {
        $i = 0;
        foreach ($json_array->data as $key => $val) {
          if ($i >= 5) {
            break;
          }
          /*$f_data[$i]->text = isset($val->message) ? $val->message : '';
          $f_data[$i]->timestamp = isset($val->created_time) ? strtotime($val->created_time) : '';
          $f_data[$i]->userphoto = isset($val->picture) ? $val->picture : $base_url . '/' . path_to_theme() . '/images/update_normal.jpg';
          $f_data[$i]->username = isset($val->from->name) ? $val->from->name : '';
          $f_data[$i]->media = 'fb';
          $i++;*/
          //Updated by dnaik, we need to display only text message
          if(isset($val->message)){
            $f_data[$i]->text = isset($val->message) ? $val->message : '';
            $f_data[$i]->timestamp = isset($val->created_time) ? strtotime($val->created_time) : '';
            $f_data[$i]->userphoto = isset($val->picture) ? $val->picture : $base_url . '/' . path_to_theme() . '/images/update_normal.jpg';
            $f_data[$i]->username = isset($val->from->name) ? $val->from->name : '';
            $f_data[$i]->media = 'fb';
            $i++;
          }
        }
      }
    }
    cache_set('fb_feed', $f_data, 'cache', 6 * 60 * 60);
  } else {
    $f_data = $f_object->data;
  }
  $t_data = is_array($t_data) ? $t_data : array();
  $f_data = is_array($f_data) ? $f_data : array();
  $i_data = $i_object;
  if (is_array($t_data) && is_array($f_data)) {
    $data = array('facebook' => $f_data, 'twitter' => $t_data, 'instagram' => $i_data);
    //$data = array_merge($t_data, $f_data, $i_data);
    //$rand = shuffle($data);
  }
  return theme('social_feed', array('feed' => $data));
}

/**
 * Function to fetch Instagram Feed Data
 *
 */
function custom_social_instagram_feed() {
  $i_data = array();
  $object = cache_get('insta_feed');
  if (!$object) {
    $inst_access_token = variable_get('inst_access_token', '717999142.1fb234f.b202bed93603481b8fa17a4fe4793b6c');
    $user_id = variable_get('inst_user_id', '717999142');
    
    $feed_url = "https://api.instagram.com/v1/users/" . $user_id . "/media/recent/?count=5&access_token=" . $inst_access_token;
    $feeds = getInstagramFeeds($feed_url);
    $feeds = json_decode($feeds);
    
    if( isset($feeds->meta->error_type) && !empty($feeds->meta->error_type) ){
      return array();
    }
    $i = 0;
    if (isset($feeds->data) && count($feeds->data)) {
      foreach($feeds->data as $dt){
        if ($i >= 5) {
          break;
        }
        $i_data[$i]->text = isset($dt->caption->text) ? $dt->caption->text : "";
        $i_data[$i]->timestamp = isset($dt->caption->created_time) ? $dt->caption->created_time : '';
        $i_data[$i]->userphoto = isset($dt->images->thumbnail->url) ? $dt->images->thumbnail->url : $base_url . '/' . path_to_theme() . '/images/update_normal.jpg';
        $i_data[$i]->username = isset($dt->from->username) ? $dt->from->username : '';
        $i_data[$i]->media = 'instagram';
        $i++;
      }
      cache_set('insta_feed', $i_data, 'cache', 6*60*60);
    }
  }
  else {
    $i_data = $object->data;
  }
  return $i_data;	
}
